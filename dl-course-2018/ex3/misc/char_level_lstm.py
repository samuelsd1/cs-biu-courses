import dynet as dy
import random
from collections import defaultdict
from itertools import count
import sys


def get_data(filename):
    with open(filename, 'r') as f:
        data = f.read()

    alphabet = set()
    for c in data:
        alphabet.add(c)

    return alphabet, data


vocab, sentence = get_data('wtf')

LAYERS = 2
INPUT_DIM = 512
HIDDEN_DIM = 100

characters = list(vocab)
characters.append("<EOS>")

int2char = list(characters)
char2int = {c: i for i, c in enumerate(characters)}

VOCAB_SIZE = len(characters)
pc = dy.ParameterCollection()

srnn = dy.SimpleRNNBuilder(LAYERS, INPUT_DIM, HIDDEN_DIM, pc)
lstm = dy.LSTMBuilder(LAYERS, INPUT_DIM, HIDDEN_DIM, pc)

params = {}
params["lookup"] = pc.add_lookup_parameters((VOCAB_SIZE, INPUT_DIM))
params["R"] = pc.add_parameters((VOCAB_SIZE, HIDDEN_DIM))
params["bias"] = pc.add_parameters((VOCAB_SIZE))


# return compute loss of RNN for one sentence
def do_one_sentence(rnn, sentence):
    # setup the sentence
    dy.renew_cg()
    s0 = rnn.initial_state()

    R = dy.parameter(params["R"])
    bias = dy.parameter(params["bias"])
    lookup = params["lookup"]
    sentence = ["<EOS>"] + list(sentence) + ["<EOS>"]
    sentence = [char2int[c] for c in sentence]
    s = s0
    loss = []
    for char, next_char in zip(sentence, sentence[1:]):
        s = s.add_input(lookup[char])
        probs = dy.softmax(R * s.output() + bias)
        loss.append(-dy.log(dy.pick(probs, next_char)))
    loss = dy.esum(loss)
    return loss


# generate from model:
def generate(rnn):
    def sample(probs):
        rnd = random.random()
        for i, p in enumerate(probs):
            rnd -= p
            if rnd <= 0: break
        return i

    # setup the sentence
    dy.renew_cg()
    s0 = rnn.initial_state()

    R = dy.parameter(params["R"])
    bias = dy.parameter(params["bias"])
    lookup = params["lookup"]

    s = s0.add_input(lookup[char2int["<EOS>"]])
    out = []
    while True:
        probs = dy.softmax(R * s.output() + bias)
        probs = probs.vec_value()
        next_char = sample(probs)
        out.append(int2char[next_char])
        if out[-1] == "<EOS>": break
        s = s.add_input(lookup[next_char])
    return "".join(out[:-1])  # strip the <EOS>


# train, and generate every 5 samples
def train(rnn, sentence):
    trainer = dy.SimpleSGDTrainer(pc)
    for i in xrange(1000):
        loss = do_one_sentence(rnn, sentence)
        loss_value = loss.value()
        loss.backward()
        trainer.update()
        if i % 10 == 0:
            print loss_value
            if loss_value < 500:
                print generate(rnn)


train(lstm, sentence)
