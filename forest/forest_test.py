
import numpy as np
from sklearn.datasets import load_digits

N = 900

digits = load_digits()
training_vectors = digits.images[0:N]
training_vectors = training_vectors.reshape(len(training_vectors), -1)
training_labels = digits.target[0:N]
test_vectors = digits.images[N:]
test_vectors = test_vectors.reshape(len(test_vectors), -1)
test_labels = digits.target[N:]

training_vectors = np.array(training_vectors).astype(np.float64)
training_labels = np.array(training_labels).astype(np.int8)
test_vectors = np.array(test_vectors).astype(np.float64)
test_labels = np.array(test_labels).astype(np.int8)

from forest import RandomForest

n_trees = 2
classifier = RandomForest(n_trees)

import time
t1 = time.time()
classifier = classifier.fit(training_vectors, training_labels)
t2 = time.time()
print("time:{}".format(t2-t1))
results = classifier.predict(test_vectors)

n_corrects = 0
for result, answer in zip(results, test_labels):
    if(result == answer):
        n_corrects += 1
accuracy = float(n_corrects)/len(results)
print("accuracy:{}".format(accuracy))
