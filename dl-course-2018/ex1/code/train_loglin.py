import numpy as np
import loglinear as ll
import random
import utils
import os

STUDENT={'name': 'David Samuelson',
         'ID': '208788851'}

def feats_to_vec(features):
    # YOUR CODE HERE.
    # Should return a numpy vector of features.
    feature_vector = np.zeros(len(utils.vocab))
    used_features_count = 0
    for f in features:
        # if the feature is in the feature vocabulary, then use it.
        if f in utils.vocab:
            feature_vector[utils.F2I[f]] += 1
            used_features_count += 1
    feature_vector = np.divide(feature_vector, used_features_count) # normalize by the used features length
    return feature_vector

def accuracy_on_dataset(dataset, params):
    good = bad = 0.0
    for label, features in dataset:
        # YOUR CODE HERE
        # Compute the accuracy (a scalar) of the current parameters
        # on the dataset.
        # accuracy is (correct_predictions / all_predictions)
        x = feats_to_vec(features)  # convert features to a vector.
        y = utils.L2I[label]        # convert the label to number
        pred = ll.predict(x,params)
        if pred == y:
            good += 1
        else:
            bad += 1

    return good / (good + bad)

def train_classifier(train_data, dev_data, num_iterations, learning_rate, params):
    """
    Create and train a classifier, and return the parameters.

    train_data: a list of (label, feature) pairs.
    dev_data  : a list of (label, feature) pairs.
    num_iterations: the maximal number of training iterations.
    learning_rate: the learning rate to use.
    params: list of parameters (initial values)
    """
    for I in xrange(num_iterations):
        cum_loss = 0.0 # total loss in this iteration.
        random.shuffle(train_data)
        for label, features in train_data:
            x = feats_to_vec(features) # convert features to a vector.
            y = utils.L2I[label]       # convert the label to number if needed.
            loss, grads = ll.loss_and_gradients(x,y,params)
            cum_loss += loss

            # loss and grads output: return loss, [gW, gb]
            # update according to SGD algorithm:
            # params are W,b
            params[0] -= learning_rate*grads[0]
            params[1] -= learning_rate*grads[1]

        train_loss = cum_loss / len(train_data)
        train_accuracy = accuracy_on_dataset(train_data, params)
        dev_accuracy = accuracy_on_dataset(dev_data, params)
        print I, train_loss, train_accuracy, dev_accuracy


        """
        Used later to craft super good test.pred :)
        if dev_accuracy > 0.86:
            dir = "results\\%s_%s_%s_%s" % (str(I), str(dev_accuracy), str(train_accuracy), str(train_loss))
            os.makedirs(dir)
            np.save(dir + '\\Trained_W', params[0])
            np.save(dir + '\\Trained_b', params[1])
            print 'Saved The Trained Parameters to Trained_w.npy and Traind_b.npy.'

            test_pred = predict_on_test(utils.TEST, params)
            f = open(dir + '\\test.pred', 'w')
            for pred in test_pred:
                f.write(utils.I2L[pred[0]])
                f.write('\n')
            f.close()
        """
    return params


def predict_on_test(dataset,params):
    predicts = []
    for _, features in dataset:
        # YOUR CODE HERE
        # Compute the accuracy (a scalar) of the current parameters
        # on the dataset.
        # accuracy is (correct_predictions / all_predictions)
        x = feats_to_vec(features)  # convert features to a vector.
        pred = ll.predict(x, params)
        predicts.append((pred,features))


    return predicts


if __name__ == '__main__':
    # write code to load the train and dev sets, set up whatever you need,
    # and call train_classifier.

    # in dim = size of the vocabulary, which is not exactly size of vocabulary itself, but of datagrams
    # of the vocabulary.

    learning_rate = 0.3
    num_iterations = 28

    in_dim = len(utils.vocab)
    out_dim = len(utils.L2I)
    train_data = utils.TRAIN
    dev_data = utils.DEV

    #print 'Rate:',learning_rate,'iter:',num_iterations
    params = ll.create_classifier(in_dim, out_dim)
    trained_params = train_classifier(train_data, dev_data, num_iterations, learning_rate, params)