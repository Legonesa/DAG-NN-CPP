#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <iostream>
#include <functional>

class Matrix{
    public:
        size_t rows;
        size_t cols;
        std::vector<double> data; // Flat 1D array representing 2D matrix

        // Constructor: Initializes a matrix of given size with zeros
        Matrix(size_t rowSize, size_t colSize);

        // Fills the matrix with random values between -0.1 and 0.1
        void randomize();

        // Fills the matrix using Xavier (Glorot) initialization for stable gradients
        void randomizeXavier(size_t fanIn, size_t fanOut);

        // Matrix operations
        Matrix multiply(const Matrix& target) const; // Dot product
        Matrix transpose() const; // Flips dimensions
        Matrix map(std::function<double(double)> func) const; // Applies a function element-wise
        Matrix slice(size_t r_offset, size_t c_offset, size_t rows_to_take, size_t cols_to_take) const; // Extracts a sub-matrix
        Matrix concatenate(const Matrix& A, const Matrix& B, int axis); // Merges two matrices
        Matrix add(const Matrix& matrix) const; // Element-wise addition
        
        // Debugging utility
        void print() const;

        // Element access methods (1D to 2D mapping)
        double& at(size_t r, size_t c);
        const double& at(size_t r, size_t c) const;
};

#endif
