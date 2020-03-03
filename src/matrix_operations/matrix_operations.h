//
// Created by Ярослав Гамаюнов on 2020-03-02.
//

#ifndef MATRIX_MATRIX_OPERATIONS_H
#define MATRIX_MATRIX_OPERATIONS_H

#include "../../include/Rational.h"

template<unsigned M, unsigned N, typename Field>
class Matrix;

// This class represents operations to be performed on a single matrix
template<unsigned M, unsigned N, typename Field>
class MatrixOperation {
public:
    virtual void operator()(Matrix<M, N, Field> &matrix) = 0;
};


// Adds row2 * factor + to row1
template<unsigned M, unsigned N, typename Field>
class MatrixAddRowOperation : public MatrixOperation<M, N, Field> {
public:
    MatrixAddRowOperation(unsigned int row1, unsigned int row2, Field factor) : row1(row1),
                                                                                row2(row2),
                                                                                factor(factor) {}

    void operator()(Matrix<M, N, Field> &matrix) override {
        matrix[row1] = matrix[row1] + matrix[row2] * factor;
    }

private:
    unsigned row1;
    unsigned row2;
    Field factor;
};

// Swaps row1 and row2
template<unsigned M, unsigned N, typename Field>
class MatrixSwapRowsOperation : public MatrixOperation<M, N, Field> {
public:
    MatrixSwapRowsOperation(unsigned int row1, unsigned int row2) : row1(row1), row2(row2) {}

    void operator()(Matrix<M, N, Field> &matrix) override {
        std::swap(matrix[row1], matrix[row2]);
    }

private:
    unsigned row1;
    unsigned row2;
};

// Multiplies row by factor
template<unsigned M, unsigned N, typename Field>
class MatrixMultiplyRowOperation : public MatrixOperation<M, N, Field> {
public:
    MatrixMultiplyRowOperation(unsigned int row, Field factor) : row(row), factor(factor) {}

public:
    void operator()(Matrix<M, N, Field> &matrix) override {
        matrix[row] = matrix[row] * factor;
    }

private:
    unsigned row;
    Field factor;
};

#endif //MATRIX_MATRIX_OPERATIONS_H
