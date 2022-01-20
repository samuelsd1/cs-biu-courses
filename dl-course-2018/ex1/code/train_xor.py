import numpy as np
import mlp1 as mlp
import random
import timeit
STUDENT={'name': 'David Samuelson',
         'ID': '208788851'}

ds_external_lr = False

def feats_to_vec(features):
    return np.array(features)

def accuracy_on_dataset(dataset, params):
    good = bad = 0.0
    for label, features in dataset:
        # YOUR CODE HERE
        # Compute the accuracy (a scalar) of the current parameters
        # on the dataset.
        # accuracy is (correct_predictions / all_predictions)
        x = feats_to_vec(features)  # convert features to a vector.
        y = label
        pred = mlp.predict(x,params)
        if pred == y:
            good += 1
        else:
            bad += 1

    return good / (good + bad)

def train_classifier(train_data, dev_data, learning_rate, params):
    """
    Create and train a classifier, and return the parameters.

    train_data: a list of (label, feature) pairs.
    dev_data  : a list of (label, feature) pairs.
    num_iterations: the maximal number of training iterations.
    learning_rate: the learning rate to use.
    params: list of parameters (initial values)
    """
    dev_accuracy = 0.0
    I = 0
    while dev_accuracy != 1.0:
        I += 1
        cum_loss = 0.0 # total loss in this iteration.
        start_time = timeit.default_timer()
        random.shuffle(train_data)
        for label, features in train_data:
            x = feats_to_vec(features) # convert features to a vector.
            y = label       # convert the label to number if needed.
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

        if I >= 100:
            break

    return params,I


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
        learning_rate = 0.3

    in_dim = 2
    hid_dim = 10
    out_dim = 2

    data = [(1, [0, 0]),
            (0, [0, 1]),
            (0, [1, 0]),
            (1, [1, 1])]

    print 'Rate:',learning_rate
    print 'I,loss,train_acc,dev_acc,elapsed'


    num_iter = 4
    arr = []
    min = 100
    minrate = 100
    while min > 12:
        for learning_rate in np.arange(0.02,2,0.01):
            avg_iter = 0
            for _ in range(0,num_iter):
                params = mlp.create_classifier(in_dim,hid_dim, out_dim)
                trained_params,I = train_classifier(data, data, learning_rate, params)
                avg_iter += I
            avg_iter /= num_iter
            if avg_iter >= 100:
                print 'bad rate',learning_rate
                break
            arr.append((avg_iter,learning_rate))


        for (i,r) in arr:
            if i < min:
                min = i
                minrate = r

    print 'MIN MINRATE',min,minrate