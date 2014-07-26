import numpy as np

import tree

class RandomForest(object):
    def __init__(self, N_TREES):
        self.N_TREES = N_TREES
        self.trees = []

    def fit(self, X, y):
        self.trees = []
        for i in range(self.N_TREES):
            classifier = tree.DecisionTree()
            classifier = classifier.fit(X, y, leaf_size=8)
            self.trees.append(classifier)
        return self

    def predict(self, X):
        results = np.zeros(len(X)).astype(np.int8)
        for classifier in self.trees:
            results += classifier.predict(X)
        results = results.astype(np.float64)/len(self.trees)
        return np.round(results).astype(np.int8)
