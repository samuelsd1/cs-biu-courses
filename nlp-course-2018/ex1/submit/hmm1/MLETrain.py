import utils
import sys
import timeit
from collections import Counter



def get_e_data_from_train(train_data):
    """
        data expected to be array of lines,
        each line is array of (word,tag) tuple.
        
        returns dict {word:Counter(tags)}
                in addition - returns number of words.
    """
    e_data = {}
    for line in train_data:
        for w,t in line:
            if w in e_data:
                e_data[w].update([t])
            else:
                e_data[w] = Counter([t])
    return e_data

def write_e_mle(e_data, filename):
    """
        writes the e_data
    """
    f = open(filename, 'w')
    for word in e_data:
        most_common = e_data[word].most_common()
        for tag,count in most_common:
            str = '%s %s\t%d'%(word,tag,count)
            f.write(str + '\n')
    f.close()

def read_e_mle(e_mle_fname, lower=False):
    e_data = {}
    e_mle = open(e_mle_fname, 'r')
    for line in e_mle:
        wt, num = line.strip().rsplit('\t', 1)
        word, tag = wt.split(' ')
        tag = tag.upper()
        if(lower):
            word = word.lower()
        num = int(num)

        if word not in e_data:
            e_data[word] = Counter()

        if tag in e_data[word]:
            e_data[word][tag] += num
        else:
            e_data[word][tag] = num
    e_mle.close()

    return e_data

def get_q_data_from_train(train_data):
    singles = Counter()     # counter for a single tag (c)
    doubles = Counter()     # counter for 2 consecutive tags (b c)
    triplets = Counter()    # counter for 3 consecutive tags (a b c)
    
    for line in train_data:
        # start of line, previous tags are not defined yet.
        prev_1 = None
        prev_2 = None
        
        # for each (word,tag) in a line
        for w,t in line:
            # update num of words
            
            # update the single tag counter
            singles.update([t])
            
            # if first previous tag is not defined yet, define the previous tag
            if prev_1 == None:
                prev_1 = t
                continue

            # update the double tag counter
            doubles.update([(prev_1,t)])
            
            # if second previous tag is defined, update the triplets tag counter
            if prev_2 != None:
                triplets.update([(prev_2, prev_1, t)])   
            
            # update the previous tags
            prev_2 = prev_1
            prev_1 = t
    
    return (singles, doubles, triplets)

def write_q_mle(q_data, filename):
    """
    Writes the given q_mle data to a file.
    :param q_data: tuple:
        Offset - Description
        0 - Counter of single tags
        1 - Counter of 2 consecutive tags
        2 - Counter of 3 consecutive tags
        3 - number of words
    :param filename: filename to write the data to
    :return: void
    """
    f = open(filename, 'w')
    singles, doubles, triplets = q_data

    for a in singles:
        str = '%s\t%d'%(a,singles[a])
        f.write(str + '\n')
    
    for (a,b) in doubles:
        str = '%s %s\t%d'%(a,b,doubles[(a,b)])
        f.write(str + '\n')
    
    for (a,b,c) in triplets:
        str = '%s %s %s\t%d'%(a,b,c,triplets[(a,b,c)])
        f.write(str + '\n')
    
    f.close()

def read_q_mle(q_mle_fname):
    """
    Reads an existing q.mle file
    :param q_mle_fname:
    :return:
    """
    singles = Counter()
    doubles = Counter()
    triplets = Counter()
    q_mle = open(q_mle_fname, 'r')
    for line in q_mle:
        tags, count = line.strip().split('\t')
        count = int(count)
        tags = tuple(tags.split(' '))
        if len(tags) == 1:
            singles[tags[0]] = count
        elif len(tags) == 2:
            doubles[tags] = count
        elif len(tags) == 3:
            triplets[tags] = count

    q_data = (singles, doubles, triplets)
    return q_data

def create_e_score(e_data, tag_count):
    e_score = {}
    for word in e_data:
        most_common = e_data[word].most_common()
        for tag,count in most_common:
            e_score[word,tag] = float(count)/tag_count[tag]
    return e_score

def create_q_score(q_data, num_words):
    singles, doubles, triplets = q_data

    q_score = {}
    lambda1 = 0.85
    lambda2 = 0.10
    lambda3 = 0.05

    # scores for triplets
    for t1,t2,t3 in triplets:
            abc = float(triplets[(t1,t2,t3)])
            ab = float(doubles[(t1,t2)])
            bc = float(doubles[(t2,t3)])
            b = float(singles[t2])
            c = float(singles[t3])

            q_score[(t1,t2,t3)] = lambda1 * (abc/ab) + lambda2 * (bc/b) + lambda3 * (c/num_words)

    # scores for doubles
    for t1,t2 in doubles:
        bc = float(doubles[(t1, t2)])
        b = float(singles[t1])
        c = float(singles[t2])

        q_score[(t1, t2)] = (lambda1+lambda2) * (bc / b) + lambda3 * (c / num_words)

    # scores for single tags
    for t1 in singles:
        c = float(singles[t1])
        q_score[t1] = max(lambda3*(c / num_words),1e-2)

    return q_score

def main(input_fname, q_mle_fname, e_mle_fname):
    print 'Reading Train data...',
    start_time = timeit.default_timer()
    TRAIN = utils.read_train_data(input_fname)
    print 'Done. Elapsed:',timeit.default_timer() - start_time

    print 'Processing e.mle...',
    start_time = timeit.default_timer()
    e_data = get_e_data_from_train(TRAIN)
    write_e_mle(e_data, e_mle_fname)
    print 'Done. Elapsed:', timeit.default_timer() - start_time

    print 'Processing q.mle...',
    start_time = timeit.default_timer()
    q_data = get_q_data_from_train(TRAIN)
    write_q_mle(q_data, q_mle_fname)
    print 'Done. Elapsed:', timeit.default_timer() - start_time

    #e_score = create_e_score(e_data, q_data[0])
    #q_score = create_q_score(q_data, len(e_data))
    
    print 'done'


if __name__ == '__main__':
    if len(sys.argv) != 4:
        print 'Usage: MLETrain.py <corpus_fname> <q_mle_filename> <e_mle_filename>'
        exit()
    main(sys.argv[1], sys.argv[2], sys.argv[3])
