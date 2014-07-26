#include "criterions.h"
#include <stdlib.h>

int is_in(Criterions *c, double element) {
    int i;

    for(i=0; i<c->size; i++) {
        if(c->criterions[i] == element) {
            return 1;
        }
    }
    return 0;
}

Criterions *init_criterions(void) {
    Criterions *c;
    c = (Criterions *)malloc(sizeof(Criterions));
    c->size = 0;
    c->criterions = (double *)malloc(c->size*sizeof(double));
    return c;
}

Criterions *append_criterion(Criterions *c, double criterion) {
    c->criterions = (double *)realloc(c->criterions, 
                                      (c->size+1)*sizeof(double));
    c->criterions[c->size] = criterion;
    c->size += 1;
    return c;
}

void free_criterions(Criterions *c) {
    free(c->criterions);
    free(c);
}
