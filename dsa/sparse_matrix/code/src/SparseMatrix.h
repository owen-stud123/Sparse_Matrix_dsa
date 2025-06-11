#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <string>

class SparseMatrix {
public:
    struct Element {
        int row, col, value;
        Element* next;
    };

    SparseMatrix(const std::string& filePath);
    SparseMatrix(int numRows, int numCols);
    
    // Rule of Three - Copy constructor and assignment operator
    SparseMatrix(const SparseMatrix& other);
    SparseMatrix& operator=(const SparseMatrix& other);
    
    ~SparseMatrix();

    int getElement(int row, int col);
    void setElement(int row, int col, int value);
    void printMatrix();
    void toFile(const std::string& filePath);

    static SparseMatrix add(const SparseMatrix& A, const SparseMatrix& B);
    static SparseMatrix subtract(const SparseMatrix& A, const SparseMatrix& B);
    static SparseMatrix multiply(const SparseMatrix& A, const SparseMatrix& B);

private:
    int rows, cols;
    Element** elements;

    void insert(int row, int col, int value);
    void validateDimensions(const SparseMatrix& B, const std::string& op) const;
};

#endif
