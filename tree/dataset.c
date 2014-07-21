#include <stdlib.h>
#include "dataset.h"
#include <stdlib.h>

Data* init_data(int label, double *vector) {
    Data *data;        
    data = (Data *)malloc(sizeof(Data));
    data->label = label;
    data->vector = vector;
    return data;
}

Dataset* init_dataset(void) {
    Dataset *dataset;
    dataset = (Dataset *)malloc(sizeof(Dataset));
    dataset->size = 0;
    dataset->array = (Data **)malloc(dataset->size*sizeof(Data *));
    return dataset;
}

Dataset *append_dataset(Dataset *dataset, Data *data) {
    dataset->array = (Data **)realloc(dataset->array, 
                                      (dataset->size+1)*sizeof(Data *));
    dataset->array[dataset->size] = data;
    dataset->size += 1;
    return dataset;
}

Dataset *generate_dataset(double *vectors[], int *labels, int n_vectors) {
    Dataset *dataset;
    Data *data;
    int i;
    
    dataset = init_dataset();
    for(i=0; i<n_vectors; i++) {
        data = init_data(labels[i], vectors[i]);
        dataset = append_dataset(dataset, data);
    }

    return dataset;
}

void free_dataset(Dataset *dataset) {
    int i;
    for(i=0; i<dataset->size; i++) {
        free(dataset->array[i]);
    }
    free(dataset);
}

TestDataset *init_test_dataset(void) {
    TestDataset *test_dataset;
    test_dataset = (TestDataset *)malloc(sizeof(TestDataset));
    test_dataset->size = 0;
    test_dataset->vectors = (double **)malloc(test_dataset->size*sizeof(double *));
    return test_dataset;
}

TestDataset *generate_test_dataset(double *vectors[], int n_vectors) {
    TestDataset *test_dataset; 
    test_dataset = init_test_dataset();
    test_dataset->vectors = vectors;
    test_dataset->size = n_vectors;
    return test_dataset;
}

void free_test_dataset(TestDataset *test_dataset) {
    int i;
    for(i=0; i<test_dataset->size; i++) {
        //free(test_dataset->vectors[i]);
    }
    free(test_dataset);
}

DatasetPair *init_dataset_pair(void) {
    DatasetPair *dataset_pair;
    dataset_pair = (DatasetPair *)malloc(sizeof(DatasetPair));
    return dataset_pair;
}

void free_dataset_pair(DatasetPair *dataset_pair) {
    free(dataset_pair->left);
    free(dataset_pair->right);
}
