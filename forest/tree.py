import libtree

class DecisionTree(object):
    def __init__(self, leaf_size, n_trials):
        self.leaf_size = leaf_size
        self.tree = libtree.DecisionTree(leaf_size, n_trials)
        self.fit_finished = False

    def fit(self, X, y):
        assert len(X) == len(y), "X and y must be the same size."
        self.tree.fit(X, y, self.leaf_size)
        self.fit_finished = True
        return self
    
    def predict(self, X):
        assert self.fit_finished, "Train before predicting."
        return self.tree.predict(X)
    
    def show(self):
        self.tree.show()
