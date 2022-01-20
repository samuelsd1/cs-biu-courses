class Indexer:

    def __init__(self, obj_set):
        self._obj_set = sorted(obj_set)
        self._size = len(self._obj_set)
        self.O2I = {o: i for i, o in enumerate(self._obj_set)}
        self.I2O = {v: k for k, v in self.O2I.iteritems()}

    @property
    def size(self):
        return self._size

    @property
    def obj_set(self):
        return set(self._obj_set)

    def contains_key(self, key):
        return key in self.O2I

    def contains_index(self, index):
        return index in self.I2O

    def to_index(self, obj):
        return self.O2I[obj]

    def from_index(self, index):
        return self.I2O[index]

    def save(self, fname):
        with open(fname, 'w') as file:
            l = len(self.O2I)
            i = 0
            for k in self._obj_set:
                i += 1
                str = '%s %d' % (k, self.O2I[k])
                if i < l:
                    str += '\n'
                file.write(str)

    @staticmethod
    def load(fname):
        obj_set = set()
        with open(fname, 'r') as file:
            for line in file:
                words = line.rsplit(' ', 1)
                if len(words) != 2:
                    break
                o, _ = words
                obj_set.add(o)
        return Indexer(obj_set)
