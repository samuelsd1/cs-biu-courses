from Indexer import Indexer
from collections import Counter
import timeit
import numpy as np

import pickle

# self explanatory
def cosine_dist(u, v):
    uv = np.dot(u, v)
    uu = np.sqrt(np.dot(u, u))
    vv = np.sqrt(np.dot(v, v))
    if uu == 0 or vv == 0:
        return np.infty
    return uv / (uu * vv)

def build_vecs(sentences):
    vecs = {}
    i = 0
    for sentence in sentences:
        for w in sentence:
            if w not in vecs:
                vecs[w] = Counter()
            vecs[w].update(sentence - [w])

        i += 1
        if i % 10000 == 0:
            print 'evaluated sentence', i
    return vecs


def read_data_and_save(input_fname, samples_fname, l2i_fname):
    print 'Reading the data...'
    start_time = timeit.default_timer()
    samples, l2i = read_data(input_fname)
    elapsed = timeit.default_timer() - start_time
    print 'Elapsed:', elapsed

    print 'Saving the samples...',
    with open(samples_fname, 'wb') as f:
        pickle.dump(samples, f)
    l2i.save(l2i_fname)
    print 'done.'


def main():
    # read_data_and_save('data/wikipedia.sample.trees.lemmatized', 'samples', 'l2i')

    print 'Loading l2i...',
    l2i = Indexer.load('l2i')
    print 'Done. Nwords:', l2i.size

    print 'loading samples...',
    with open('samples', 'rb') as f:
        samples = pickle.load(f)
    print 'done. nsamples:', len(samples)

    print 'Building vectors...'
    vecs = build_vecs(samples)
    print 'Done building vectors.'

    print 'Saving vectors...',
    np.save('vecs', vecs)
    print 'Done.'

    print 'loading vectorss...',
    vecs = np.load('vecs.npy')
    print 'done.'

    l = ['car', 'bus', 'hospital', 'hotel', 'gun', 'bomb', 'horse', 'fox', 'table', 'bowl', 'guitar', 'piano']
    for word in l:
        distances = -np.ones(l2i.size)
        l_idx = l2i[word]
        for i in xrange(l2i.size):
            distances[i] = cosine_dist(vecs[l_idx], vecs[i])

        idx = np.argsort(distances)[-100:-20][::-1]
        print 'Closest words to', word
        print ' '.join(l2i(i) for i in idx)
        print ' '


if __name__ == '__main__':
    main()
