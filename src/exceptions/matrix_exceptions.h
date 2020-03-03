//
// Created by Ярослав Гамаюнов on 2020-03-02.
//

#ifndef MATRIX_MATRIX_EXCEPTIONS_H
#define MATRIX_MATRIX_EXCEPTIONS_H

#include <exception>

class DegenerateMatrixException : public std::exception {
public:
    const char *what() const noexcept override {
        return "Matrix is degenerate, its determinant is 0";
    }
};

#endif //MATRIX_MATRIX_EXCEPTIONS_H
