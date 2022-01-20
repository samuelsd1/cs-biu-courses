class Indexer:

    def __init__(self, obj_set, default=None):
        self._obj_set = sorted(obj_set)
        if default is not None:
            if default not in self._obj_set:
                self._obj_set.append(default)
        self._size = len(self._obj_set)
        self.O2I = {o: i for i, o in enumerate(self._obj_set)}
        self.I2O = {v: k for k, v in self.O2I.iteritems()}
        self.default = default

    def __call__(self, index):
        return self.from_index(index)

    def __getitem__(self, item):
        return self.to_index(item)

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
        if obj not in self.O2I:
            obj = self.default
        return self.O2I[obj]

    def from_index(self, index):
        return self.I2O[index]

    def save(self, fname):
        with open(fname, 'w') as file:
            l = len(self.O2I)
            i = 0
            if self.default is not None:
                file.write('default:%s\n' % self.default)
            for k in self._obj_set:
                i += 1
                str = '%s %d' % (k, self.O2I[k])
                if i < l:
                    str += '\n'
                file.write(str)

    @staticmethod
    def load(fname):
        kv = {}
        default = None
        with open(fname, 'r') as file:
            for line in file:
                line = line.strip()
                if line.startswith('default:'):
                    default = line[8:]
                    continue
                words = line.rsplit(' ', 1)
                if len(words) != 2:
                    break
                o, d = words
                kv[o] = int(d)

        vk = {v: k for k, v in kv.iteritems()}
        i = Indexer([], default)
        i._obj_set = set(vk.viewkeys())
        i.O2I = kv
        i.I2O = vk
        i._size = len(kv)
        return i

