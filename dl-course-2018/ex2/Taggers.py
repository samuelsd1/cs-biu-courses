import random

from Indexer import Indexer

# using the special tags from the given vocabulary
# for all the taggers (1, 2 and 3)
START = '<s>'
END = '</s>'
UNK = 'UUUNKKK'


def pad_list(l):
    padded = [START, START]
    padded.extend(l)
    padded.extend([END, END])
    return padded


def get_window(padded_list, i):
    return [padded_list[idx] for idx in range(i - 2, i + 3)]


class Tagger(object):
    def __init__(self, tagset, vocab, network, trainer):
        self.tag_indexer = Indexer(tagset.union([START, UNK, END]))
        self.network = network
        self.trainer = trainer

        # safety check
        for t in [START, END, UNK]:
            _ = vocab[t]

        self.vocab = vocab

    def predict_tags(self, word_list):
        padded = pad_list(word_list)
        predictions = []
        for i in xrange(2, len(padded) - 2):
            window = get_window(padded, i)
            inputs = self.window_to_inputs(window)

            pred = self.network.create_network_return_best(inputs)
            predictions.append(self.tag_indexer.from_index(pred))
        return predictions

    def window_to_inputs(self, window):
        inputs = []
        # if a word is unknown - treat it as UNKNOWN
        for word in window:
            if word in self.vocab:
                inputs.append(self.vocab[word])
            else:
                inputs.append(self.vocab[UNK])

        return inputs

    def train_iter(self, train_data, print_each=0):
        seen_instances = 0
        total_loss = 0.0
        num_sentences = 0

        losses = []

        random.shuffle(train_data)
        for word_list, tag_list in train_data:
            padded_word_list = pad_list(word_list)
            padded_tag_list = pad_list(tag_list)

            for i in xrange(2, len(padded_word_list) - 2):
                t = padded_tag_list[i]

                window = get_window(padded_word_list, i)
                inputs = self.window_to_inputs(window)
                lbl = self.tag_indexer.to_index(t)

                loss = self.network.create_network_return_loss(inputs, lbl)

                seen_instances += 1
                total_loss += loss.value()
                loss.backward()
                self.trainer.update()

            avg_loss = total_loss / seen_instances
            losses.append(avg_loss)
            num_sentences += 1
            if print_each > 0:
                if num_sentences % print_each == 0:
                    print 'sentence: %d\tavg.loss: %f' % (num_sentences, avg_loss)

        return losses

    def acc_on_dataset(self, labeled_data):
        good = bad = 0.0
        for word_list, tag_list in labeled_data:
            prediction = self.predict_tags(word_list)

            for pred, lbl in zip(prediction, tag_list):
                if pred == lbl:
                    good += 1
                else:
                    bad += 1

        return good / (good + bad)


class AffixTagger(Tagger):
    def __init__(self, tagset, vocab, pref_vocab, suff_vocab, network, trainer):
        super(AffixTagger, self).__init__(tagset, vocab, network, trainer)

        # safety check
        for t in [START, END, UNK]:
            _ = pref_vocab[t]
            _ = suff_vocab[t]

        self.pref_vocab = pref_vocab
        self.suff_vocab = suff_vocab

    def window_to_inputs(self, window):
        word_inputs = super(AffixTagger, self).window_to_inputs(window)

        pref_inputs = []
        suff_inputs = []
        for w in window:
            if w in [START, UNK, END]:
                pre = suff = w
            else:
                if len(w) >= 3:
                    pre = w[:3]
                    suff = w[-3:]
                else:
                    pre = suff = UNK

            pref_inputs.append(self.pref_vocab.get(pre, self.pref_vocab[UNK]))
            suff_inputs.append(self.suff_vocab.get(suff, self.suff_vocab[UNK]))

        return word_inputs, pref_inputs, suff_inputs
