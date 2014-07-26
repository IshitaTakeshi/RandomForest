import numpy as np

import copy
import tree

class RandomForest(object):
    def __init__(self, n_trees, leaf_size=1, n_trials=1):
        self.n_trees = n_trees
        self.leaf_size = leaf_size
        self.n_trials = n_trials
        self.trees = []

    def fit(self, X, y):
        self.trees = []
        
        for _ in range(self.n_trees):
            classifier = tree.DecisionTree(self.leaf_size, self.n_trials)
            self.trees.append(classifier)

        for i, classifier in enumerate(self.trees):
            classifier = classifier.fit(X, y)
            self.trees[i] = classifier 
        return self

    def show(self, index):
        assert index < self.n_trees, "An index must be smaller than n_trees"
        self.trees[index].show()

    def predict(self, X):
        results = np.zeros(len(X)).astype(np.int64)
        for classifier in self.trees:
            results += classifier.predict(X)
        results = results.astype(np.float64)/len(self.trees)
        return np.round(results).astype(np.int64)
