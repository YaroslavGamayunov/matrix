//
// Created by Ярослав Гамаюнов on 2020-02-23.
//

#ifndef MATRIX_MATRIX_H
#define MATRIX_MATRIX_H

#include "Rational.h"

template<unsigned M, unsigned N, typename Field=Rational>
class Matrix {

public:
    Matrix(Field fillValue) {
        rows = std::vector<std::vector<Field>>(M, std::vector<Field>(N, fillValue));
    }


    Matrix<N, M, Field> transposed() {
        Matrix<N, M, Field> transposedMatrix(Field::getZero());
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                transposedMatrix[j][i] = this[i][j];
            }
        }
        return transposedMatrix;
    }

    std::vector<Field> &operator[](int index) {
        return rows[index]; // todo add exceptions
    }

    std::vector<Field> operator[](int index) const {
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

//template<unsigned M, unsigned N, typename Field=Rational>
#endif //MATRIX_MATRIX_H
