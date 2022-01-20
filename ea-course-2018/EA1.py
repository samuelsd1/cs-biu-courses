import numpy as np
import random
import timeit

STUDENT = {'name': 'David Samuelson',
           'ID': '208788851'}


def softmax(x):
    """
    Compute the softmax vector.
    x: a n-dim vector (numpy array)
    returns: an n-dim vector (numpy array) of softmax values
    """
    c = np.amax(x)
    shiftx = np.subtract(x, c)
    exps = np.exp(shiftx)
    exps = np.divide(exps, np.sum(exps))
    return exps


def activation(x):
    # return np.tanh(x) # tanh
    return np.maximum(x, 0) # ReLU


def activation_der(x):
    # return 1 - x ** 2  # tanh
    return np.greater(x, 0).astype(int) # ReLU


def classifier_output(x, params):
    # iterate all (w,b)'s except the last one, there we need to apply softmax
    for i in xrange(0, len(params) - 2, 2):
        W = params[i]
        b = params[i + 1]
        x = activation(np.dot(x, W) + b)

    # apply softmax on the last layer
    W = params[-2]
    b = params[-1]
    probs = softmax(np.dot(x, W) + b)
    return probs


def predict(x, params):
    return np.argmax(classifier_output(x, params))


def all_outputs(x, params):
    probs = np.array(x)
    list = [probs]
    for i in xrange(0, len(params) - 2, 2):
        W = params[i]
        b = params[i + 1]
        probs = activation(np.dot(probs, W) + b)
        list.append(probs)
    return list


def loss_and_gradients(x, y, params):
    layers_outputs = all_outputs(x, params)
    W = params[-2]
    b = params[-1]
    probs = softmax(np.dot(layers_outputs[-1], W) + b)
    loss = -np.log(probs[y])

    lastLayerDer = np.dot(W, probs) - W[:, y]
    lastLayerOutput = layers_outputs[-1]
    gW = np.outer(lastLayerOutput, probs)
    gW[:, y] -= lastLayerOutput

    gb = probs
    gb[y] -= 1
    grads = [gW, gb]

    for i in xrange(len(params) / 2 - 2, -1, -1):
        W = params[i * 2]
        l_in = layers_outputs[i]
        nonlin_der = activation_der(lastLayerOutput)
        gW = np.outer(l_in, nonlin_der)
        gb = nonlin_der
        grads.insert(0, lastLayerDer * gb)
        grads.insert(0, lastLayerDer * gW)
        if i > 0:
            lastLayerOutput = l_in
            lastLayerDer = np.dot(nonlin_der * W, lastLayerDer)

    return loss, grads


def xavier_init(in_dim, out_dim):
    """
    :param in_dim: self explanatory
    :param out_dim: self explanatory
    :return: matrix in shape of (in_dim, out_dim) with good values
    """
    if in_dim == 1 or out_dim == 1:
        if in_dim == 1:
            shape = (out_dim,)
        else:
            shape = (in_dim,)
    else:
        shape = (in_dim, out_dim)
    dim_sum = in_dim + out_dim
    return np.random.uniform(
        -np.sqrt(6.0 / dim_sum),
        np.sqrt(6.0 / dim_sum),
        shape)


def create_classifier(dims):
    """
    returns the parameters for a multi-layer perceptron with an arbitrary number
    of hidden layers.
    dims is a list of length at least 2, where the first item is the input
    dimension, the last item is the output dimension, and the ones in between
    are the hidden layers.
    For example, for:
        dims = [300, 20, 30, 40, 5]
    We will have input of 300 dimension, a hidden layer of 20 dimension, passed
    to a layer of 30 dimensions, passed to learn of 40 dimensions, and finally
    an output of 5 dimensions.
    return:
    a list of parameters where the first two elements are the W and b from input
    to first layer, then the second two are the matrix and vector from first to
    second layer, and so on.
    """
    params = []
    for i in xrange(len(dims) - 1):
        in_dim = dims[i]
        out_dim = dims[i + 1]
        params.append(xavier_init(in_dim, out_dim))
        params.append(xavier_init(1, out_dim))
    return params


def accuracy_on_dataset(dataset, params):
    good = bad = 0.0
    for x, y in dataset:
        # Compute the accuracy (a scalar) of the current parameters
        # on the dataset.
        # accuracy is (correct_predictions / all_predictions)
        pred = predict(x, params)
        if pred == y:
            good += 1
        else:
            bad += 1

    return good / (good + bad)


def train_epoch(train_data, dev_data, learning_rate, params):
    start_time = timeit.default_timer()
    cum_loss = 0.0  # total loss in this iteration.
    random.shuffle(train_data)
    for x, y in train_data:
        loss, grads = loss_and_gradients(x, y, params)
        cum_loss += loss

        # SGD update parameters
        for i in range(0, len(params)):
            params[i] -= learning_rate * grads[i]

    # notify progress
    elapsed = timeit.default_timer() - start_time
    train_loss = cum_loss / len(train_data)
    train_accuracy = accuracy_on_dataset(train_data, params)
    dev_accuracy = accuracy_on_dataset(dev_data, params)
    return params, (train_loss, train_accuracy, dev_accuracy, elapsed)


def load_data(mnist_pkl):
    import gzip, cPickle
    f = gzip.open(mnist_pkl, 'rb')
    train_set, valid_set, test_set = cPickle.load(f)
    f.close()
    return zip(train_set[0], train_set[1]), zip(valid_set[0], valid_set[1]), zip(test_set[0], test_set[1])


def check_gradients():
    # Sanity checks. If these fail, your gradient calculation is definitely wrong.
    # If they pass, it is likely, but not certainly, correct.
    from grad_check import gradient_check

    dims = [5, 4, 7, 3]
    params = create_classifier(dims)

    def _loss_and_p_grad(p):
        """
        General function - return loss and the gradients with respect to parameter p
        """
        params_to_send = np.copy(params)
        par_num = 0
        for i in range(len(params)):
            if p.shape == params[i].shape:
                params_to_send[i] = p
                par_num = i

        loss, grads = loss_and_gradients(range(dims[0]), 0, params_to_send)
        return loss, grads[par_num]

    for _ in xrange(10):
        my_params = create_classifier(dims)
        for p in my_params:
            gradient_check(_loss_and_p_grad, p)


def main():
    TRAIN, DEV, TEST = load_data('data/mnist.pkl.gz')
    params = create_classifier([784, 100, 10])
    num_iterations = 30
    learning_rate_start = 10e-3
    learning_rate_end = 10e-6
    learning_rate_factor = (learning_rate_start - learning_rate_end) / num_iterations
    learning_rate = learning_rate_start
    for I in xrange(num_iterations):
        trained_params, details = train_epoch(TRAIN, DEV, learning_rate, params)
        learning_rate -= learning_rate_factor
        loss, train_accuracy, dev_accuracy, elapsed = details
        print I, loss, train_accuracy, dev_accuracy, elapsed


if __name__ == '__main__':
    # check_gradients()
    main()
