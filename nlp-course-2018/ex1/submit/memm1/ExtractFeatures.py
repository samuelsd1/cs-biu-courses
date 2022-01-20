import utils
import timeit
from collections import Counter
import sys

def give_features(w,is_rare,nw,nnw,pw,pt,ppw,ppt):
    """

    :param w:       Current Word
    :param is_rare: Bool signifies if the current word is rare
    :param nw:      Next word
    :param nnw:     Next next word
    :param pw:      Previous word
    :param pt:      Previous word tag
    :param ppw:     Previous previous word
    :param ppt:     Previous previous word tag
    :return:        list of features
    """
    feats = []
    def add(feat,arg):
        if arg != None:
            feats.append('%s=%s'%(feat,arg))
            if str(arg).lower() == 'falsevb':
                raise NameError

    add('pt',pt)
    add('ppt',ppt)
    add('pw',pw)
    add('ppw',ppw)
    add('nw',nw)
    add('nnw',nnw)

    if pt != None and ppt != None:
        add('ppt_pt',ppt+pt)

    if not is_rare:
        add('w', w)
    else:
        #prefixes
        s = min([len(w),4])
        for i in range(1,s+1):
            add('pref%d'%i,w[:i])
        #suffixes
        for i in range(i,s+1):
            add('suff%d'%i,w[-i:])

        add('hasNum',any(ch.isdigit() for ch in w))
        add('hasUpper',any(ch.isupper() for ch in w))
        hasHyphen = bool('-' in w)
        add('hasHyphen',hasHyphen)

    return feats

def main(corpus_fname, features_fname):

    print 'Reading train data...',
    start_time = timeit.default_timer()
    train_data = utils.read_train_data(corpus_fname)
    print 'Done. Elapsed:',timeit.default_timer() - start_time

    print 'Counting words...',
    word_count = Counter()
    start_time = timeit.default_timer()
    for line in train_data:
        for w,_ in line:
            word_count.update([w])
    print 'Done. Elapsed:', timeit.default_timer() - start_time

    # now extract all features from the file.
    print 'Writing features file...',
    start_time = timeit.default_timer()
    with open(features_fname, 'w') as feats_file:
        for line in train_data:

            k = len(line)
            pw = None
            ppw = None
            pt = None
            ppt = None

            for i in range(k):
                # current word
                w,t = line[i]

                # next words
                nw,_ = line[i + 1] if i + 1 < k else (None, None)
                nnw,_ = line[i + 2] if i + 2 < k else (None, None)

                is_rare = word_count[w] <= 5
                feats = give_features(w,is_rare,nw,nnw,pw,pt,ppw,ppt)

                features_str = t
                for f in feats:
                    features_str += ' %s'%f
                features_str += '\n'
                feats_file.write(features_str)

                # end of iteration for this word, update the previous tags and words
                ppw = pw
                pw = w
                ppt = pt
                pt = t

    print 'Done. Elapsed:', timeit.default_timer() - start_time
    print 'done'

if __name__ == '__main__':
    if len(sys.argv) <= 2:
        print 'Usage: ExtractFeatures.py <corpus-fname> <features_fname>'
        exit()
    main(sys.argv[1],sys.argv[2])