rm -rf build tree.so tree.c
cython --gdb ./tree.pyx
python setup.py build_ext --inplace #--gdb
