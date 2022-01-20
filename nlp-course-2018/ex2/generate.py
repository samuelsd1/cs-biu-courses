"""Usage:   generate.py FILE [-n num] [-t]

-h --help       show this
-n num          number [default: 1]
-t              Tree [default: False]

"""
from docopt import docopt

from collections import defaultdict
import random


class PCFG(object):
    def __init__(self, tree=False):
        self._rules = defaultdict(list)
        self._sums = defaultdict(float)
        self._tree = tree

    def add_rule(self, lhs, rhs, weight):
        assert (isinstance(lhs, str))
        assert (isinstance(rhs, list))
        self._rules[lhs].append((rhs, weight))
        self._sums[lhs] += weight

    @classmethod
    def from_file(cls, filename, tree=False):
        grammar = PCFG(tree)
        with open(filename) as fh:
            for line in fh:
                line = line.split("#")[0].strip()
                if not line: continue
                w, l, r = line.split(None, 2)
                r = r.split()
                w = float(w)
                grammar.add_rule(l, r, w)
        return grammar

    def is_terminal(self, symbol):
        return symbol not in self._rules

    def gen(self, symbol):
        if self.is_terminal(symbol):
            return symbol
        else:
            expansion = self.random_expansion(symbol)
            gen = " ".join(self.gen(s) for s in expansion)
            if self._tree:
                gen = '(' + symbol + ' ' + gen + ')'
            return gen

    def random_sent(self):
        return self.gen("ROOT")

    def random_expansion(self, symbol):
        """
        Generates a random RHS for symbol, in proportion to the weights.
        """
        p = random.random() * self._sums[symbol]
        for r, w in self._rules[symbol]:
            p = p - w
            if p < 0: return r
        return r


if __name__ == '__main__':
    args = docopt(__doc__, argv=None, help=True, version=None, options_first=False)

    filename = args['FILE']
    num = int(args['-n'])
    tree = args['-t']
    for _ in xrange(num):
        pcfg = PCFG.from_file(filename, tree)
        print pcfg.random_sent()
