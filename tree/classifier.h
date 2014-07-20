#include "dataset.h"

typedef struct Node {
    struct Node *left;
    struct Node *right;
    int label;
    int isleaf;
    int key;
    double criterion;
} Node;


Node *fit(Dataset *dataset, int n_dim, int leaf_size);
int *predict(Node *tree, TestDataset *test_dataset);
int predict_once(Node *node, double *vector);
void free_tree(Node *node);
