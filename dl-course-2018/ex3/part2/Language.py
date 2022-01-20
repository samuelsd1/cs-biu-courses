import random
import sys


class Language(object):

    @property
    def vocab(self):
        raise NotImplementedError()

    @property
    def max_word_len(self):
        raise NotImplementedError()

    def good_sample(self):
        raise NotImplementedError()

    def bad_sample(self):
        raise NotImplementedError()


def generate_train(lang, nsamples):
    samples = []
    for _ in xrange(nsamples):
        label = random.randint(0, 1)
        if label == 1:
            sample = lang.good_sample()
        else:
            sample = lang.bad_sample()
        samples.append((label, sample))
    return samples


class Part1(Language):
    def __init__(self, maxlen):
        self._maxlen = maxlen

    def _generate_sample(self, order):
        MAXLEN = self.max_word_len / 2
        sample = ''
        for c in order:
            if c == 'n':
                sample += ''.join(str(random.choice(range(10))) for _ in xrange(random.randint(1, MAXLEN)))
            else:
                sample += random.randint(1, MAXLEN) * c
        return sample

    @property
    def vocab(self):
        return set('0123456789abcd')

    @property
    def max_word_len(self):
        return self._maxlen

    def good_sample(self):
        return self._generate_sample('nanbncnd')

    def bad_sample(self):
        return self._generate_sample('nancnbnd')


class Even(Language):
    @property
    def vocab(self):
        return set('0123456789')

    @property
    def max_word_len(self):
        return len(str(sys.maxint))

    def good_sample(self):
        num = random.randint(0, sys.maxint)
        if num % 2 == 1:
            num -= 1
        return str(num)

    def bad_sample(self):
        num = random.randint(0, sys.maxint - 1)
        if num % 2 == 0:
            num += 1
        return str(num)


class Odd(Language):
    @property
    def vocab(self):
        return set('0123456789')

    @property
    def max_word_len(self):
        return len(str(sys.maxint))

    def good_sample(self):
        num = random.randint(0, sys.maxint - 1)
        if num % 2 == 0:
            num += 1
        return str(num)

    def bad_sample(self):
        num = random.randint(0, sys.maxint)
        if num % 2 == 1:
            num -= 1
        return str(num)


class anbn(Language):
    def __init__(self, maxlen):
        self._maxlen = maxlen

    @property
    def vocab(self):
        return set('ab')

    @property
    def max_word_len(self):
        return self._maxlen

    def good_sample(self):
        n = random.randint(1, self.max_word_len / 2)
        return 'a' * n + 'b' * n

    def bad_sample(self):
        n = random.randint(0, self.max_word_len)
        notn = random.randint(0, self.max_word_len - n)
        while notn == n:
            notn = random.randint(0, self.max_word_len - n)
        return 'a' * n + 'b' * notn


class anbncn(Language):
    def __init__(self, maxlen):
        self._maxlen = maxlen

    @property
    def vocab(self):
        return set('abc')

    @property
    def max_word_len(self):
        return self._maxlen

    def good_sample(self):
        n = random.randint(1, self.max_word_len / 3)
        return 'a' * n + 'b' * n + 'c' * n

    def bad_sample(self):
        a = random.randint(0, self.max_word_len)
        b = random.randint(0, self.max_word_len - a)
        while b == a:
            b = random.randint(0, self.max_word_len - a)
        c = random.randint(0, self.max_word_len - a - b)
        return 'a' * a + 'b' * b + 'c' * c


class Prime(Language):
    def __init__(self, n):
        prime = [True for _ in xrange(n + 1)]
        p = 2
        while (p * p <= n):

            # If prime[p] is not changed, then it is a prime
            if prime[p]:
                # Update all multiples of p
                for i in xrange(p * 2, n + 1, p):
                    prime[i] = False
            p += 1

        self.primes = []
        for i in xrange(n + 1):
            if prime[i]:
                self.primes.append(i)
        self.maxprime = n

    @property
    def vocab(self):
        return set('0123456789')

    @property
    def max_word_len(self):
        return len(str(self.maxprime))

    def good_sample(self):
        return str(random.choice(self.primes))

    def bad_sample(self):
        n = random.randint(0, self.maxprime)
        while n in self.primes:
            n = random.randint(0, self.maxprime)
        return str(n)


class PrimeBinary(Prime):
    @property
    def vocab(self):
        return set('01')

    @property
    def max_word_len(self):
        return len('{0:b}'.format(self.maxprime))

    def good_sample(self):
        return '{0:b}'.format(random.choice(self.primes))

    def bad_sample(self):
        n = random.randint(0, self.maxprime)
        while n in self.primes:
            n = random.randint(0, self.maxprime)
        return '{0:b}'.format(n)
