#include <stdio.h>
#include <stdlib.h>
#include "classifier.h"
#include "dictionary.h"
#include "criterions.h"
#include "node.h"
#include "../random/random.h"
#include "../error/error.h"

int N_TRIALS = 1;
int LEAF_SIZE = 1;
int RANDOMIZED = 0;

void init_classifier(int n_trials, int leaf_size, int randomized) {
    //TODO make error messages fluent
    if(leaf_size <= 0) {
        fatal_error("leaf_size must be larger than 0\n");
        exit(-1);
    }
    
    if(n_trials <= 0) {
        fatal_error("n_trials must be larger than 0\n");
        exit(-1);
    }

    N_TRIALS = n_trials;
    LEAF_SIZE = leaf_size;
    RANDOMIZED = randomized;
    init_random();
}

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

Node *construct_tree(Dataset *dataset, int n_dim) {
    int key, best_key;
    double best_info_gain;
    double criterion, best_criterion;
    double info_gain;
    Criterions *criterions;
    DatasetPair *dataset_pair, *best_dataset_pair;
    Data *data; 
    int i, t;
    
    best_info_gain = 0;

    for(key=0; key<n_dim; key++) {
        criterions = init_criterions();
        for(i=0; i<dataset->size; i++) {
            data = dataset->array[i];
            t = data->vector[key];
            if(!is_in(criterions, t)) {
                criterions = append_criterion(criterions, t);
            }
        }
         
        for(i=0; i<criterions->size; i++) {
            criterion = criterions->criterions[i];
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

        free_criterions(criterions);
    }
    
    if(best_info_gain <= 0 || dataset->size <= LEAF_SIZE) {
        int label = find_most_common_label(dataset);
        return construct_node(NULL, NULL, label, 1, -1, -1);
    }

    Node *left, *right; 
    left = construct_tree(best_dataset_pair->left, n_dim);
    right = construct_tree(best_dataset_pair->right, n_dim);
    return construct_node(left, right, -1, 0, best_key, best_criterion);
}

Node *construct_randomized_tree(Dataset *dataset, int n_dim) {
    int key, best_key;
    double best_info_gain;
    double criterion, best_criterion;
    double info_gain;
    Criterions *criterions;
    DatasetPair *dataset_pair, *best_dataset_pair;
    Data *data; 
    double t;
    int i, r;
    
    best_info_gain = 0;
    
    for(key=0; key<n_dim; key++) {
        criterions = init_criterions();
        for(i=0; i<dataset->size; i++) {
            data = dataset->array[i];
            t = data->vector[key];
            if(!is_in(criterions, t)) {
                criterions = append_criterion(criterions, t);
            }
        }
         
        for(i=0; i<N_TRIALS; i++) {
            r = random_int(criterions->size);
            criterion = criterions->criterions[r];

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
        free_criterions(criterions);
    }
    
    if(best_info_gain <= 0 || dataset->size <= LEAF_SIZE) {
        int label = find_most_common_label(dataset);
        return construct_node(NULL, NULL, label, 1, -1, -1);
    }

    Node *left, *right; 
    left = construct_tree(best_dataset_pair->left, n_dim);
    right = construct_tree(best_dataset_pair->right, n_dim);
    return construct_node(left, right, -1, 0, best_key, best_criterion);
}

void free_tree(Node *node) {
    if(node == NULL) {
        return;
    }

    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

Node *fit(double **vectors, int *labels, int n_vectors, int n_dim) {
    //TODO assert here

    Node *tree; 
    Dataset *dataset;    
    dataset = generate_dataset(vectors, labels, n_vectors);
    
    if(RANDOMIZED) {
        tree = construct_randomized_tree(dataset, n_dim); 
    } else {
        tree = construct_tree(dataset, n_dim);
    }

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
