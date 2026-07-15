#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <iostream>
#include <functional>

class Matrix{
    public:
        size_t rows;
        size_t cols;
        std::vector<double> data;

        void randomize();
        Matrix(size_t rowSize, size_t colSize);
        Matrix multiply(const Matrix& target) const;
        Matrix transpose() const;
        Matrix map(std::function<double(double)> func) const;
        Matrix slice(size_t r_offset, size_t c_offset, size_t rows_to_take, size_t cols_to_take) const;
        Matrix concatenate(const Matrix& A, const Matrix& B, int axis);
        void randomizeXavier(size_t fanIn, size_t fanOut);
        Matrix add(const Matrix& matrix) const;
        void print() const;

        double& at(size_t r, size_t c);
        const double& at(size_t r, size_t c) const;
};

#endif