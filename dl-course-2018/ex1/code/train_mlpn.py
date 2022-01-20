import mlpn
import random
import utils
import numpy as np

STUDENT = {'name': 'Tamir Moshiashvili',
           'ID': '316131259'}

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
        pred = mlpn.predict(x,params)
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
        cum_loss = 0.0  # total loss in this iteration.
        random.shuffle(train_data)
        for label, features in train_data:
            x = feats_to_vec(features)  # numpy vector.
            y = utils.L2I[label]
            loss, grads = mlpn.loss_and_gradients(x, y, params)
            cum_loss += loss

            # SGD update parameters
            for i in range(0, len(params)):
                params[i] -= learning_rate * grads[i]

        # notify progress
        train_loss = cum_loss / len(train_data)
        train_accuracy = accuracy_on_dataset(train_data, params)
        dev_accuracy = accuracy_on_dataset(dev_data, params)
        print I, train_loss, train_accuracy, dev_accuracy
    return params

if __name__ == '__main__':
    # get params
    learning_rate = 0.4
    num_iterations = 30

    in_dim = len(utils.vocab)
    out_dim = len(utils.L2I)
    train_data = utils.TRAIN
    dev_data = utils.DEV

    print 'Rate:', learning_rate
    print 'I,loss,train_acc,dev_acc,elapsed'

    dims = [in_dim, 30, 20, out_dim]

    params = mlpn.create_classifier(dims)
    trained_params = train_classifier(train_data, dev_data, num_iterations, learning_rate, params)
