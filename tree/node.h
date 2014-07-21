typedef struct Node {
    struct Node *left;
    struct Node *right;
    int label;
    int isleaf;
    int key;
    double criterion;
} Node;
