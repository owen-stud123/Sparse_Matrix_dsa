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
