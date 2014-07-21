
all:
	gcc test/test_iris.c tree/classifier.c tree/dataset.c tree/dictionary.c tree/visualizer.c error/error.c -g -Wall
