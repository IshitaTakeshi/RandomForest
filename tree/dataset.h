#ifndef DATASET_H 
#define DATASET_H
 
typedef struct Data {
    int label;
    double *vector;
} Data;


typedef struct Dataset {
    int size;
    Data **array;
} Dataset;


typedef struct TestDataset {
    int size;
    double **vectors;
} TestDataset;


typedef struct DatasetPair {
    Dataset *left;
    Dataset *right;
} DatasetPair;


Data* init_data(int label, double *vector);
Dataset* init_dataset();
Dataset *append_dataset(Dataset *dataset, Data *data);
Dataset *generate_dataset(double **vectors, int *labels, int n_vectors);
void free_dataset(Dataset *dataset);

TestDataset *init_test_dataset();
TestDataset *generate_test_dataset(double **vectors, int n_vectors);
void free_test_dataset(TestDataset *test_dataset);

DatasetPair *init_dataset_pair();
void free_dataset_pair(DatasetPair *dataset_pair);
#endif /* DATASET_H */
