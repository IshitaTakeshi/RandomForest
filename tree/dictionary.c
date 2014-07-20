#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"
#include "../error/error.h"

//TODO rewrite args in pointers

Map *init_map(int key, int value) {
    Map *map;
    map = (Map *)malloc(sizeof(Map));
    map->key = key;
    map->value = value;
    return map;
}

Dictionary *init_dictionary() {
    Dictionary *dictionary;
    dictionary = (Dictionary *)malloc(sizeof(Dictionary));
    dictionary->size = 0;
    dictionary->maps = NULL;
    return dictionary;
}

Dictionary *append_dictionary(Dictionary *dictionary, int key, int value) {
    dictionary->maps = (Map **)realloc(dictionary->maps, 
                                      (dictionary->size+1)*sizeof(Map));
    dictionary->maps[dictionary->size] = init_map(key, value);
    dictionary->size += 1;
    return dictionary;
}

Dictionary *set_value(Dictionary *dictionary, int key, int value) {
    int i;

    for(i=0; i<dictionary->size; i++) {
        if(dictionary->maps[i]->key == key) {
            dictionary->maps[i]->value = value;
            return dictionary;
        }
    }
    
    dictionary = append_dictionary(dictionary, key, value);
    return dictionary;
}

int get_value(Dictionary *dictionary, int key) {
    int i;
    for(i=0; i<dictionary->size; i++) {
        if(dictionary->maps[i]->key == key) {
            return dictionary->maps[i]->value;
        }
    }
    
    char message[30]; 
    sprintf(message, "KeyError: %d", key);
    fatal_error(message);
    exit(0);
}

int *get_keys(Dictionary *dictionary) {
    int *keys;
    int i;
    
    keys = (int *)malloc(dictionary->size*sizeof(int));
    for(i=0; i<dictionary->size; i++) {
        keys[i] = dictionary->maps[i]->key;
    }

    return keys;
}

int *get_values(Dictionary *dictionary) {
    int *values;
    int i;

    values = (int *)malloc(dictionary->size*sizeof(int));

    for(i=0; i<dictionary->size; i++) {
        values[i] = dictionary->maps[i]->value; 
    }
    
    return values;
}

int has_key(Dictionary *dictionary, int key) {
    int i;
    for(i=0; i<dictionary->size; i++) {
        if(dictionary->maps[i]->key == key) {
            return 1;
        }
    }
    return 0;
}

void free_dictionary(Dictionary *dictionary) {
    int i;
    for(i=0; i<dictionary->size; i++) {
        free(dictionary->maps[i]);
    }
}
