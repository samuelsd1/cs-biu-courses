import numpy as np
import dynet as dy


class BasicNetwork(object):
    def __init__(self, pc, vocab_size, emb_vec_size, tagset_size):
        window_size = 5
        in_dim = window_size * emb_vec_size

        hidden_dim = 100

        self.pW1 = pc.add_parameters((hidden_dim, in_dim))
        self.pb1 = pc.add_parameters(hidden_dim)

        self.pW2 = pc.add_parameters((tagset_size, hidden_dim))
        self.pb2 = pc.add_parameters(tagset_size)

        self.E = pc.add_lookup_parameters((vocab_size, emb_vec_size))

    def set_word_emb_vecs(self, emb_vecs):
        self.E.init_from_array(emb_vecs)

    def emb_vecs_from_inputs(self, inputs):
        return [self.E[i] for i in inputs]

    def __call__(self, inputs):
        W1 = dy.parameter(self.pW1)
        b1 = dy.parameter(self.pb1)
        W2 = dy.parameter(self.pW2)
        b2 = dy.parameter(self.pb2)

        emb_vectors = self.emb_vecs_from_inputs(inputs)
        net_input = dy.concatenate(emb_vectors)

        layer1 = ((W1 * net_input) + b1)  # Wx + b
        out_layer = ((W2 * dy.tanh(layer1)) + b2)  # U * tanh(layer1) + bt
        net_output = dy.softmax(out_layer)  # softmax on everything
        return net_output

    def create_network_return_loss(self, inputs, expected_output):
        dy.renew_cg()
        out = self(inputs)
        loss = -dy.log(dy.pick(out, expected_output))
        return loss

    def create_network_return_best(self, inputs):
        dy.renew_cg()
        out = self(inputs)
        return np.argmax(out.npvalue())


class AffixNetwork(BasicNetwork):
    def __init__(self, pc, vocab_size, emb_vec_size, tagset_size, aff_size):
        super(AffixNetwork, self).__init__(pc, vocab_size, emb_vec_size, tagset_size)

        self.Epref = pc.add_lookup_parameters((aff_size, emb_vec_size))
        self.Esuff = pc.add_lookup_parameters((aff_size, emb_vec_size))

    def emb_vecs_from_inputs(self, inputs):
        words, prefs, suffs = inputs
        word_embs = np.array(super(AffixNetwork, self).emb_vecs_from_inputs(words))
        pref_embs = np.array([self.Epref[i] for i in prefs])
        suff_embs = np.array([self.Esuff[i] for i in suffs])
        emb_sum = word_embs + pref_embs + suff_embs
        return emb_sum.tolist()
