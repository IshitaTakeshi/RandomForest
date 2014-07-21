#include <stdio.h>
#include <stdlib.h>
#include "classifier.h"
#include "dictionary.h"

//Returns a dictionary which 
//the key is a label and
//the value is the number of labels in the dataset
Dictionary *count_labels(Dataset *dataset) {
    Dictionary *labels;
    int label, count;
    int i;
 
    labels = init_dictionary();
    for(i=0; i<dataset->size; i++) {
        label = dataset->array[i]->label;
        if(!has_key(labels, label)) {
            labels = set_value(labels, label, 0);
        }

        count = get_value(labels, label);
        labels = set_value(labels, label, count+1);
    }

    return labels;
}

/*
double calc_entropy(dataset) {
    labels = count_labels(dataset)
    
    entropy = 0.0
    for label, count in labels.items():
        p = float(count)/dataset.size
        entropy -= p*math.log(p, 2)
    return entropy
}
*/

double calc_gini(Dataset *dataset) {
    double gini_impurity = 1.0;
    //TODO use register
    double r;
    int *count_array;
    int i;
    Dictionary *labels;
     
    labels = count_labels(dataset);
    count_array = get_values(labels);
    free_dictionary(labels); 

    for(i=0; i<labels->size; i++) {
        r = (double)count_array[i]/(double)dataset->size;
        gini_impurity -= r*r;
    }

    free(count_array);
    return gini_impurity;
}

double calc_randomness(Dataset *dataset) {
    return calc_gini(dataset);
}

double calc_info_gain(Dataset *dataset, DatasetPair *dataset_pair) {
    double p_left = (double)dataset_pair->left->size/(double)dataset->size;
    double p_right = (double)dataset_pair->right->size/(double)dataset->size;
    
    double left = p_left * calc_randomness(dataset_pair->left);
    double right = p_right * calc_randomness(dataset_pair->right);
     
    double randomness = calc_randomness(dataset);
    return randomness-left-right;
}

DatasetPair *divide(Dataset *dataset, int key, double criterion) {
    Dataset *left;
    Dataset *right;
    Data *data;
    int i;
    
    left = init_dataset();
    right = init_dataset();
    for(i=0; i<dataset->size; i++) {
        data = dataset->array[i];
        if(data->vector[key] >= criterion) {
            left = append_dataset(left, data);
        } else {
            right = append_dataset(right, data);
        }
    }

    DatasetPair *dataset_pair;
    dataset_pair = init_dataset_pair();
    dataset_pair->left = left;
    dataset_pair->right = right;
    return dataset_pair;
}

//Returns the most frequent label in the dataset
int find_most_common_label(Dataset *dataset) { 
    Dictionary *labels;
    int *labels_array, *count_array;
    int common_label;
    int max_count = 0;
    int i;

    labels = count_labels(dataset);

    labels_array = get_keys(labels);
    count_array = get_values(labels);

    free_dictionary(labels);

    for(i=0; i<labels->size; i++) {
        if(count_array[i] > max_count) {
            max_count = count_array[i];
            common_label = labels_array[i];
        }
    }
    
    free(labels_array); 
    free(count_array); 
    return common_label;
}

//TODO add is_in and append_double to a library
double is_in(double *array, int array_size, double element) {
    int i;

    for(i=0; i<array_size; i++) {
        if(array[i] == element) {
            return 1;
        }
    }
    return 0;
}

double *append_double(double *array, int array_size, double element) {
    array = (double *)realloc(array, (array_size+1)*sizeof(double));
    array[array_size] = element;
    return array;
}

Node *construct_tree(Dataset *dataset, int n_dim, int leaf_size) {
    int key, best_key;
    double best_info_gain;
    double criterion, best_criterion;
    double *criterions;
    int criterions_size;
    double info_gain;
    DatasetPair *dataset_pair, *best_dataset_pair;
    Data *data; 
    int i, t;
    
    best_info_gain = 0;

    for(key=0; key<n_dim; key++) {
        criterions_size = 0;
        criterions = (double *)malloc(sizeof(double));

        for(i=0; i<dataset->size; i++) {
            data = dataset->array[i];
            t = data->vector[key];
            if(!is_in(criterions, criterions_size, t)) {
                criterions = append_double(criterions, criterions_size, t);
                criterions_size += 1;
            }
        }
        
        for(i=0; i<criterions_size; i++) {
            criterion = criterions[i];
            dataset_pair = divide(dataset, key, criterion);
            info_gain = calc_info_gain(dataset, dataset_pair);
            if(info_gain > best_info_gain &&
               dataset_pair->left->size > 0 &&
               dataset_pair->right->size > 0) {
                best_info_gain = info_gain;
                best_key = key;
                best_criterion = criterion;
                best_dataset_pair = dataset_pair;
            }
        }

        free(criterions);
    }
    
    Node *node;
    node = (Node *)malloc(sizeof(Node));
 
    if(best_info_gain <= 0 || dataset->size <= leaf_size) {
        node->left = NULL;
        node->right = NULL;
        node->isleaf = 1;
        node->label = find_most_common_label(dataset);
        return node;
    }

    node->isleaf = 0;
    node->left = construct_tree(best_dataset_pair->left, n_dim, leaf_size);
    node->right = construct_tree(best_dataset_pair->right, n_dim, leaf_size);
    node->key = best_key;
    node->criterion = best_criterion;
    return node;
}

void free_tree(Node *node) {
    if(node == NULL) {
        return;
    }

    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

Node *fit(double **vectors, int *labels, int n_vectors, int n_dim, int leaf_size) {
    //TODO assert here
    if(leaf_size <= 0) {
        //error();
    }

    Node *tree; 
    Dataset *dataset;    
    dataset = generate_dataset(vectors, labels, n_vectors);
    tree = construct_tree(dataset, n_dim, leaf_size); 
    free_dataset(dataset);

    return tree;
}

int predict_once(Node *node, double *vector) {
    if(node->isleaf) {
        return node->label;
    }
 
    if(vector[node->key] >= node->criterion) {
        return predict_once(node->left, vector);
    } else {
        return predict_once(node->right, vector);
    }
}

int *predict(Node *tree, double **vectors, int n_vectors) {
    int *results;
    int i;
 
    results = (int *)malloc(n_vectors*sizeof(int));

    for(i=0; i<n_vectors; i++) {
        results[i] = predict_once(tree, vectors[i]);
    }

    return results;
}

double measure_accuracy(Node *tree, double **vectors, int *labels, int n_vectors) {
    int result;
    int i;
    
    int n_corrects = 0;
    for(i=0; i<n_vectors; i++) {
        result = predict_once(tree, vectors[i]);
        if(result == labels[i]) {
            n_corrects += 1;
        }
    }
 
    return (double)n_corrects/(double)n_vectors;
}
