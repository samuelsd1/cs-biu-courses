import os
import sys
import random
import dynet as dy
import numpy as np
import timeit
from zipfile import ZipFile

from Indexer import Indexer
import NeuralNets

import pickle

MODEL = 'POS'


def read_labeled_data(fname, lower=False):
    word_set = set()
    tag_set = set()
    char_set = set()
    labeled_data = []
    with open(fname, 'r') as f:
        words = None
        tags = None
        for line in f:
            line = line.strip()

            # if reached an empty line (sentence separator)
            if len(line) <= 0:
                # append the current sentence to the sentences list
                if words is not None:
                    labeled_data.append((words, tags))
                    words = None
                    tags = None
                continue

            # if no current sentence defined, create it
            if words is None:
                words = []
                tags = []
            word, tag = line.rsplit(None, 1)
            if lower:
                word = word.lower()
            words.append(word)
            tags.append(tag)

            word_set.add(word)
            tag_set.add(tag)
            char_set.update(word)

    return labeled_data, word_set, tag_set, char_set


def get_affixes(wordset):
    pref = set()
    suff = set()
    for w in wordset:
        if len(w) < 3:
            continue
        pref.add(w[:3])
        suff.add(w[-3:])
    return pref, suff


def acc_on_dataset(dataset, network):
    good = bad = 0.0
    for word_list, tag_list in dataset:
        prediction = network.create_network_return_best(word_list)
        for pred, lbl in zip(prediction, tag_list):
            if MODEL == 'NER' and lbl == 'O':
                continue
            if pred == lbl:
                good += 1
            else:
                bad += 1

    return good / (good + bad)


def main(repr, trainfile, modelfile, devfile=None):
    if repr not in 'abcd':
        raise Exception('bad repr format')

    print 'Reading Train',
    start_time = timeit.default_timer()
    train, wordset, tagset, charset = read_labeled_data(trainfile, lower=True)
    print 'Done. elapsed:', timeit.default_timer() - start_time

    if devfile is not None and devfile != '-n':
        dev, _, _, _ = read_labeled_data(devfile, lower=True)
    else:
        tenth = len(train) / 10
        tenth = min(tenth, 2000)
        dev = train[:tenth]
        train = train[tenth:]

    charset.add(NeuralNets.SOE)
    w2i = Indexer(wordset, NeuralNets.UNK)
    t2i = Indexer(tagset, NeuralNets.UNK)
    c2i = Indexer(charset)

    pc = dy.ParameterCollection()
    if repr == 'a':
        network = NeuralNets.AModel(pc, w2i, t2i)
    elif repr == 'b':
        network = NeuralNets.BModel(pc, w2i, t2i, c2i)
    elif repr == 'c':
        pref, suff = get_affixes(wordset)
        p2i = Indexer(pref, NeuralNets.UNK)
        s2i = Indexer(suff, NeuralNets.UNK)
        network = NeuralNets.CModel(pc, w2i, t2i, p2i, s2i)
    else:
        network = NeuralNets.DModel(pc, w2i, t2i, c2i)

    trainer = dy.AdamTrainer(pc)
    # train network
    dev_accs = []
    print 'dev acc \tavg. loss\telapsed'
    for epoch in xrange(5):
        losses = []
        i = 0
        random.shuffle(train)
        start_time = timeit.default_timer()
        for words, tags in train:
            loss = network.create_network_return_loss(words, tags)
            losses.append(loss.value())  # need to run loss.value() for the forward prop
            loss.backward()
            trainer.update()

            i += 1
            if i % 500 == 0:
                avg = np.average(losses)
                losses = []

                #################################################
                #                                               #
                #   DISABLE DEV PREDICTION FOR TRAIN SPEEDUP!   #
                #                                               #
                #################################################
                dev_acc = acc_on_dataset(dev, network)
                dev_accs.append(dev_acc)

                elapsed = timeit.default_timer() - start_time
                start_time = timeit.default_timer()
                print '%f\t%f\t%f'%(dev_acc, avg, elapsed)

        print 'Finished epoch #%d' % (epoch + 1)

    # save for later graph

    rand = random.randint(0, 10000)
    devfile = 'accs_%s_%d.pkl' % (repr, rand)
    with open(devfile, 'wb') as f:
        pickle.dump(dev_accs, f)

    # save everything
    files_to_zip = ['model_%d.mdl' % rand, 'w2i_%d.vocab' % rand, 't2i_%d.vocab' % rand, 'c2i_%d.vocab' % rand]
    pc.save(files_to_zip[0])
    w2i.save(files_to_zip[1])
    t2i.save(files_to_zip[2])
    c2i.save(files_to_zip[3])
    if repr == 'c':
        l = ['p2i_%d.vocab' % rand, 's2i_%d.vocab' % rand]
        files_to_zip.extend(l)
        p2i.save(l[0])
        s2i.save(l[1])

    with ZipFile(modelfile, 'w') as trick:
        for f in files_to_zip:
            trick.write(f)

    # cleanup
    for f in files_to_zip:
        os.remove(f)


if __name__ == '__main__':
    if len(sys.argv) <= 3:
        print 'Usage: <repr> <trainfile> <modelfile> [Optional: devfile] [Optional: -n]'
        exit()
    if '-n' in sys.argv:
        sys.argv.remove('-n')
        MODEL = 'NER'

    print 'model is',MODEL
    main(*sys.argv[1:])
