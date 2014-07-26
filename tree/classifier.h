#include "dataset.h"
#include "node.h"

void init_classifier(int n_trials, int leaf_size, int randomized);
Node *fit(double **vectors, int *labels, int n_vectors, int n_dim);
int *predict(Node *tree, double **vectors, int n_vectors);
double measure_accuracy(Node *tree, double **vectors, int *labels, int n_vectors);
void free_tree(Node *tree);
