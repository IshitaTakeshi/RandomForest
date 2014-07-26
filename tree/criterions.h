typedef struct Criterions {
    int size;
    double *criterions;
} Criterions;

int is_in(Criterions *c, double element);
Criterions *init_criterions(void);
Criterions *append_criterion(Criterions *c, double criterion);
void free_criterions(Criterions *c);
