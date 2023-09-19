
all:
	gcc -o ./run test/test_iris.c tree/classifier.c tree/dataset.c tree/dictionary.c tree/visualizer.c tree/criterions.c tree/node.c random/random.c error/error.c -g -Wall
