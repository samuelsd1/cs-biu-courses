import timeit


def read_labeled_data(fname, lower=False):
    word_set = set()
    tag_set = set()
    labeled_data = []
    with open(fname, 'r') as f:
        words = None
        tags = None
        for line in f:
            line = line.strip()

            # if reached an empty line (sentence separator)
            if len(line) <= 0:
                # append the current sentence to the sentences list
                if words is not None:
                    labeled_data.append((words, tags))
                    words = None
                    tags = None
                continue

            # if no current sentence defined, create it
            if words is None:
                words = []
                tags = []
            word, tag = line.rsplit(None, 1)
            if lower:
                word = word.lower()
            words.append(word)
            tags.append(tag)

            word_set.add(word)
            tag_set.add(tag)

    return labeled_data, word_set, tag_set


def read_unlabeled_data(fname, lower=False):
    sentences = []
    with open(fname, 'r') as f:
        curr = None
        for line in f:
            word = line.strip()
            if lower:
                line = line.lower()

            # if reached an empty line (sentence separator)
            if len(word) <= 0:
                # append the current sentence to the sentences list
                if curr is not None:
                    sentences.append(curr)
                    curr = None
                continue

            # if no current sentence defined, create it
            if curr is None:
                curr = []
            curr.append(word)

    return sentences


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
