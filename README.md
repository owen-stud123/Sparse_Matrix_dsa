# Sparse Matrix Operations

This is C++ program that implements sparse matrix operations that 
include addition, subtraction, and multiplication.

## Overview

This project provides a memory efficient implementation for working with sparse matrices (matrices with mostly zero elements). Instead of storing all elements, only non-zero elements are stored.

## Getting Started

### Clone the Repository
```bash
git clone https://github.com/owen-stud123/Sparse_Matrix_dsa.git
cd Sparse_Matrix_dsa/dsa/sparse_matrix/code/
```
Compilation
```
g++ -o sparse_matrix main.cpp SparseMatrix.cpp -std=c++11
```
usage
```
./sparse_matrix
```
How to Use

 1.When prompted, enter the full path to the first matrix file

 2.Enter the full path to the second matrix file
 
 3.Choose an operation: `add`, `subtract`, or `multiply`
 
 4.View results in console and in the generated result_matrix.txt file

Input File Format
```
rows=<number_of_rows>
cols=<number_of_columns>
(row, column, value)
(row, column, value)
...
```
Operations:

•Addition: Combines two matrices of identical dimensions
•Subtraction: Subtracts second matrix from first
•Multiplication: Multiplies matrices (requires columns of first matrix to equal rows of second)

