import sys
import random
import timeit
import numpy as np
import dynet as dy

from Indexer import Indexer
import utils

class OurNetwork(object):
    def __init__(self, pc, vocab_size, tagset_size, aff_size):
        edim = 50
        wsize = 5
        indim = edim * wsize

        hdim = 100

        self.pW1 = pc.add_parameters((hdim, indim))
        self.pb1 = pc.add_parameters(hdim)

        self.pW2 = pc.add_parameters((tagset_size, hdim))
        self.pb2 = pc.add_parameters(tagset_size)

        self.E = pc.add_lookup_parameters((vocab_size, edim))
        self.Esuff = pc.add_lookup_parameters((aff_size, edim))
        self.Epre = pc.add_lookup_parameters((aff_size, edim))

    def __call__(self, inputs, inputs_pre, inputs_suff):
        W1 = dy.parameter(self.pW1)
        b1 = dy.parameter(self.pb1)
        W2 = dy.parameter(self.pW2)
        b2 = dy.parameter(self.pb2)

        emb_vectors = np.array([self.E[i] for i in inputs])
        emb_vectors_pre = np.array([self.Epre[i] for i in inputs_pre])
        emb_vectors_suff = np.array([self.Esuff[i] for i in inputs_suff])
        emb_sum = emb_vectors + emb_vectors_pre + emb_vectors_suff
        net_input = dy.concatenate(emb_sum.tolist())

        layer1 = ((W1 * net_input) + b1)  # Wx + b
        out_layer = ((W2 * dy.tanh(layer1)) + b2)  # U * tanh(layer1) + bt
        net_output = dy.softmax(out_layer)  # softmax on everything
        return net_output

    def create_network_return_loss(self, inputs, inputs_pre, inputs_suff, expected_output):
        dy.renew_cg()
        out = self(inputs, inputs_pre, inputs_suff)
        loss = -dy.log(dy.pick(out, expected_output))
        return loss

    def create_network_return_best(self, inputs, inputs_pre, inputs_suff):
        dy.renew_cg()
        out = self(inputs, inputs_pre, inputs_suff)
        return np.argmax(out.npvalue())


class Tagger:
    def __init__(self, train_fname, dev_fname):
        train_data, train_wordset, train_tagset = utils.read_labeled_data(train_fname)
        train_wordset.update([utils.START, utils.UNK, utils.END])
        train_tagset.update([utils.START, utils.UNK, utils.END])
        self.train_data = train_data
        self.tag_indexer = Indexer(train_tagset)
        self.word_indexer = Indexer(train_wordset)

        pre = set()
        suff = set()
        for w in train_wordset:
            if w in [utils.START, utils.UNK, utils.END]:
                continue
            if len(w) >= 3:
                pre.add(w[:3])
                suff.add(w[-3:])

        pre.update([utils.START, utils.UNK, utils.END])
        suff.update([utils.START, utils.UNK, utils.END])
        aff_size = len(pre)

        self.pre_indexer = Indexer(pre)
        self.suff_indexer = Indexer(suff)

        self.m = dy.ParameterCollection()
        self.network = OurNetwork(self.m, self.word_indexer.size, self.tag_indexer.size, aff_size)
        self.trainer = dy.AdamTrainer(self.m)

        dev_data, _, _ = utils.read_labeled_data(dev_fname)
        self.dev_data = dev_data

    def window_to_inputs(self, window):
        inputs = []
        inputs_pre = []
        inputs_suff = []

        for w in window:
            if w in [utils.START, utils.UNK, utils.END]:
                word = pre = suff = w
            else:
                word = w if self.word_indexer.contains_key(w) else utils.UNK
                if len(w) >= 3:
                    pre = w[:3]
                    suff = w[-3:]

                    if not self.pre_indexer.contains_key(pre):
                        pre = utils.UNK
                    if not self.suff_indexer.contains_index(suff):
                        suff = utils.UNK
                else:
                    pre = suff = utils.UNK

            inputs.append(self.word_indexer.to_index(word))
            inputs_pre.append(self.pre_indexer.to_index(pre))
            inputs_suff.append(self.suff_indexer.to_index(suff))
        return inputs, inputs_pre, inputs_suff

    def predict(self, word_list):
        padded = utils.pad_list(word_list)
        predictions = []
        for i in xrange(2, len(padded) - 2):
            window = utils.get_window(padded, i)
            inputs, inputs_pre, inputs_suff = self.window_to_inputs(window)

            pred = self.network.create_network_return_best(inputs, inputs_pre, inputs_suff)
            predictions.append(self.tag_indexer.from_index(pred))
        return predictions

    def train(self, num_epochs, verbose=False, report_acc=False):
        for epoch in xrange(num_epochs):
            total_loss = 0.0
            seen_instances = 0
            num_sentences = 0
            random.shuffle(self.train_data)

            epoch_timer = utils.Timer()
            epoch_timer.start('#%d training iteration' % (epoch + 1))
            for wordlist, taglist in self.train_data:
                padded_wordlist = utils.pad_list(wordlist)
                padded_taglist = utils.pad_list(taglist)

                for i in xrange(2, len(padded_wordlist) - 2):
                    t = padded_taglist[i]

                    window = utils.get_window(padded_wordlist, i)
                    inputs, inputs_pre, inputs_suff = self.window_to_inputs(window)
                    lbl = self.tag_indexer.to_index(t)

                    dy.renew_cg()
                    out = self.network(inputs, inputs_pre, inputs_suff)
                    loss = -dy.log(dy.pick(out, lbl))
                    seen_instances += 1
                    total_loss += loss.value()
                    loss.backward()
                    self.trainer.update()

                num_sentences += 1
                if verbose:
                    if num_sentences % 2000 == 0:
                        print 'epoch:', (epoch + 1),
                        print '\tsentence:', num_sentences,
                        print '\tAvg. loss:', total_loss / seen_instances

            elapsed = epoch_timer.stop()
            if verbose:
                print '=' * 60
                print 'epoch: %d,evaluated_sentences: %d,elapsed: %f' % (epoch + 1, num_sentences, elapsed)
                if report_acc:
                    t = utils.Timer(verbose=True)

                    t.start('evaluating train acc')
                    train_acc = self.acc_on_dataset(self.train_data)
                    t.stop()

                    t.start('evaluating dev acc')
                    dev_acc = self.acc_on_dataset(self.dev_data)
                    t.stop()

                    print ''
                    print 'Train Accuracy:\t', train_acc
                    print 'Dev Accuracy:\t', dev_acc
                print '=' * 60

    def acc_on_dataset(self, labeled_data):
        good = bad = 0.0
        for wordlist, taglist in labeled_data:
            prediction = self.predict(wordlist)

            for pred, lbl in zip(prediction, taglist):
                if pred == lbl:
                    good += 1
                else:
                    bad += 1

        return good / (good + bad)

    def tag(self, input_fname, output_fname):
        infile = utils.read_unlabeled_data(input_fname)

        with open(output_fname, 'w') as f:
            for words in infile:
                predictions = self.predict(words)

                s = ''
                for word, pred in zip(words, predictions):
                    s += '%s %s\n' % (word, pred)
                s += '\n'

                f.write(s)


def main(train_fname, dev_fname, test_fname, output_fname):
    t = Tagger(train_fname, dev_fname)
    t.train(5, verbose=True, report_acc=True)
    t.tag(test_fname,output_fname)

if __name__ == '__main__':
    if len(sys.argv) <= 4:
        print 'Usage: <train_filename> <dev_filename> <test_filename> <output_filename>'
        exit()

    main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
