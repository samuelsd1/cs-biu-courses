import numpy as np

STUDENT = {'name': 'David Samuelson',
           'ID': '208788851'}

def create_layer_params(params):
    """
    Create the parameter for all the layers
    """
    return np.array([(params[d : d + 2]) for d in xrange(0,len(params),2)])

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

def classifier_output(x, params):
    W_bs = create_layer_params(params)

    # iterate all (w,b)'s except the last one, there we need to apply softmax
    for (W, b) in W_bs[:-1]:
        x = np.tanh(np.dot(x, W) + b)

    # apply softmax on the last layer
    W, b = W_bs[-1]
    probs = softmax(np.dot(x, W) + b)
    return probs

def predict(x, params):
    return np.argmax(classifier_output(x, params))


def compute_n_layers_probability(x,W_bs,n):
    probs = np.array(x)
    for W, b in W_bs[:n]:
        probs = np.tanh(np.dot(probs, W) + b)
    return probs


def loss_and_gradients(x, y, params):
    # calculate the loss
    probs = classifier_output(x, params)
    loss = -np.log(probs[y])

    # compute the gradients
    grads = []
    W_bs = create_layer_params(params)
    l_count = len(W_bs)  # layers count
    layer_probs = compute_n_layers_probability(x, W_bs, l_count - 1)

    # compute the last layer gradients.

    W, b = W_bs[-1]
    W_bs_except_last = W_bs[:-1]
    W_bs_except_last = W_bs_except_last[::-1]

    der_current = -W[:, y] + np.dot(W, probs)
    der_by_b = np.copy(probs)
    der_by_b[y] -= 1
    der_by_W = np.zeros(W.shape)
    for (i, j) in np.ndindex(der_by_W.shape):
        grad = layer_probs[i] * probs[j]
        if y == j:
            grad -= layer_probs[i]
        der_by_W[i, j] = grad

    # add to the grads in reverse way (at the end we will be reverse back)
    grads.append(der_by_b)
    grads.append(der_by_W)

    l_num = l_count - 2
    for W, b in W_bs_except_last:
        l_in = compute_n_layers_probability(x, W_bs, l_num)
        tanh_grad = 1 - (np.tanh(np.dot(l_in, W) + b)) ** 2
        der_by_b = tanh_grad
        der_by_W = np.dot(l_in.reshape(len(l_in), 1), tanh_grad.reshape(1, len(tanh_grad)))
        grads.append(der_current * der_by_b)
        grads.append(der_current * der_by_W)
        der_current = np.dot(tanh_grad * W, der_current)
        l_num -= 1

    grads.reverse()
    return loss, grads

def xavier_init(in_dim, out_dim):
    """
    I don't really understand what it does exactly, but it creates a
    matrix with dimension in_dim x out_dim, and initializes it with "good"
    values.
    Found a nice article about this, but currently I don't have time to read it.
    https://prateekvjoshi.com/2016/03/29/understanding-xavier-initialization-in-deep-neural-networks/
    But I will read it later, after I submit the assignment :)

    Got more reference about the function from here:
    https://stackoverflow.com/questions/33640581/how-to-do-xavier-initialization-on-tensorflow
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

    Assume a tanh activation function between all the layers.

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


if __name__ == '__main__':
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