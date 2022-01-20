class Feature(object):
    def __call__(self, word, context):
        raise NotImplementedError()


class Feat1(Feature):
    def __call__(self, word, context):
        # context should be the sentence.
        pass


class Feat2(Feature):
    def __call__(self, word, context):
        # context should be the window
        pass


class Feat3(Feature):
    def __call__(self, word, context):
        # context should be the complicated thing.
        pass
