from collections import Counter
from Indexer import Indexer

CONTENT_CLASSES = ['JJ', 'JJR', 'JJS', 'NN', 'NNS', 'NNP', 'NNPS', 'RB', 'RBR', 'RBS', 'VB', 'VBD', 'VBG', 'VBN', 'VBP',
                   'VBZ', 'WRB']


def read_data(filename):
    lemmas_count = Counter()
    samples = []

    with open(filename, 'r') as f:
        lemmas = []
        for line in f:
            line = line.strip()
            if len(line) == 0:
                if len(lemmas) < 1:
                    continue

                lemmas_count.update(lemmas)
                samples.append(lemmas)
                lemmas = []
                continue

            s = line.split()
            # word = s[1]
            lemma = s[2]
            tag = s[4]

            if tag not in CONTENT_CLASSES:
                continue

            lemmas.append(lemma)

    # filter the uncommon words
    for lemmas in samples:
        for lemma in lemmas:
            if lemma not in lemmas_count:
                continue
            if lemmas_count[lemma] < 100:
                del lemmas_count[lemma]

    l2i = Indexer(lemmas_count)

    indexed_samples = []
    for lemmas in samples:
        indexed_lemmas = []
        for l in lemmas:
            if l in l2i:
                indexed_lemmas.append(l2i[l])
        if len(indexed_lemmas) > 0:
            indexed_samples.append(indexed_lemmas)

    del samples

    return indexed_samples, l2i
