from tree import DecisionTree
from iris_dataset import vectors, labels

N = int(len(vectors)*0.8)
training_vectors = vectors[:N]
training_labels = labels[:N]
test_vectors = vectors[N:]
test_labels = labels[N:]

tree = DecisionTree(leaf_size=1, n_trials=1)
tree.fit(training_vectors, training_labels)
results = tree.predict(test_vectors)
tree.show()

print("results:{}".format(results))
print("answers:{}".format(test_labels))
