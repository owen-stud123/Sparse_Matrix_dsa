#include "SparseMatrix.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>


SparseMatrix::SparseMatrix(int numRows, int numCols) {
    rows = numRows;
    cols = numCols;
    elements = new Element*[rows];
    for (int i = 0; i < rows; i++) elements[i] = nullptr;
}

SparseMatrix::~SparseMatrix() {
    for (int i = 0; i < rows; i++) {
        Element* current = elements[i];
        while (current) {
            Element* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] elements;
}

SparseMatrix::SparseMatrix(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) throw std::invalid_argument("Cannot open file");

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (line.find("rows=") == 0) rows = std::stoi(line.substr(5));
        else if (line.find("cols=") == 0) cols = std::stoi(line.substr(5));
        else break;
    }

    elements = new Element*[rows];
    for (int i = 0; i < rows; i++) elements[i] = nullptr;

    do {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        if (line.empty()) continue;
        if (line[0] != '(' || line.back() != ')')
            throw std::invalid_argument("Input file has wrong format");

        line = line.substr(1, line.length() - 2);
        std::stringstream ss(line);
        std::string token;
        int vals[3], i = 0;

        while (std::getline(ss, token, ',')) {
            if (token.find('.') != std::string::npos)
                throw std::invalid_argument("Input file has wrong format");
            vals[i++] = std::stoi(token);
        }
        if (i != 3) throw std::invalid_argument("Input file has wrong format");
        insert(vals[0], vals[1], vals[2]);
    } while (std::getline(file, line));
}

void SparseMatrix::insert(int row, int col, int value) {
    if (value == 0) return;
    Element* newElem = new Element{row, col, value, elements[row]};
    elements[row] = newElem;
}

int SparseMatrix::getElement(int row, int col) {
    Element* current = elements[row];
    while (current) {
        if (current->col == col) return current->value;
        current = current->next;
    }
    return 0;
}

void SparseMatrix::setElement(int row, int col, int value) {
    Element* current = elements[row];
    Element* prev = nullptr;
    while (current) {
        if (current->col == col) {
            current->value = value;
            return;
        }
        prev = current;
        current = current->next;
    }
    Element* newElem = new Element{row, col, value, nullptr};
    if (prev) prev->next = newElem;
    else elements[row] = newElem;
}

void SparseMatrix::validateDimensions(const SparseMatrix& B, const std::string& op) const {
    if ((op == "add" || op == "subtract") && (rows != B.rows || cols != B.cols))
        throw std::invalid_argument("Matrix dimension mismatch for " + op);
    if (op == "multiply" && cols != B.rows)
        throw std::invalid_argument("Matrix dimension mismatch for multiplication");
}

SparseMatrix SparseMatrix::add(const SparseMatrix& A, const SparseMatrix& B) {
    A.validateDimensions(B, "add");
    SparseMatrix result(A.rows, A.cols);
    for (int i = 0; i < A.rows; ++i) {
        Element* curr = A.elements[i];
        while (curr) {
            result.setElement(curr->row, curr->col, result.getElement(curr->row, curr->col) + curr->value);
            curr = curr->next;
        }
        curr = B.elements[i];
        while (curr) {
            result.setElement(curr->row, curr->col, result.getElement(curr->row, curr->col) + curr->value);
            curr = curr->next;
        }
    }
    return result;
}

SparseMatrix SparseMatrix::subtract(const SparseMatrix& A, const SparseMatrix& B) {
    A.validateDimensions(B, "subtract");
    SparseMatrix result(A.rows, A.cols);
    for (int i = 0; i < A.rows; ++i) {
        Element* curr = A.elements[i];
        while (curr) {
            result.setElement(curr->row, curr->col, curr->value);
            curr = curr->next;
        }
        curr = B.elements[i];
        while (curr) {
            result.setElement(curr->row, curr->col, result.getElement(curr->row, curr->col) - curr->value);
            curr = curr->next;
        }
    }
    return result;
}

SparseMatrix SparseMatrix::multiply(const SparseMatrix& A, const SparseMatrix& B) {
    A.validateDimensions(B, "multiply");
    SparseMatrix result(A.rows, B.cols);
    for (int i = 0; i < A.rows; ++i) {
        for (Element* a = A.elements[i]; a; a = a->next) {
            for (Element* b = B.elements[a->col]; b; b = b->next) {
                int val = result.getElement(i, b->col);
                result.setElement(i, b->col, val + a->value * b->value);
            }
        }
    }
    return result;
}

void SparseMatrix::printMatrix() {
    for (int i = 0; i < rows; ++i) {
        Element* curr = elements[i];
        while (curr) {
            std::cout << "(" << curr->row << "," << curr->col << "," << curr->value << ")\n";
            curr = curr->next;
        }
    }
}

void SparseMatrix::toFile(const std::string& filePath) {
    std::ofstream out(filePath);
    out << "rows=" << rows << "\ncols=" << cols << "\n";
    for (int i = 0; i < rows; ++i) {
        Element* curr = elements[i];
        while (curr) {
            out << "(" << curr->row << ", " << curr->col << ", " << curr->value << ")\n";
            curr = curr->next;
        }
    }
}
