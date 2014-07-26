import numpy as np
cimport numpy as np
cimport ctree

from libc.stdlib cimport malloc, free

cdef class DecisionTree:
    cdef ctree.Node *tree 
    
    cdef double **generate_c_vectors(self, 
                                   np.ndarray[np.float64_t, ndim=2] vectors):
        """This method allocates double[n_vectors][n_dim] and copy vectors there"""
        cdef double **c_vectors
        cdef int n_vectors = len(vectors)
        cdef int n_dim = len(vectors[0])

        c_vectors = <double **>malloc(n_vectors*sizeof(double *))
        for y in range(n_vectors):
            c_vectors[y] = <double *>malloc(n_dim*sizeof(double))
            for x in range(n_dim):
                c_vectors[y][x] = <double>vectors[y, x]
        return c_vectors
    
    cdef int *generate_c_labels(self, np.ndarray[np.int8_t, ndim=1] labels):
        """This method allocates int[n_labels] and copy labels there"""
        cdef int *c_labels
        cdef int n_labels = len(labels)
        c_labels = <int *>malloc(n_labels*sizeof(int))
        for i in range(n_labels):
            c_labels[i] = <int>labels[i]
        return c_labels
    
    cdef void free_c_vectors(self, double **c_vectors, int n_vectors):
        for i in range(n_vectors):
            free(c_vectors[i])
        free(c_vectors)
    
    cdef void free_c_labels(self, int *c_labels):
        free(c_labels)

    def __init__(self, leaf_size, n_trials):
        ctree.init_classifier(n_trials, leaf_size, 1)

    def fit(self, np.ndarray[np.float64_t, ndim=2] vectors, 
            np.ndarray[np.int8_t, ndim=1] labels, leaf_size):
 
        cdef double **c_vectors
        cdef int *c_labels
        
        cdef int n_vectors = len(vectors)
        cdef int n_dim = len(vectors[0])
    
        c_vectors = self.generate_c_vectors(vectors)
        c_labels = self.generate_c_labels(labels)

        self.tree = ctree.fit(c_vectors, c_labels, n_vectors, n_dim)
        
        self.free_c_vectors(c_vectors, n_vectors)
        self.free_c_labels(c_labels)

        if(self.tree is NULL):
            raise MemoryError()

        return self
    
    def show(self):
        ctree.show_tree(self.tree)

    def predict(self, np.ndarray[np.float64_t, ndim=2] vectors):
        cdef int n_vectors = len(vectors) 
        cdef double **c_vectors
        cdef int *results        
        
        c_vectors = self.generate_c_vectors(vectors)
        results = ctree.predict(self.tree, c_vectors, n_vectors) 

        self.free_c_vectors(c_vectors, n_vectors)
        
        cdef np.ndarray np_results = np.empty(n_vectors).astype(np.int8)
        for i in range(n_vectors):
            np_results[i] = <np.int8_t>results[i]
        return np_results
    
    def __dealloc__(self):
        if(self.tree is not NULL): 
            ctree.free_tree(self.tree)
