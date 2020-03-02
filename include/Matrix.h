//
// Created by Ярослав Гамаюнов on 2020-02-23.
//

#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H

#include "Rational.h"
#include "../src/math_utils.h"

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
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                transposedMatrix[j][i] = this[i][j];
            }
        }
        return transposedMatrix;
    }

    void invert() {
        
    }

    unsigned rank() {
        return echelonForm(rows, false).size();
    }

    Field det() {
        COMPILE_ASSERT(N == M);
        Field result = Field::ONE;
        auto triangularForm = echelonForm(rows);
        for (int i = 0; i < M; i++) {
            result = result * triangularForm[i][i];
        }
        return result;
    }

    Field trace() {
        COMPILE_ASSERT(N == M);
        Field sum = Field::ZERO;
        for (int i = 0; i < N; i++) {
            sum += rows[i][i];
        }
        return sum;
    }

    std::vector<Field> getRow(unsigned index) {
        return rows[index];
    }

    std::vector<Field> getColumn(unsigned index) {
        std::vector<Field> column(N);
        for (int i = 0; i < N; i++) {
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
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                this[i][j] -= other[i][j];
            }
        }
        return *this;
    }

    Matrix<M, N, Field> &operator+=(const Matrix<M, N, Field> other) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                this[i][j] += other[i][j];
            }
        }
        return *this;
    }

    Matrix<M, N, Field> &operator*=(const Field &val) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
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
