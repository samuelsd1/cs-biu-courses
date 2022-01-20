def read_train_data(fname, lower=False):
    """
    Reads the training data.
    :param fname:
    :param lower:
    :return: list of lists of (word,tag)
    """
    data = []
    i = 0
    for line in file(fname):
        if lower:
            words = line.lower().strip().split(" ")
        else:
            words = line.strip().split(" ")
        linedata = []
        for w in words:
            word, tag = w.rsplit("/", 1)
            tag = tag.upper()
            linedata.append((word, tag))
        data.append(linedata)
    return data
