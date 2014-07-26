cdef extern from "../tree/node.h":
    ctypedef struct Node:
        pass

cdef extern from "../tree/classifier.h": 
    void init_classifier(int n_trials, int leaf_size, int randomized)
    Node *fit(double **vectors, int *labels, int n_vectors, int n_dim)
    int *predict(Node *tree, double **vectors, int n_vectors)
    void free_tree(Node *tree)

cdef extern from "../tree/visualizer.h":
    void show_tree(Node *node)
