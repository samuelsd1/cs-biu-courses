from sklearn.datasets import load_svmlight_file as lsf
from sklearn.linear_model import LogisticRegression as LogRegress
from pickle import dump as dmp
import sys



def main(fvec_fname, train_fname):

    X_train, Y_train = lsf(fvec_fname)
    log_regress = LogRegress(verbose=True)
    training = log_regress.fit(X_train, Y_train)
    dmp(training, open(train_fname, 'wb'))

if __name__ == '__main__':
    if len(sys.argv) <= 2:
        print 'Usage: TrainSolver <feature_vector_fname> <train_file>'
        exit()

    main(sys.argv[1],sys.argv[2])