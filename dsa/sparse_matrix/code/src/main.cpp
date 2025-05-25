#include "SparseMatrix.h"
#include <iostream>
#include <string>

int main() {
    try {
        std::cout << "Sparse Matrix Operations\n";
        std::cout << "========================\n";
        std::string file1, file2, operation;
        std::cout << "Enter path to first matrix file: ";
        std::getline(std::cin, file1);
        std::cout << "Enter path to second matrix file: ";
        std::getline(std::cin, file2);
        std::cout << "Choose operation (add, subtract, multiply): ";
        std::getline(std::cin, operation);

        SparseMatrix A(file1);
        SparseMatrix B(file2);
        SparseMatrix result(1, 1);

        if (operation == "add") result = SparseMatrix::add(A, B);
        else if (operation == "subtract") result = SparseMatrix::subtract(A, B);
        else if (operation == "multiply") result = SparseMatrix::multiply(A, B);
        else throw std::invalid_argument("Unknown operation");

        result.printMatrix();
        result.toFile("result_matrix.txt");
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
    return 0;
}
