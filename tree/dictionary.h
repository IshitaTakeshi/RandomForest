typedef struct Map {
    int key;
    int value;
} Map;

typedef struct Dictionary {
    int size;
    Map **maps;
} Dictionary;

Dictionary *init_dictionary();
Dictionary *append_dictionary(Dictionary *dictionary, int key, int value);
Dictionary *set_value(Dictionary *dictionary, int key, int value);
int get_value(Dictionary *dictionary, int key);
int *get_keys(Dictionary *dictionary);
int *get_values(Dictionary *dictionary);
int has_key(Dictionary *dictionary, int key);
void free_dictionary(Dictionary *dictionary);
