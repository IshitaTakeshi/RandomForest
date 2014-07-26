import numpy as np

from iris_dataset import labels, vectors
from forest import RandomForest

N = int(len(vectors)*0.8)
training_vectors = vectors[:N]
training_labels = labels[:N]
test_vectors = vectors[N:]
test_labels = labels[N:]

import tree

n_trees = 2
classifier = RandomForest(n_trees)
classifier.fit(training_vectors, training_labels)
results = classifier.predict(test_vectors)

print("results:{}".format(results))
print("answers:{}".format(test_labels))
