import sys
import timeit
import numpy as np


# just for some fancy stuff, makes it easier for me :)
class Timer:
    def __init__(self, verbose=False):
        self.verbose = verbose
        self.start_called = False
        self.start_time = 0.0
        self.msg = ''

    def start(self, task_desc):
        self.start_called = True
        self.msg = task_desc

        if self.verbose:
            print 'Started %s...' % self.msg
        self.start_time = timeit.default_timer()

    def stop(self):
        if self.start_called:
            elapsed = timeit.default_timer() - self.start_time
            if self.verbose:
                print 'Done %s. elapsed: %f' % (self.msg, elapsed)
            self.start_called = False
            return elapsed
        return 0.0


# self explanatory
def read_vocab(vocab_filename):
    vocab = []
    with open(vocab_filename, 'r') as f:
        for line in f:
            vocab.append(line.strip())
    return vocab

# self explanatory
def cosine_dist(u, v):
    uv = np.dot(u, v)
    uu = np.sqrt(np.dot(u, u))
    vv = np.sqrt(np.dot(v, v))
    return uv / (uu * vv)


# Meet bob. Bob is a nice guy, and he knows how to find similarities between words.
# Bob is very smart, but you have to treat him well. If you dont, he will be sad.
# Bob Manual: instantiate Bob, and then just use bob.most_common(word,k).
# make sure Bob knows what is the word.
class BobTheWordSimilarityFinder:

    def __init__(self, vec_fname, vocab_fname):
        t = Timer(verbose=False)
        t.start('loading vector file')
        vecs = np.loadtxt(vec_fname)
        t.stop()

        t.start('loading vocab')
        vocab = read_vocab(vocab_fname)
        t.stop()

        t.start('creating map')
        self.word2vec = {word: vec for word, vec in zip(vocab, vecs)}
        t.stop()

    def most_similar(self, word, k):
        if word not in self.word2vec:
            str = 'Unknown word . Bob is sad'
            return str.split()

        u = self.word2vec[word]

        similarities = []
        for w in self.word2vec:
            v = self.word2vec[w]
            dist = cosine_dist(u, v)
            similarities.append((w, dist))

        indexes = sorted(range(len(similarities)), key=lambda x: similarities[x][1])[-k:]
        indexes.reverse()
        return [similarities[i][0] for i in indexes]


def main(vec_filename, vocab_filename):
    bob = BobTheWordSimilarityFinder(vec_filename, vocab_filename)
    words = ['dog', 'england', 'john', 'explode', 'office']

    sims = [bob.most_similar(w, 5) for w in words]

    for word, sim in zip(words, sims):
        print '%s: %s' % (word, sim)


if __name__ == '__main__':
    if len(sys.argv) <= 2:
        print 'Usage: <vecs_filename> <vocab_filename>'
        exit()

    main(sys.argv[1], sys.argv[2])
