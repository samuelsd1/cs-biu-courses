import sys
import random
import timeit

import dynet as dy
import numpy as np

from Indexer import Indexer
import utils


class OurNetwork(object):
    def __init__(self, pc, vecs, tagset_size):
        edim = len(vecs[0])
        wsize = 5
        indim = edim * wsize

        hdim = 100

        self.pW1 = pc.add_parameters((hdim, indim))
        self.pb1 = pc.add_parameters(hdim)

        self.pW2 = pc.add_parameters((tagset_size, hdim))
        self.pb2 = pc.add_parameters(tagset_size)

        self.lookup = pc.add_lookup_parameters((len(vecs), edim))
        self.lookup.init_from_array(vecs)

    def __call__(self, inputs):
        W1 = dy.parameter(self.pW1)
        b1 = dy.parameter(self.pb1)
        W2 = dy.parameter(self.pW2)
        b2 = dy.parameter(self.pb2)
        lookup = self.lookup
        emb_vectors = [lookup[i] for i in inputs]
        net_input = dy.concatenate(emb_vectors)

        layer1 = ((W1 * net_input) + b1)  # Wx + b
        out_layer = ((W2 * dy.tanh(layer1)) + b2)  # U * tanh(layer1) + bt
        net_output = dy.softmax(out_layer)  # softmax on everything
        return net_output

    def create_network_return_loss(self, inputs, expected_output):
        dy.renew_cg()
        out = self(inputs)
        loss = -dy.log(dy.pick(out, expected_output))
        return loss

    def create_network_return_best(self, inputs):
        dy.renew_cg()
        out = self(inputs)
        return np.argmax(out.npvalue())


def main(train_fname, dev_fname, vocab_fname, vec_fname, test_fname, output_fname):
    pass
    train_data, train_wordset, train_tagset = utils.read_labeled_data(train_fname, lower=True)
    tag_indexer = Indexer(train_tagset.union([utils.START, utils.UNK, utils.END]))

    vocab = {}
    with open(vocab_fname, 'r') as f:
        for i, line in enumerate(f):
            word = line.strip()
            vocab[word] = i

    for w in train_wordset:
        if w not in vocab:
            vocab[w] = vocab[utils.UNK]

    vecs = np.loadtxt(vec_fname)

    dev_data, _, _ = utils.read_labeled_data(dev_fname, lower=True)

    m = dy.ParameterCollection()
    network = OurNetwork(m, vecs, tag_indexer.size)
    trainer = dy.AdamTrainer(m)

    def predict(word_list):
        padded = utils.pad_list(word_list)
        predictions = []
        for i in xrange(2, len(padded) - 2):
            window = utils.get_window(padded, i)
            inputs = []

            # if a word is unknown - treat it as UNKNOWN
            for word in window:
                if word in vocab:
                    inputs.append(vocab[word])
                else:
                    inputs.append(vocab[utils.UNK])
            pred = network.create_network_return_best(inputs)
            predictions.append(tag_indexer.from_index(pred))
        return predictions

    def acc_on_dataset(labeled_data):
        good = bad = 0.0
        for wordlist, taglist in labeled_data:
            prediction = predict(wordlist)

            for pred, lbl in zip(prediction, taglist):
                if pred == lbl:
                    good += 1
                else:
                    bad += 1

        return good / (good + bad)

    def tag(input_filename, output_filename):
        infile = utils.read_unlabeled_data(input_filename)
        with open(output_filename, 'w') as f:
            for wordlist in infile:
                predlist = predict(wordlist)

                s = ''
                for word, pred in zip(wordlist, predlist):
                    s += '%s %s\n' % (word, pred)
                s += '\n'
                f.write(s)

    def train(num_epochs, verbose=False, report_acc=False):
        for epoch in xrange(num_epochs):
            total_loss = 0.0
            seen_instances = 0
            num_sentences = 0
            random.shuffle(train_data)

            start_time = timeit.default_timer()
            for wordlist, taglist in train_data:
                padded_wordlist = utils.pad_list(wordlist)
                padded_taglist = utils.pad_list(taglist)

                for i in xrange(2, len(padded_wordlist) - 2):
                    t = padded_taglist[i]

                    window = utils.get_window(padded_wordlist, i)
                    inputs = [vocab[word] for word in window]
                    lbl = tag_indexer.to_index(t)

                    dy.renew_cg()
                    out = network(inputs)
                    loss = -dy.log(dy.pick(out, lbl))
                    seen_instances += 1
                    total_loss += loss.value()
                    loss.backward()
                    trainer.update()

                num_sentences += 1
                if verbose:
                    if num_sentences % 2000 == 0:
                        print 'epoch:', (epoch + 1),
                        print '\tsentence:', num_sentences,
                        print '\tAvg. loss:', total_loss / seen_instances

            if verbose:
                print '=' * 60
                elapsed = timeit.default_timer() - start_time
                print 'epoch: %d,evaluated_sentences: %d,elapsed: %d' % (epoch + 1, num_sentences, elapsed)
                if report_acc:
                    print 'evaluating train acc...',
                    start_time = timeit.default_timer()
                    train_acc = acc_on_dataset(train_data)
                    print 'Done. elapsed:', timeit.default_timer() - start_time

                    print 'evaluating dev acc...',
                    start_time = timeit.default_timer()
                    dev_acc = acc_on_dataset(dev_data)
                    print 'Done. elapsed:', timeit.default_timer() - start_time
                    print ''
                    print 'Train Accuracy:\t', train_acc
                    print 'Dev Accuracy:\t', dev_acc
                print '=' * 60

    # The reason i turned everything into functions INSIDE the main func, is because it was inside
    # a class first, but for some reason dynet always dropped at trainer.update() because
    # what so called MemoryError: std::bad_alloc.
    # I didnt want to mess with that, because it was working, and moving it into a class
    # caused it not to work.
    # But - I saved a code file that creates that error, and will submit it to Yoav if needed,
    # so we can try to find a bug in my code, or find a bug in dynet.
    #
    # this time - I believe the bug is either in dynet or in the relationship between python and dynet,
    # because after that class didnt work , I literally copied all the functions inside the main
    # class, and removed the self. from all the variables, and it worked, strangely.

    train(5, verbose=True, report_acc=True)
    tag(test_fname, output_fname)


if __name__ == '__main__':
    if len(sys.argv) <= 6:
        print 'Usage: <train_fname> <dev_fname> <vocab_fname> <vecs_fname> <test_fname> <output_fname>'
        exit()

    main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6])
