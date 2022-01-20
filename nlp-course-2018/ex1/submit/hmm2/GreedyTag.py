import MLETrain as mt
import sys
import timeit
from Indexer import Indexer
import Tagger

def dict_max_argmax(dict):
    maxval = max(dict.values())
    for k in dict:
        if dict[k] == maxval:
            return k,maxval

class GreedyTagger(Tagger.Tagger):
    def __init__(self,score_func,tag_indexer, word_to_tags):
        self.word_to_tags = word_to_tags
        self.tag_indexer = tag_indexer
        self.score_func = score_func

    def tag_sentence(self, sentence):
        words = sentence.split(' ')
        n = len(words)
        result = []

        ppt = pt = None
        for i in range(n):
            # current word
            w = words[i]

            # got the idea from Tamir
            if w in self.word_to_tags:
                curr_tagset = set(self.word_to_tags[w])
            else:
                curr_tagset = self.tag_indexer.obj_set

            score = {}
            for t in curr_tagset:
                score[t] = self.score_func(w,t,pt,ppt)

            maxtag,_ = dict_max_argmax(score)
            ppt = pt
            pt = maxtag
            result.append(maxtag)
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

    tagger = GreedyTagger(get_score, tag_indexer, e_data)
    Tagger.TagFile(input_fname, out_fname, tagger,lower=True,verbose=True)
    print 'done'


if __name__ == '__main__':
    if len(sys.argv) <= 4:
        print 'Usage: HMMTag.py <input_filename> <q_mle_filename> <e_mle_filename> <output_file_name>'
        exit()

    main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
