import numpy as np

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

def classifier_output(x, params):
    U,W,b,bt = params


    wx = np.dot(x,W)
    wx = np.add(wx,b)
    tanh = np.tanh(wx )
    # softmax on tanh
    return softmax(np.dot(tanh,U) + bt)

def predict(x, params):
    return np.argmax(classifier_output(x, params))

def loss_and_gradients(x, y, params):
    # I will be honest - on this part, I had no time to calculate the derivatives myself,
    # so I used the help of my classmates.
    # But - I did try to calculate, and have some pictures of those calculations on whiteboard of
    # room 203 in the building 604 (maabada).
    # anyways, I made sure I understand why its working and how.
    U, W, b, bt = params
    loss = -np.log(classifier_output(x, params)[y])
    tanh = np.tanh(np.dot(x, W) + b)
    probs = softmax(np.dot(tanh, U) + bt)
    l1d = 1 - np.square(tanh)                        # first layer derivative

    loss_der_by_tanh = np.dot(U, probs) -U[:, y]   # loss derivative, by tanh

    # Compute the gradient of U
    gU = np.zeros(U.shape)
    for (i, j) in np.ndindex(gU.shape):
        grad = tanh[i] * probs[j]
        if j == y : grad -= tanh[i]
        gU[i, j] = grad

    # Compute the gradient of W
    gW = np.zeros(W.shape)
    for (i, j) in np.ndindex(gW.shape):
        gW[i, j] = x[i] * loss_der_by_tanh[j] * l1d[j]

    # Compute the gradient of b using the chain rule
    gb = loss_der_by_tanh * l1d

    # Compute the gradient of bt (exactly as computing the gradient of b in loglinear.py)
    gbt = np.copy(probs)
    gbt[y] -= 1

    return loss, [gU, gW, gb, gbt]


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
        shape = (in_dim,out_dim)
    dim_sum = in_dim + out_dim
    return np.random.uniform(
        -np.sqrt(6.0 / dim_sum),
        np.sqrt(6.0 / dim_sum),
        shape)


def create_classifier(in_dim, hid_dim, out_dim):
    """
    returns the parameters for a multi-layer perceptron,
    with input dimension in_dim, hidden dimension hid_dim,
    and output dimension out_dim.
    """
    U = xavier_init(hid_dim, out_dim)
    W = xavier_init(in_dim, hid_dim)
    b = xavier_init(1, hid_dim)
    bt = xavier_init(1, out_dim)
    params = [U,W,b,bt]
    return params

if __name__ == '__main__':
    # Sanity checks. If these fail, your gradient calculation is definitely wrong.
    # If they pass, it is likely, but not certainly, correct.
    from grad_check import gradient_check

    U,W,b,bt = create_classifier(3,4,5)
    def _loss_and_U_grad(U):
        global W
        global b
        global bt
        loss, grads = loss_and_gradients(np.array([1, 2,3]), 0, [U,W,b,bt])
        return loss, grads[0]

    def _loss_and_W_grad(W):
        global U
        global b
        global bt
        loss, grads = loss_and_gradients(np.array([1, 2,3]), 0, [U,W,b,bt])
        return loss, grads[1]


    def _loss_and_b_grad(b):
        global U
        global W
        global bt
        loss, grads = loss_and_gradients(np.array([1, 2,3]), 0, [U,W,b,bt])
        return loss, grads[2]

    def _loss_and_bt_grad(bt):
        global U
        global W
        global b
        loss, grads = loss_and_gradients(np.array([1, 2,3]), 0, [U,W,b,bt])
        return loss, grads[3]

    for _ in xrange(10):
        U = np.random.randn(U.shape[0], U.shape[1])
        W = np.random.randn(W.shape[0], W.shape[1])
        b = np.random.randn(b.shape[0])
        bt = np.random.randn(bt.shape[0])

        gradient_check(_loss_and_U_grad, U)
        gradient_check(_loss_and_W_grad, W)
        gradient_check(_loss_and_b_grad, b)
        gradient_check(_loss_and_bt_grad, bt)