from tree import DecisionTree
from iris_dataset import vectors, labels

N = int(len(vectors)*0.8)
training_vectors = vectors[:N]
training_labels = labels[:N]
test_vectors = vectors[N:]
test_labels = labels[N:]

import copy
tree = DecisionTree(leaf_size=1, n_trials=1)
for _ in range(20):
    tree.fit(training_vectors, training_labels)
    t = copy.deepcopy(tree)
    print(id(t))
    #tree.show()

results = tree.predict(test_vectors)
print("results:{}".format(results))
print("answers:{}".format(test_labels))
