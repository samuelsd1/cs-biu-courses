import sys


def acc(true, predict):
    good = bad = 0.0
    for i in range(0, len(true)):
        if true[i] == predict[i]:
            good += 1
        else:
            bad += 1

    return good / (good + bad)

def main(orig_fname, test_fname):
    orig_tags = []
    with open(orig_fname,'r') as file:
        for line in file:
            sentence = line.strip().split(' ')
            for wt in sentence:
                _,t = line.rsplit('/',1)
                orig_tags.append(t)

    test_tags = []
    with open(test_fname,'r') as file:
        for line in file:
            sentence = line.strip().split(' ')
            for wt in sentence:
                _,t = line.rsplit('/',1)
                test_tags.append(t)

    print 'Accuracy:',acc(orig_tags,test_tags)


if __name__ == '__main__':
    if len(sys.argv) <= 2:
        print 'Usage: measure_acc <original_fname> <test_fname>'
        exit()

    main(sys.argv[1],sys.argv[2])