import sys
import random

MAX_LEN = 50


def generate_sample(order):
    sample = ''
    for c in order:
        if c == 'n':
            sample += ''.join(str(random.choice(range(10))) for _ in xrange(random.randint(1, MAX_LEN)))
        else:
            sample += random.randint(1, MAX_LEN) * c
    return sample


def good_sample():
    return generate_sample('nanbncnd')


def bad_sample():
    return generate_sample('nancnbnd')


def get_sample():
    label = random.randint(0, 1)
    if label == 1:
        sample = good_sample()
    else:
        sample = bad_sample()
    return sample, label


def gen_pos_neg(pos_fname, neg_fname):
    print 'Writing POS samples...',
    with open(pos_fname, 'w') as f:
        for _ in xrange(500):
            f.write(good_sample())
            f.write('\n')
    print 'Done.'
    print 'Writing NEG samples...',
    with open(neg_fname, 'w') as f:
        for _ in xrange(500):
            f.write(bad_sample())
            f.write('\n')
    print 'Done.'


def gen_train_test(train, dev, test):
    TRAIN_SIZE = 500
    DEV_SIZE = TRAIN_SIZE / 5
    TEST_SIZE = TRAIN_SIZE / 2

    with open(train, 'w') as f:
        for _ in xrange(TRAIN_SIZE):
            sample, label = get_sample()
            f.write('%d %s\n' % (label, sample))

    with open(dev, 'w') as f:
        for _ in xrange(DEV_SIZE):
            sample, label = get_sample()
            f.write('%d %s\n' % (label, sample))

    with open(test, 'w') as f:
        with open(test + '.true', 'w') as fc:
            for _ in xrange(TEST_SIZE):
                sample, label = get_sample()
                fc.write('%d %s\n' % (label, sample))
                f.write('%s\n' % (sample))


def main(pos_fname, neg_fname):
    gen_pos_neg(pos_fname, neg_fname)
    # gen_train_test('data/train', 'data/dev', 'data/test')


if __name__ == '__main__':
    if len(sys.argv) <= 2:
        print 'Usage: <pos_fname> <neg_fname>'
        exit()
    main(sys.argv[1], sys.argv[2])
