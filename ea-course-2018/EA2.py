import numpy as np
import random
import copy

STUDENT1 = {'name': 'David Samuelson',
           'ID': '208788851'}
STUDENT2 = {'name': 'Saar Shtalryd',
           'ID': '307838854'}
amountOfChromosomes = 50
amountOfSamples = 100
elitismRate = 0.1
dropRate = 0.1
mutateRate = 0.1
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


def loss(x, y, params):
    layers_outputs = all_outputs(x, params)
    W = params[-2]
    b = params[-1]
    probs = softmax(np.dot(layers_outputs[-1], W) + b)
    loss = -np.log(probs[y])

    return loss


def xavier_init(in_dim, out_dim):
    """
    :param in_dim: self explanatory
    :param out_dim: self explanatory
    :return: matrix in shape of (in_dim, out_dim) with good values
    """
    if in_dim == 1 or out_dim == 1:
        if in_dim == 1:
            shape = (out_dim,)
            #return np.random.randn(out_dim) / np.sqrt(out_dim)
        else:
            shape = (in_dim,)
            #return np.random.randn(in_dim) / np.sqrt(in_dim)
    else:
        shape = (in_dim, out_dim)
    dim_sum = in_dim + out_dim
    #return np.random.randn(in_dim, out_dim) / np.sqrt(in_dim)

    return np.random.uniform(
        -np.sqrt(6.0 / (dim_sum)),
        np.sqrt(6.0 / (dim_sum)),
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


def accuracy_on_dataset(dataset, params, indices):
    good = bad = 0.0
    cum_loss = 0.0
    #for x, y in dataset:
    for i in xrange(len(indices)):
        x, y = dataset[indices[i]]
        # Compute the accuracy (a scalar) of the current parameters
        # on the dataset.
        # accuracy is (correct_predictions / all_predictions)
        pred = predict(x, params)
        if pred == y:
            good += 1
        else:
            bad += 1
        currentLoss = loss(x, y, params)
        cum_loss += currentLoss

    avgLoss = cum_loss / len(indices)
    return (good / (good + bad), avgLoss)


def load_data(mnist_pkl):
    import gzip, cPickle
    f = gzip.open(mnist_pkl, 'rb')
    train_set, valid_set, test_set = cPickle.load(f)
    f.close()
    return zip(train_set[0], train_set[1]), zip(valid_set[0], valid_set[1]), zip(test_set[0], test_set[1])

# Creates a breeding array, according to roulette method. The best nets will appear the most times.
def getBreedingArray(population):
    # Drop a percentage of the worst nets for breeding
    breedingPopulation = population[:-int(dropRate*len(population))]
    breedingArray = []
    for i in xrange(len(breedingPopulation)):
        for j in xrange(len(breedingPopulation) - i):
            breedingArray.append(copy.copy(breedingPopulation[i]))
    return breedingArray

# Evolve the population, according to accuracy on a given dataset.
def evolve(population, dataSet):
    elitismIndex = int(elitismRate * len(population))
    newPopulation = []
    indices = random.sample(range(1, len(dataSet)), amountOfSamples)
    # Re-calculate fitness on elitist population, according to the currently chosen indices.
    for chromosome in population[:elitismIndex]:
        fitness = accuracy_on_dataset(dataSet, chromosome[0], indices)
        newPopulation.append((chromosome[0], fitness))
    breedingArray = getBreedingArray(population)
    # Sort from lowest loss to greatest loss.
    newPopulation.sort(key=lambda t: t[1][1])

    # Breed best net with a random net. We assure the best net gets bred at least once every generation
    best = newPopulation[0][0]
    parentForElitismCrossOver = random.choice(population)[0]
    child = breed(best, parentForElitismCrossOver)
    fitness = accuracy_on_dataset(dataSet, child, indices)
    newPopulation.append((child, fitness))

    while len(newPopulation) < amountOfChromosomes:
        mother = random.choice(breedingArray)[0]
        father = random.choice(breedingArray)[0]
        child = breed(mother, father)
        fitness = accuracy_on_dataset(dataSet, child, indices)
        newPopulation.append((child, fitness))
    # Sort from lowest loss to greatest loss.
    newPopulation.sort(key=lambda t: t[1][1])
    return newPopulation

def breed(mother, father):
    # Sanity check - assure both nets have the same amount of parameters.
    if len(mother) != len(father):
        raise AttributeError('Length of mother and father should be equal')
    child = []
    for paramIndex in xrange(len(mother)):

        motherParam = mother[paramIndex]
        fatherParam = father[paramIndex]
        # Sanity check - assure both parameters are of the same shape.
        if motherParam.shape != fatherParam.shape:
            raise AttributeError('Shape of params on mother and father should be equal')

        # If the parameter is a vector, simply choose randomly from parents and copy to child.
        if len(motherParam.shape) == 1:
            childParam = random.choice([motherParam, fatherParam])
        # If the parameter is a matrix, choose columns randomly from parents.
        # We choose columns and not rows, because we multiply xW and not Wx when calculating prediction.
        else:
            childParam = np.zeros(motherParam.shape)
            for i in xrange(motherParam.shape[1]):
                # random.random is needed here, cannot choose columns with random.choice.
                if random.random() < 0.5:
                    childParam[:, i] += motherParam[:, i]
                else:
                    childParam[:, i] += fatherParam[:, i]
        # Sanity check - assure both parameters are of the same shape.
        if motherParam.shape != childParam.shape:
            raise AttributeError('Shape of mother param and child param should be equal')
        # Append parameter to child
        child.append(childParam)
    if random.random() < mutateRate:
        mutationSizeVar = random.random()
        # Add normally distributed noise to all parameters
        for i in xrange(len(child)):
            if mutationSizeVar < 0.3:
                mutation = np.random.normal(scale=0.0001, size=child[i].shape)
            elif mutationSizeVar < 0.6:
                mutation = np.random.normal(scale=0.001, size=child[i].shape)
            else:
                mutation = np.random.normal(scale=0.01, size=child[i].shape)
            child[i] = np.add(child[i], mutation)
    return child

def initializeNetworks(dataSet):
    classifiers = []
    fitnessArr = []
    for i in xrange(amountOfChromosomes):
        classifier = create_classifier([784, 100, 50, 10])
        classifiers.append(classifier)
        indices = random.sample(range(1, len(dataSet)), amountOfSamples)
        fitness = accuracy_on_dataset(dataSet, classifier, indices)
        fitnessArr.append(fitness)

    classifiersAndAccuracies = zip(classifiers, fitnessArr)
    classifiersAndAccuracies.sort(key=lambda t: t[1][1])
    return classifiersAndAccuracies

def getDatasetPredictionsAsString(params, dataset, datasetName):
    good = bad = 0.0
    predictions = ''
    for x, y in dataset:
        # Compute the accuracy (a scalar) of the current parameters
        # on the dataset.
        # accuracy is (correct_predictions / all_predictions)
        pred = predict(x, params)
        if pred == y:
            good += 1
        else:
            bad += 1
        predictions += str(pred)
        predictions += '\n'
    print 'Accuracy on ' + datasetName + ' dataset: '
    print good / (good + bad)
    return predictions[:-1]

def main():
    TRAIN, DEV, TEST = load_data('data/mnist.pkl.gz')
    population = initializeNetworks(TRAIN)
    for i in xrange(10000):
        population = evolve(population, TRAIN)
        # Print loss every 50 generations
        if i % 50 == 0:
            print 'i: ' + str(i)
            print 'Loss on TRAIN: ' + str(population[0][1])
            print 'Loss on DEV: ' + str(accuracy_on_dataset(DEV, population[0][0], range(len(DEV) - 1)))

    output = open('./test.pred', 'w+')
    # population[0] is the best net, index 0 of the best net is the net itself, while index 1 is the fitness.
    output.write(getDatasetPredictionsAsString(population[0][0], TEST, 'test'))
    output.close()

main()