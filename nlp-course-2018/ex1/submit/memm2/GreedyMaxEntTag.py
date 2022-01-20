import sys
import timeit
from collections import Counter

import Tagger
from Indexer import Indexer
import ExtractFeatures as ef

import numpy as np
from scipy.sparse import lil_matrix
from pickle import load as loadModel


def get_trained_data(model_filename):
    with open(model_filename, 'r') as model_file:
        trained_data = loadModel(model_file)
    return trained_data

def dict_max_argmax(dict):
    maxval = max(dict.values())
    for k in dict:
        if dict[k] == maxval:
            return k,maxval

class GreedyMaxEntTagger(Tagger.Tagger):
    def __init__(self,model_fname,feat_map_fname,word_to_tags,tag_indexer):
        self.predictor = get_trained_data(model_fname)
        self.feat_indexer = Indexer.load(feat_map_fname)
        self.word_to_tags = word_to_tags
        self.tag_indexer = tag_indexer

    def tag_sentence(self, sentence):
        words = sentence.split(' ')
        n = len(words)
        result = []

        ppw = pw = None
        ppt = pt = None
        for i in range(n):
            # current word
            w = words[i]
            # next words
            nw = words[i + 1] if i + 1 < n else (None, None)
            nnw = words[i + 2] if i + 2 < n else (None, None)

            # got the idea from Tamir
            if w in self.word_to_tags:
                curr_tagset = set(self.word_to_tags[w])
            else:
                curr_tagset = self.tag_indexer.obj_set

            scores = {}
            for t in curr_tagset:
                feats = ef.give_features(w, False, nw, nnw, pw, pt, ppw, ppt)
                feat_vec = lil_matrix((1, self.feat_indexer.size), dtype=np.int8)
                for f in feats:
                    if self.feat_indexer.contains_key(f):
                        feat_vec[0, self.feat_indexer.to_index(f)] = 1
                del feats
                # now predict the tag.
                prediction = self.predictor.predict_proba(feat_vec)
                pred_score = prediction[0, self.tag_indexer.to_index(t)]
                scores[t] = pred_score

            maxtag, _ = dict_max_argmax(scores)
            ppw = pw
            pw = w
            ppt = pt
            pt = maxtag
            result.append(maxtag)
        return result



def main(input_fname,model_filename,feature_map_fname,out_fname):
    tag_indexer = Indexer.load('tagset_map')

    print 'Extracting some features...'
    with open(feature_map_fname, 'r') as fmap_file:
        word_to_labels = {}
        saw_seperator = False
        for line in fmap_file:
            line = line.strip()
            if not saw_seperator:
                if '%%%%%' in line:
                    saw_seperator = True
                continue
            else:
                splitted = line.split(' ')
                w = splitted[0]
                word_to_labels[w] = splitted[1:]

    print 'Creating the tagger...',
    start_time = timeit.default_timer()
    tagger = GreedyMaxEntTagger(model_filename, feature_map_fname, word_to_labels, tag_indexer)
    print 'Done. Elapsed', timeit.default_timer() - start_time

    Tagger.TagFile(input_fname, out_fname, tagger, verbose=True)
    print 'Done tagging.'



if __name__ == '__main__':
    if len(sys.argv) <= 4:
        print 'Usage: GreedyMaxEntTag.py <input_fname> <model_fname> <feat_map_fname> <out_fname>'
        exit()
    main(sys.argv[1],sys.argv[2],sys.argv[3],sys.argv[4])