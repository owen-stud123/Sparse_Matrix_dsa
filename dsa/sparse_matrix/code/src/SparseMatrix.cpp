#include "SparseMatrix.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>

SparseMatrix::SparseMatrix(int numRows, int numCols) : rows(numRows), cols(numCols) {
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
    elements = new Element*[rows];
    for (int i = 0; i < rows; i++) {
        elements[i] = nullptr;
    }
}

// Copy constructor
SparseMatrix::SparseMatrix(const SparseMatrix& other) : rows(other.rows), cols(other.cols) {
    elements = new Element*[rows];
    for (int i = 0; i < rows; i++) elements[i] = nullptr;
    
    // Deep copy all elements
    for (int i = 0; i < rows; i++) {
        Element* current = other.elements[i];
        Element* prev = nullptr;
        while (current) {
            Element* newElem = new Element{current->row, current->col, current->value, nullptr};
            if (prev) {
                prev->next = newElem;
            } else {
                elements[i] = newElem;
            }
            prev = newElem;
            current = current->next;
        }
    }
}

// Assignment operator
SparseMatrix& SparseMatrix::operator=(const SparseMatrix& other) {
    if (this != &other) {
        // Clean up existing data
        for (int i = 0; i < rows; i++) {
            Element* current = elements[i];
            while (current) {
                Element* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] elements;
        
        // Copy new data
        rows = other.rows;
        cols = other.cols;
        elements = new Element*[rows];
        for (int i = 0; i < rows; i++) {
            elements[i] = nullptr;
        }
        
        // Deep copy all elements
        for (int i = 0; i < rows; i++) {
            Element* current = other.elements[i];
            Element* prev = nullptr;
            while (current) {
                Element* newElem = new Element{current->row, current->col, current->value, nullptr};
                if (prev) {
                    prev->next = newElem;
                } else {
                    elements[i] = newElem;
                }
                prev = newElem;
                current = current->next;
            }
        }
    }
    return *this;
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

SparseMatrix::SparseMatrix(const std::string& filePath) : rows(0), cols(0), elements(nullptr) {
    std::ifstream file(filePath);
    if (!file) throw std::invalid_argument("Cannot open file");

    std::string line;
    bool foundDimensions = false;
    
    // First pass: find dimensions
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (line.substr(0, 2) == "//") continue;
        
        if (line.find("rows=") == 0) {
            rows = std::stoi(line.substr(5));
            if (cols > 0) {
                foundDimensions = true;
                break;
            }
        } else if (line.find("cols=") == 0) {
            cols = std::stoi(line.substr(5));
            if (rows > 0) {
                foundDimensions = true;
                break;
            }
        }
    }
    
    if (!foundDimensions || rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Invalid or missing matrix dimensions in file");
    }

    // Initialize the matrix
    elements = new Element*[rows];
    for (int i = 0; i < rows; i++) {
        elements[i] = nullptr;
    }

    // Second pass: read matrix entries
    file.clear();
    file.seekg(0, std::ios::beg);
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (line.substr(0, 2) == "//") continue;
        if (line.find("rows=") == 0 || line.find("cols=") == 0) continue;
        
        // Remove whitespace
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (line.empty()) continue;
        if (line[0] != '(' || line.back() != ')')
            throw std::invalid_argument("Input file has wrong format");

        line = line.substr(1, line.length() - 2);
        std::stringstream ss(line);
        std::string token;
        int vals[3], i = 0;

        while (std::getline(ss, token, ',') && i < 3) {
            if (token.find('.') != std::string::npos)
                throw std::invalid_argument("Input file has wrong format");
            vals[i++] = std::stoi(token);
        }
        if (i != 3) throw std::invalid_argument("Input file has wrong format");
        
        // Validate bounds
        if (vals[0] < 0 || vals[0] > rows || vals[1] < 0 || vals[1] > cols) {
            throw std::invalid_argument("Matrix entry out of bounds: (" + 
                                      std::to_string(vals[0]) + "," + std::to_string(vals[1]) + ")");
        }
        
        insert(vals[0], vals[1], vals[2]);
    }
}

void SparseMatrix::insert(int row, int col, int value) {
    if (value == 0) return;
    if (row < 0 || row > rows || col < 0 || col > cols) {
        throw std::out_of_range("Index out of bounds");
    }
    
    Element* newElem = new Element{row, col, value, elements[row]};
    elements[row] = newElem;
}

int SparseMatrix::getElement(int row, int col) {
    if (row < 0 || row > rows || col < 0 || col > cols) {
        throw std::out_of_range("Index out of bounds");
    }
    
    Element* current = elements[row];
    while (current) {
        if (current->col == col) return current->value;
        current = current->next;
    }
    return 0;
}

void SparseMatrix::setElement(int row, int col, int value) {
    if (row < 0 || row > rows || col < 0 || col > cols) {
        throw std::out_of_range("Index out of bounds");
    }
    
    Element* current = elements[row];
    Element* prev = nullptr;
    
    // Find existing element
    while (current) {
        if (current->col == col) {
            if (value == 0) {
                // Remove element
                if (prev) {
                    prev->next = current->next;
                } else {
                    elements[row] = current->next;
                }
                delete current;
            } else {
                // Update value
                current->value = value;
            }
            return;
        }
        prev = current;
        current = current->next;
    }
    
    // Element not found, add new one if value is non-zero
    if (value != 0) {
        Element* newElem = new Element{row, col, value, elements[row]};
        elements[row] = newElem;
    }
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
    
    // Add elements from both matrices
    for (int i = 0; i < A.rows; ++i) {
        // Add elements from A
        Element* currA = A.elements[i];
        while (currA) {
            result.setElement(currA->row, currA->col, currA->value);
            currA = currA->next;
        }
        
        // Add elements from B
        Element* currB = B.elements[i];
        while (currB) {
            int existingValue = result.getElement(currB->row, currB->col);
            result.setElement(currB->row, currB->col, existingValue + currB->value);
            currB = currB->next;
        }
    }
    return result;
}

SparseMatrix SparseMatrix::subtract(const SparseMatrix& A, const SparseMatrix& B) {
    A.validateDimensions(B, "subtract");
    SparseMatrix result(A.rows, A.cols);
    
    // Copy elements from A
    for (int i = 0; i < A.rows; ++i) {
        Element* curr = A.elements[i];
        while (curr) {
            result.setElement(curr->row, curr->col, curr->value);
            curr = curr->next;
        }
    }
    
    // Subtract elements from B
    for (int i = 0; i < B.rows; ++i) {
        Element* curr = B.elements[i];
        while (curr) {
            int existingValue = result.getElement(curr->row, curr->col);
            result.setElement(curr->row, curr->col, existingValue - curr->value);
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
                int existingValue = result.getElement(i, b->col);
                result.setElement(i, b->col, existingValue + a->value * b->value);
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
    if (!out) {
        throw std::runtime_error("Cannot create output file: " + filePath);
    }
    
    out << "rows=" << rows << "\ncols=" << cols << "\n";
    for (int i = 0; i < rows; ++i) {
        Element* curr = elements[i];
        while (curr) {
            out << "(" << curr->row << ", " << curr->col << ", " << curr->value << ")\n";
            curr = curr->next;
        }
    }
}
