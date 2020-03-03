//
// Created by Ярослав Гамаюнов on 2020-02-23.
//

#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H

#include "Rational.h"
#include "../src/math_utils.h"
#include "../src/exceptions/matrix_exceptions.h"
#include "../src/matrix_operations/matrix_operations.h"

template<unsigned M, unsigned N, typename Field=Rational>
class Matrix {

public:
    Matrix(Field fillValue) {
        rows = std::vector<std::vector<Field>>(M, std::vector<Field>(N, fillValue));
    }

    Matrix() {
        rows = std::vector<std::vector<Field>>(M, std::vector<Field>(N));
    }


    Matrix<N, M, Field> transposed() {
        Matrix<N, M, Field> transposedMatrix();
        for (unsigned i = 0; i < M; i++) {
            for (unsigned j = 0; j < N; j++) {
                transposedMatrix[j][i] = this[i][j];
            }
        }
        return transposedMatrix;
    }

    Matrix<N, M, Field> inverted() {
        COMPILE_ASSERT(N == M);
        if (det() == 0) {
            throw DegenerateMatrixException();
        }
        std::vector<std::shared_ptr<MatrixOperation<M, N, Field>>> operationLog;
        echelonForm<M, N, Field>(rows, true, &operationLog);

        Matrix<N, N> identityMatrix;
        for (unsigned i = 0; i < N; i++) {
            identityMatrix[i][i] = Field::ONE;
        }

        for (auto op : operationLog) {
            (*op)(identityMatrix);
        }
        return identityMatrix;
    }

    void invert() {
        COMPILE_ASSERT(N == M);
        if (det() == 0) {
            throw DegenerateMatrixException();
        }
        *this = inverted();
    }

    unsigned rank() {
        return echelonForm<M, N, Field>(rows, false).size();
    }

    Field det() {
        COMPILE_ASSERT(N == M);
        Field result = Field::ONE;
        auto triangularForm = echelonForm<M, N, Field>(rows);
        for (unsigned i = 0; i < M; i++) {
            result = result * triangularForm[i][i];
        }
        return result;
    }

    Field trace() {
        COMPILE_ASSERT(N == M);
        Field sum = Field::ZERO;
        for (unsigned i = 0; i < N; i++) {
            sum += rows[i][i];
        }
        return sum;
    }

    std::vector<Field> getRow(unsigned index) {
        return rows[index];
    }

    std::vector<Field> getColumn(unsigned index) {
        std::vector<Field> column(N);
        for (unsigned i = 0; i < N; i++) {
            column[i] = rows[i][index];
        }
        return column;
    }

    std::vector<Field> &operator[](unsigned index) {
        return rows[index]; // todo add exceptions
    }

    std::vector<Field> operator[](unsigned index) const {
        return rows[index];
    }

    Matrix<M, N, Field> &operator-=(const Matrix<M, N, Field> other) {
        for (unsigned i = 0; i < M; i++) {
            for (unsigned j = 0; j < N; j++) {
                this[i][j] -= other[i][j];
            }
        }
        return *this;
    }

    Matrix<M, N, Field> &operator+=(const Matrix<M, N, Field> other) {
        for (unsigned i = 0; i < M; i++) {
            for (unsigned j = 0; j < N; j++) {
                this[i][j] += other[i][j];
            }
        }
        return *this;
    }

    Matrix<M, N, Field> &operator*=(const Field &val) {
        for (unsigned i = 0; i < M; i++) {
            for (unsigned j = 0; j < N; j++) {
                this[i][j] *= val;
            }
        }
    }

private:
    std::vector<std::vector<Field>> rows;
};

template<unsigned N, typename Field=Rational>
using SquareMatrix = Matrix<N, N, Field>;

#endif //MATRIX_MATRIX_H
