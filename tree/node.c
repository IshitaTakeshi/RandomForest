#include <stdlib.h>
#include "node.h"

Node *construct_node(Node *left, Node *right, int label, int isleaf, 
                     int key, double criterion) {
    Node *node;
    node = (Node *)malloc(sizeof(Node));
    node->left = left;
    node->right = right;
    node->label = label;
    node->isleaf = isleaf;
    node->key = key;
    node->criterion = criterion;
    return node;
}
