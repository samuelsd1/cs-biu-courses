import dynet as dy
import numpy as np

"""
In this part I used the DyNet official biLstm tutorial,
found here:
https://github.com/clab/dynet_tutorial_examples/blob/master/tutorial_bilstm_tagger.py
"""

UNK = '_UNK_'
SOE = '<*>'


class AModel(object):
    def __init__(self, model, w2i, t2i):
        word_embed_dim = 128
        lstm_dim = 64
        mlp_dim = 32

        self.w2i = w2i
        self.t2i = t2i

        self.word_lookup = model.add_lookup_parameters((w2i.size, word_embed_dim))

        # MLP on top of biLSTM outputs
        self.pH = model.add_parameters((mlp_dim, word_embed_dim))
        self.pO = model.add_parameters((t2i.size, mlp_dim))

        # layer 1 biLSTM
        self.fwd1 = dy.LSTMBuilder(1, word_embed_dim, lstm_dim, model)
        self.bwd1 = dy.LSTMBuilder(1, word_embed_dim, lstm_dim, model)

        # layer 2 biLSTM
        self.fwd2 = dy.LSTMBuilder(1, word_embed_dim, lstm_dim, model)
        self.bwd2 = dy.LSTMBuilder(1, word_embed_dim, lstm_dim, model)

    def word_rep(self, w):
        w_idx = self.w2i[w]
        return self.word_lookup[w_idx]

    def build_tagging_graph(self, words):
        dy.renew_cg()
        H = dy.parameter(self.pH)
        O = dy.parameter(self.pO)

        # get the word vectors. word_rep(...) returns a 128-dim vector expression for each word.
        wembs = [self.word_rep(w) for w in words]

        # initialize the RNNs
        f1_init = self.fwd1.initial_state()
        b1_init = self.bwd1.initial_state()

        # feed word vectors into biLSTM
        b = []
        xf = wembs
        xb = reversed(wembs)
        fw_exps = f1_init.transduce(xf)
        bw_exps = b1_init.transduce(xb)
        for fw, bw in zip(fw_exps, bw_exps):
            b.append(dy.concatenate([fw, bw]))

        # now we have the vector b
        # feed it through the second layer
        # initialize the RNNs
        f2_init = self.fwd2.initial_state()
        b2_init = self.bwd2.initial_state()

        # feed word vectors into biLSTM
        bt = []
        xf = b
        xb = reversed(b)
        fw_exps = f2_init.transduce(xf)
        bw_exps = b2_init.transduce(xb)
        for fw, bw in zip(fw_exps, bw_exps):
            bt.append(dy.concatenate([fw, bw]))

        # feed each biLSTM state to an MLP
        exps = []
        for x in bt:
            r_t = O * (dy.tanh(H * x))
            exps.append(r_t)

        return exps

    def create_network_return_loss(self, words, tags):
        vecs = self.build_tagging_graph(words)
        errs = []
        for v, t in zip(vecs, tags):
            tid = self.t2i[t]
            err = dy.pickneglogsoftmax(v, tid)
            errs.append(err)
        return dy.esum(errs)

    def create_network_return_best(self, words):
        vecs = self.build_tagging_graph(words)
        vecs = [dy.softmax(v) for v in vecs]
        probs = [v.npvalue() for v in vecs]
        tags = []
        for prb in probs:
            tag = np.argmax(prb)
            tag = self.t2i(tag)
            tags.append(tag)

        return tags


class BModel(AModel):
    def __init__(self, model, w2i, t2i, c2i):
        super(BModel, self).__init__(model, w2i, t2i)

        char_embed_dim = 20
        char_lstm_dim = 128

        self.c2i = c2i

        self.CHARS_LOOKUP = model.add_lookup_parameters((c2i.size, char_embed_dim))

        # char-level LSTMs
        self.charLSTM = dy.LSTMBuilder(1, char_embed_dim, char_lstm_dim, model)

    def word_rep(self, w):
        pad_char = self.c2i[SOE]
        char_ids = [pad_char] + [self.c2i[c] for c in w] + [pad_char]
        char_embs = [self.CHARS_LOOKUP[cid] for cid in char_ids]

        cf_init = self.charLSTM.initial_state()
        exps = cf_init.transduce(char_embs)
        return exps[-1]


# C model - embeddings + Affixes
class CModel(AModel):
    def __init__(self, model, w2i, t2i, p2i, s2i):
        super(CModel, self).__init__(model, w2i, t2i)

        self.p2i = p2i
        self.s2i = s2i

        pref_embed_dim = 128
        suff_embed_dim = 128

        self.pref_lookup = model.add_lookup_parameters((p2i.size, pref_embed_dim))
        self.suff_lookup = model.add_lookup_parameters((s2i.size, suff_embed_dim))

    def word_rep(self, w):
        embs = super(CModel, self).word_rep(w)
        if len(w) < 3:
            return embs

        pref_idx = self.p2i[w[:3]]
        suff_idx = self.s2i[w[-3:]]

        pref_embs = self.pref_lookup[pref_idx]
        suff_embs = self.suff_lookup[suff_idx]

        embs = dy.esum([pref_embs, embs, suff_embs])
        return embs


# D model - both base and B
class DModel(BModel):
    def __init__(self, model, w2i, t2i, c2i):
        super(DModel, self).__init__(model, w2i, t2i, c2i)

        word_embed_dim = 128
        self.pW = model.add_parameters((word_embed_dim, word_embed_dim * 2))
        self.pb = model.add_parameters((word_embed_dim))

    def word_rep(self, w):
        embs_a = AModel.word_rep(self, w)
        embs_b = BModel.word_rep(self, w)

        embs_d = dy.concatenate([embs_a, embs_b])

        # linear stuff
        W = dy.parameter(self.pW)
        b = dy.parameter(self.pb)

        res = ((W * embs_d) + b)
        return res
