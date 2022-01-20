import numpy as np
import mlp1 as mlp
import random
import utils
import timeit
STUDENT={'name': 'David Samuelson',
         'ID': '208788851'}

ds_external_lr = False

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
        # Compute the accuracy (a scalar) of the current parameters
        # on the dataset.
        # accuracy is (correct_predictions / all_predictions)
        x = feats_to_vec(features)  # convert features to a vector.
        y = utils.L2I[label]        # convert the label to number
        pred = mlp.predict(x,params)
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
        start_time = timeit.default_timer()
        random.shuffle(train_data)
        for label, features in train_data:
            x = feats_to_vec(features) # convert features to a vector.
            y = utils.L2I[label]       # convert the label to number if needed.
            loss, grads = mlp.loss_and_gradients(x,y,params)
            cum_loss += loss

            # loss and grads output: return loss, [gU, gW, gb, gbt]
            # update according to SGD algorithm:
            # params are U, W, b, bt
            for i in xrange(len(params)):
                params[i] -= learning_rate*grads[i]

        elapsed = timeit.default_timer() - start_time
        train_loss = cum_loss / len(train_data)
        train_accuracy = accuracy_on_dataset(train_data, params)
        dev_accuracy = accuracy_on_dataset(dev_data, params)
        print I, train_loss, train_accuracy, dev_accuracy,elapsed

    return params


if __name__ == '__main__':
    # YOUR CODE HERE
    # write code to load the train and dev sets, set up whatever you need,
    # and call train_classifier.

    # in dim = size of the vocabulary, which is not exactly size of vocabulary itself, but of datagrams
    # of the vocabulary.

    if ds_external_lr:
        import sys
        if len(sys.argv) <= 1:
            print 'Please provide learning rate'
            sys.exit()
        else:
            learning_rate = float(sys.argv[1])
    else:
        learning_rate = 0.4


    num_iterations = 30
    in_dim = len(utils.vocab)
    hid_dim = 50
    out_dim = len(utils.L2I)
    train_data = utils.TRAIN
    dev_data = utils.DEV

    #print 'Rate:',learning_rate
    #print 'I,loss,train_acc,dev_acc,elapsed'
    params = mlp.create_classifier(in_dim,hid_dim, out_dim)
    trained_params = train_classifier(train_data, dev_data, num_iterations, learning_rate, params)
