#ifndef NODE_H 
#define NODE_H

typedef struct Node {
    struct Node *left;
    struct Node *right;
    int label;
    int isleaf;
    int key;
    double criterion;
} Node;

#endif /* NODE_H */
