#include <Matrix.hpp>
#include <stdexcept>
#include <random>
#include <iomanip>
#include <cmath>

// Fills matrix with uniform random values [-0.1, 0.1]
void Matrix::randomize() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.1, 0.1);
    for (auto& val : data) val = dis(gen);
}

// Xavier Initialization: Keeps variance stable across deep layers
void Matrix::randomizeXavier(size_t fanIn, size_t fanOut) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    // Calculate limit based on input and output sizes
    double limit = std::sqrt(6.0 / (double)(fanIn + fanOut));
    std::uniform_real_distribution<> dis(-limit, limit);

    for (auto& val : data) val = dis(gen);
}

// Constructor initializes flat vector with zeros
Matrix::Matrix(size_t rowSize, size_t colSize) : cols(colSize), rows(rowSize) {
    data.resize(cols * rows, 0);
}

// Standard Matrix Multiplication (Dot Product)
Matrix Matrix::multiply(const Matrix& target) const{
    if (cols != target.rows) throw std::invalid_argument("Invalid sizes for multiply!");
    Matrix result(rows, target.cols);
    for(size_t i = 0; i < rows; i++){
        for(size_t m = 0; m < target.cols; m++){
            double sum = 0;
            for(size_t k = 0; k < cols; k++){
                sum += at(i, k) * target.at(k, m);
            }
            result.at(i, m) = sum;
        }
    }
    return result;
}

// Transposes the matrix (swaps rows and columns)
Matrix Matrix::transpose() const{
    Matrix result(cols, rows);
    for(size_t i = 0; i < rows; i++){
        for(size_t j = 0; j < cols; j++){
            result.at(j, i) = at(i, j);
        }
    }
    return result;
}

// Applies a given lambda function to every element in the matrix
Matrix Matrix::map(std::function<double(double)> func) const{
    Matrix result(rows, cols);
    for(size_t i = 0; i < data.size(); i++){
        result.data[i] = func(data[i]);
    }
    return result;
}

// Slices out a smaller matrix from the current matrix
Matrix Matrix::slice(size_t r_offset, size_t c_offset, size_t rows_to_take, size_t cols_to_take) const{
    if(r_offset + rows_to_take > rows || c_offset + cols_to_take > cols)
        throw std::out_of_range("Out of the range!");
    Matrix result(rows_to_take, cols_to_take);
    for(size_t i = 0; i < rows_to_take; i++){
        for (size_t k = 0; k < cols_to_take; k++){
            result.at(i, k) = at(r_offset + i, c_offset+k);
        }
    }
    return result;
}

// Concatenates two matrices either horizontally (axis=1) or vertically (axis=0)
Matrix Matrix::concatenate(const Matrix& A, const Matrix& B, int axis){
    if(axis == 1){ // Horizontal merge
        if(A.rows != B.rows) throw std::invalid_argument("Row size is not same!");
        Matrix result(A.rows, A.cols + B.cols);
        for(size_t i = 0; i < A.rows; i++){
            for(size_t k = 0; k < A.cols; k++)
                result.at(i, k) = A.at(i, k);
            for(size_t l = 0; l < B.cols; l++)
                result.at(i, A.cols + l) = B.at(i, l);
        }
        return result;
    } else if (axis == 0){ // Vertical merge
        if(A.cols != B.cols) throw std::invalid_argument("Column size is not same!");
        Matrix result(A.rows + B.rows, A.cols);
        for(size_t i = 0; i < A.cols; i++){
            for(size_t k = 0; k < A.rows; k++)
                result.at(k, i) = A.at(k, i);
            for(size_t l = 0; l < B.rows; l++)
                result.at(A.rows + l, i) = B.at(l, i);
        }
        return result;
    }
    throw std::invalid_argument("Invalid axis for concatenate!");
}

// Element-wise addition
Matrix Matrix::add(const Matrix& matrix) const {
    if (cols != matrix.cols || rows != matrix.rows)
        throw std::invalid_argument("Not same size to add!");
    Matrix result(rows, cols);
    for (size_t i = 0; i < data.size(); i++) {
        result.data[i] = data[i] + matrix.data[i];
    }
    return result;
}

// Prints the matrix for debugging
void Matrix::print() const {
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            std::cout << std::fixed << std::setprecision(2) << at(i, j) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

// Returns a reference to the element at (r, c)
double& Matrix::at(size_t r, size_t c){
    return data[r * cols + c];
}
// Returns a const reference to the element at (r, c)
const double& Matrix::at(size_t r, size_t c) const{
    return data[r * cols + c];
}
