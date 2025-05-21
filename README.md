## Sparse Matrix Operations
========================

This is C++ program that implements sparse matrix operations that 
include addition, subtraction, and multiplication.

# USAGE:
  ./sparse_matrix

# INSTRUCTIONS:
  1. When prompted, enter the full path to the first matrix file
  2. Enter the full path to the second matrix file
  3. Choose an operation: add, subtract, or multiply
  4. View results in console and in result_matrix.txt

INPUT FILE FORMAT:
  rows=<number_of_rows>
  cols=<number_of_columns>
  (row, column, value)
  (row, column, value)
  ...

EXAMPLE INPUT:
  rows=4
  cols=4
  (0, 1, 5)
  (2, 3, -2)
  (3, 0, 7)

COMPILATION:
  g++ -o sparse_matrix main.cpp SparseMatrix.cpp -std=c++11

# OPERATIONS:
  - Addition
  - Subtraction
  - Multiplication
