import timeit

class Tagger:
    def tag_sentence(self, sentence):
        raise NotImplementedError()


def TagFile(input_fname, output_fname, tagger, lower=False,verbose=False):
    with open(input_fname,'r') as file:
        lines = [line.strip() for line in file]

    with open(output_fname, 'w') as file:
        lineno = 0
        for line in lines:
            sentence = line.split(' ')

            if lower:
                sent_to_pred = line.lower()
            else:
                sent_to_pred = line

            lineno += 1

            if verbose:
                shouldPrint = False
                if lineno % 10 == 0:
                    shouldPrint = True

                if shouldPrint:
                    print 'Predicting line %d...' % lineno,
                    start_time = timeit.default_timer()

            result = tagger.tag_sentence(sent_to_pred)
            if verbose:
                if shouldPrint:
                    print 'Done. Elapsed:', timeit.default_timer() - start_time

            str = ''
            for i in range(len(sentence)):
                str += ' %s/%s'%(sentence[i],result[i])

            str = str[1:]
            str += '\n'
            file.write(str)