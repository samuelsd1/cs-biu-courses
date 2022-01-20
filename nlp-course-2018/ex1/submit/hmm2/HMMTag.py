import MLETrain as mt
import sys
import timeit
import random
from Indexer import Indexer
import numpy as np
import Tagger

def dict_max_argmax(dict):
    maxval = max(dict.values())
    for k in dict:
        if dict[k] == maxval:
            return k,maxval

class HMMTagger(Tagger.Tagger):
    def __init__(self,score_func,tag_indexer, word_to_tags):
        self.word_to_tags = word_to_tags
        self.tag_indexer = tag_indexer
        self.score_func = score_func

    def tag_sentence(self, sentence):
        _start = '_START_'
        extra_tags = self.tag_indexer.obj_set.union([_start])
        extra_tag_indexer = Indexer(extra_tags)

        words = sentence.split(' ')

        n = len(words)
        k = len(extra_tags)

        V = np.zeros((n+1,k,k))
        bp = np.zeros((n,k,k))

        startidx = extra_tag_indexer.to_index(_start)
        V[0,startidx,startidx] = 1

        pp_tagset = [_start]
        p_tagset = [_start]

        Y = np.zeros(n, dtype=int)

        for i in range(n):
            # current word
            w = words[i]

            # got the idea from Tamir
            if w in self.word_to_tags:
                curr_tagset = set(self.word_to_tags[w])
            else:
                curr_tagset = self.tag_indexer.obj_set

            for pt in p_tagset:
                pt_idx = extra_tag_indexer.to_index(pt)
                for t in curr_tagset:
                    scores = {}
                    t_x_idx = extra_tag_indexer.to_index(t)
                    for ppt in pp_tagset:
                        ppt_idx = extra_tag_indexer.to_index(ppt)
                        vi = V[i,ppt_idx,pt_idx]
                        score = self.score_func(w,t,pt,ppt)
                        scores[ppt] = vi*score

                    maxtag,maxscore = dict_max_argmax(scores)
                    V[i+1,pt_idx,t_x_idx] = maxscore
                    bp[i,pt_idx,t_x_idx] = extra_tag_indexer.to_index(maxtag)

            pp_tagset = p_tagset
            p_tagset = curr_tagset

        (Y[n - 2], Y[n - 1]) = np.unravel_index(np.argmax(V[n]), (k, k))
        for i in xrange(n - 3, -1, -1):
            Y[i] = bp[i + 2, Y[i + 1], Y[i + 2]]

        result = []
        for y in Y:
            tag = extra_tag_indexer.from_index(y)
            if tag == _start:
                tag = random.choice(sorted(self.tag_indexer.obj_set))
            result.append(tag)

        return result


def main(input_fname, q_mle_fname,e_mle_fname, out_fname):
    e_data = mt.read_e_mle(e_mle_fname, lower=True)
    q_data = mt.read_q_mle(q_mle_fname)
    e_score = mt.create_e_score(e_data, q_data[0])
    q_score = mt.create_q_score(q_data, len(e_data))
    singles,_,_ = q_data
    tag_indexer = Indexer(set(singles))

    num_words = float(len(e_data))
    num_tags = float(len(singles))
    def get_score(word, tag, prev_tag=None, prev_prev_tag=None):
        escore = e_score.get((word, tag), num_tags/num_words)

        if prev_prev_tag == None:
            if prev_tag == None:
                qscore = q_score[tag]
            else:
                qscore = q_score.get((prev_tag, tag), q_score[tag])
        else:
            qscore = q_score.get((prev_prev_tag, prev_tag, tag), q_score.get((prev_tag, tag), q_score[tag]))

        return escore * qscore

    tagger = HMMTagger(get_score, tag_indexer, e_data)
    Tagger.TagFile(input_fname, out_fname, tagger,lower=True,verbose=True)
    print 'Done tagging :)'


if __name__ == '__main__':
    if len(sys.argv) <= 4:
        print 'Usage: HMMTag.py <input_filename> <q_mle_filename> <e_mle_filename> <output_file_name>'
        exit()

    main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
