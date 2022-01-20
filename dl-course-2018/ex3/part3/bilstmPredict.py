import os
import sys
import NeuralNets
import dynet as dy
from Indexer import Indexer
from zipfile import ZipFile


def read_unlabeled_data(fname, lower=False):
    sentences = []
    with open(fname, 'r') as f:
        curr = None
        for line in f:
            if lower:
                line = line.lower()
            word = line.strip()

            # if reached an empty line (sentence separator)
            if len(word) <= 0:
                # append the current sentence to the sentences list
                if curr is not None:
                    sentences.append(curr)
                    curr = None
                continue

            # if no current sentence defined, create it
            if curr is None:
                curr = []
            curr.append(word)

    return sentences


def main(repr, modelfile, inputfile, outputfile):
    if repr not in 'abcd':
        raise Exception('bad repr format')

    sentences = read_unlabeled_data(inputfile, lower=True)

    with ZipFile(modelfile, 'r') as f:
        filenames = f.namelist()
        f.extractall()

    w2iname = ''
    t2iname = ''
    c2iname = ''
    modelname = ''
    p2iname = ''
    s2iname = ''
    for f in filenames:
        if f.startswith('w2i'):
            w2iname = f
        elif f.startswith('t2i'):
            t2iname = f
        elif f.startswith('c2i'):
            c2iname = f
        elif f.startswith('p2i'):
            p2iname = f
        elif f.startswith('s2i'):
            s2iname = f
        elif f.startswith('model'):
            modelname = f
        else:
            raise Exception('Unknown filename ' + f)

    w2i = Indexer.load(w2iname)
    t2i = Indexer.load(t2iname)
    c2i = Indexer.load(c2iname)

    pc = dy.ParameterCollection()
    if repr == 'a':
        network = NeuralNets.AModel(pc, w2i, t2i)
    elif repr == 'b':
        network = NeuralNets.BModel(pc, w2i, t2i, c2i)
    elif repr == 'c':
        p2i = Indexer.load(p2iname)
        s2i = Indexer.load(s2iname)
        network = NeuralNets.CModel(pc, w2i, t2i, p2i, s2i)
    else:
        network = NeuralNets.DModel(pc, w2i, t2i, c2i)

    pc.populate(modelname)

    # cleanup
    for f in filenames:
        os.remove(f)

    print 'Predicting...',
    with open(outputfile, 'w') as f:
        for sentence in sentences:
            tags = network.create_network_return_best(sentence)

            s = ''
            for w, t in zip(sentence, tags):
                s += '%s %s\n' % (w, t)
            s += '\n'
            f.write(s)
    print 'Done'


if __name__ == '__main__':
    if len(sys.argv) <= 4:
        print 'Usage: <repr> <modelfile> <inputfile> <outputfile>'
        exit()
    main(*sys.argv[1:])
