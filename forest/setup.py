from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

extensions = [
    Extension("tree", 
              [
                  "tree.pyx", 
                  "../tree/classifier.c", 
                  "../tree/dataset.c",
                  "../tree/dictionary.c",
                  "../tree/visualizer.c",
                  "../error/error.c"
              ]
    )
]

setup(ext_modules=cythonize(extensions, gdb_debug=True))
