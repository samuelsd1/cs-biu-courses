import dynet as dy
import numpy as np
import random
from Indexer import Indexer
import matplotlib.pyplot as plt


# acceptor LSTM
class LstmAcceptor(object):
    def __init__(self, in_dim, lstm_dim, out_dim, model):
        self.builder = dy.VanillaLSTMBuilder(1, in_dim, lstm_dim, model)
        self.W = model.add_parameters((out_dim, lstm_dim))

    def __call__(self, sequence):
        lstm = self.builder.initial_state()
        W = self.W.expr()  # convert the parameter into an Expession (add it to graph)
        outputs = lstm.transduce(sequence)
        result = W * outputs[-1]
        return result


class AcceptorDeployer(object):
    def __init__(self, vocab, embed_dim, lstm_dim, out_dim):
        self.m = dy.Model()
        self.trainer = dy.AdamTrainer(self.m)
        self.embeds = self.m.add_lookup_parameters((len(vocab), embed_dim))
        self.acceptor = LstmAcceptor(embed_dim, lstm_dim, out_dim, self.m)
        self.v2i = Indexer(set(vocab))

    def train_iter(self, train_data):
        sum_of_losses = 0.0
        losses = []

        random.shuffle(train_data)
        for label, sample in train_data:
            dy.renew_cg()  # new computation graph
            vecs = [self.embeds[self.v2i[c]] for c in sample]
            preds = self.acceptor(vecs)
            loss = dy.pickneglogsoftmax(preds, label)

            losses.append(loss.value())

            loss.backward()
            self.trainer.update()

        return losses

    def predict(self, word):
        dy.renew_cg()  # new computation graph
        vecs = [self.embeds[self.v2i[c]] for c in word]
        preds = dy.softmax(self.acceptor(vecs))
        vals = preds.npvalue()

        pred = np.argmax(vals)
        # print pred, vals
        return pred

    def acc_on_dataset(self, labeled_data):
        good = bad = 0.0
        for lbl, word in labeled_data:
            pred = self.predict(word)

            if pred == lbl:
                good += 1
            else:
                bad += 1

        return good / (good + bad)


def read_labeled_data(fname):
    samples = []
    with open(fname, 'r') as f:
        for line in f:
            lbl, word = line.strip().split()
            samples.append((int(lbl), word))
    return samples


def read_unlabeled_data(fname):
    with open(fname, 'r') as f:
        samples = [line.strip() for line in f]
    return samples


def main():
    vocab = '0123456789abcd'
    train = read_labeled_data('data/train')
    dev = read_labeled_data('data/dev')
    test = read_unlabeled_data('data/test')

    b = AcceptorDeployer(vocab, 100, 100, 2)

    num_epoch = 10

    losses = []
    for epoch in xrange(num_epoch):
        iter_losses = b.train_iter(train)
        losses.extend(iter_losses)
        dev_acc = b.acc_on_dataset(dev)
        test_acc = b.acc_on_dataset(train)
        print np.average(iter_losses), test_acc, dev_acc

    num_points = 200
    x = np.arange(0., num_epoch, float(num_epoch) / num_points)
    jmp = len(losses) / num_points
    losses = losses[0::jmp]

    plt.plot(x, losses)
    plt.ylabel('loss')
    plt.xlabel('epoch')
    plt.savefig('loss.png')


if __name__ == '__main__':
    main()
