START = '$_start_$'
END = '$_end_$'
UNK = '$_UNK_$'


def read_labeled_data(fname):
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
            words.append(word)
            tags.append(tag)

            word_set.add(word)
            tag_set.add(tag)

    return labeled_data, word_set, tag_set


def read_unlabeled_data(fname):
    sentences = []
    with open(fname, 'r') as f:
        curr = None
        for line in f:
            word = line.strip()
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


def pad_list(l):
    padded = [START, START]
    padded.extend(l)
    padded.extend([END, END])
    return padded


def get_window(padded_list, i):
    w = padded_list[i]
    pw = padded_list[i - 1]
    ppw = padded_list[i - 2]
    nw = padded_list[i + 1]
    nnw = padded_list[i + 2]

    return [ppw, pw, w, nw, nnw]
