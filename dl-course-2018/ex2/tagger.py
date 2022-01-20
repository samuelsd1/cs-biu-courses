import sys

import dynet as dy
import numpy as np

import NeuralNets
import Taggers
import utils


def print_usage():
    print 'Usage: tagger <train> <test> <output> [OPTIONS]'
    print ''
    print 'Options:             Meaning'
    print ' -h                  Prints the help menu'
    print ' -d <dev>            Enables accuracy report during the train'
    print ' -v <vocab> <vecs>   Enables using pre-trained word vectors'
    print ' -s                  Enables sub-words units'


def bad_args():
    print 'Usage: tagger <train> <test> <output> [OPTIONS]'
    print 'Type \'tagger -h\' for a list of available options.'
    exit()


class TaggerDeployer(object):
    def __init__(self, tagger, train_data, dev_data=None):
        self.tagger = tagger
        self.train_data = train_data
        self.dev_data = dev_data

    def train(self, num_iters, print_each=2000):
        for epoch in xrange(num_iters):
            self.tagger.train_iter(self.train_data, print_each)

            # if I should report accuracy
            if self.dev_data is not None:
                print '=' * 60
                t = utils.Timer(verbose=True)

                t.start('evaluating train acc')
                train_acc = self.tagger.acc_on_dataset(self.train_data)
                t.stop()

                t.start('evaluating dev acc')
                dev_acc = self.tagger.acc_on_dataset(self.dev_data)
                t.stop()

                print ''
                print 'Train Accuracy:\t', train_acc
                print 'Dev Accuracy:\t', dev_acc
                print '=' * 60

    def tag_file(self, input_filename, output_filename, lower=False):
        unlabeled_input = utils.read_unlabeled_data(input_filename, lower=lower)

        with open(output_filename, 'w') as f:
            for word_list in unlabeled_input:
                predictions = self.tagger.predict_tags(word_list)

                s = ''
                for word, pred in zip(word_list, predictions):
                    s += '%s %s\n' % (word, pred)
                s += '\n'

                f.write(s)


def generate_vocab(word_set):
    return {w: i for i, w in enumerate(sorted(word_set))}


def read_vocab(vocab_f, vec_f):
    vocab = {}
    with open(vocab_f, 'r') as f:
        for i, line in enumerate(f):
            word = line.strip()
            vocab[word] = i

    vecs = np.loadtxt(vec_f)
    return vocab, vecs


def generate_affix_sets(word_set):
    pref = set()
    suff = set()
    for w in word_set:
        if w in [Taggers.START, Taggers.UNK, Taggers.END]:
            continue
        if len(w) >= 3:
            pref.add(w[:3])
            suff.add(w[-3:])

    pref.update([Taggers.START, Taggers.UNK, Taggers.END])
    suff.update([Taggers.START, Taggers.UNK, Taggers.END])

    return pref, suff


def main(train_f, test_f, output_f, dev_f=None, vocab_f=None, vecs_f=None, affix=False):
    vocab_exists = False
    if vocab_f is not None:
        vocab_exists = True
    train, train_wordset, train_tagset = utils.read_labeled_data(train_f, lower=vocab_exists)
    train_tagset.update([Taggers.START, Taggers.END, Taggers.UNK])
    train_wordset.update([Taggers.START, Taggers.END, Taggers.UNK])

    dev = None
    if dev_f is not None:
        dev, _, _ = utils.read_labeled_data(dev_f, lower=vocab_exists)

    emb_size = 50
    if vocab_exists:
        vocab, vecs = read_vocab(vocab_f, vecs_f)

        emb_size = len(vecs[0])
        # vocabulary has to contain UNK, START,END.
        # for each word in the training set, if its not in the vocabulary,
        # treat it as unknown
        for w in train_wordset:
            if w not in vocab:
                vocab[w] = vocab[Taggers.UNK]
    else:
        vocab = generate_vocab(train_wordset)

    m = dy.ParameterCollection()

    if not affix:
        network = NeuralNets.BasicNetwork(m, len(vocab), emb_size, len(train_tagset))
    else:
        pref, suff = generate_affix_sets(vocab)
        pref_vocab = generate_vocab(pref)
        suff_vocab = generate_vocab(suff)
        network = NeuralNets.AffixNetwork(m, len(vocab), emb_size, len(train_tagset), len(pref))

    if vocab_exists:
        network.set_word_emb_vecs(vecs)

    trainer = dy.AdamTrainer(m)

    if not affix:
        tagger = Taggers.Tagger(train_tagset, vocab, network, trainer)
    else:
        tagger = Taggers.AffixTagger(train_tagset, vocab, pref_vocab, suff_vocab, network, trainer)

    # deploy the tagger.
    deployer = TaggerDeployer(tagger, train, dev)
    deployer.train(5)
    deployer.tag_file(test_f, output_f)


if __name__ == '__main__':
    # args: <train_fname> <test_fname> <output_fname> [Optional] -d <dev> -v <vocab> <vecs> -s
    for arg in sys.argv:
        if arg == '-h':
            print_usage()
            exit()

    arglen = len(sys.argv)
    if arglen <= 3:
        bad_args()

    flags = ['-h', '-d', '-v', '-s']

    train_f = sys.argv[1]
    test_f = sys.argv[2]
    output_f = sys.argv[3]

    if train_f in flags:
        print 'Missing \'train\' argument.'
        bad_args()
    if test_f in flags:
        print 'Missing <test> argument.'
        bad_args()
    if output_f in flags:
        print 'Missing <output> argument.'
        bad_args()

    sub = False
    dev_f = None
    vocab_f = None
    vecs_f = None

    i = 4
    while i < arglen:
        arg = sys.argv[i]
        if arg not in flags:
            print 'Unexpected argument \'%s\'' % arg
            bad_args()
        if arg == '-s':
            sub = True
        elif arg == '-d':
            if i + 1 >= arglen or sys.argv[i + 1] in flags:
                print 'tagger: option \'-d\' requires an argument.'
                print 'Type \'tagger -h\' for a list of available options.'
                exit()
            dev_f = sys.argv[i + 1]
            i += 1
        elif arg == '-v':
            # expecting 2 more parameters,at i+1 i+2
            if i + 2 >= arglen or sys.argv[i + 1] in flags or sys.argv[i + 2] in flags:
                print 'tagger: option \'-v\' requires two arguments.'
                print 'Type \'tagger -h\' for a list of available options.'
                exit()
            vocab_f = sys.argv[i + 1]
            vecs_f = sys.argv[i + 2]
            i += 2
        i += 1

    main(train_f, test_f, output_f, dev_f, vocab_f, vecs_f, sub)
