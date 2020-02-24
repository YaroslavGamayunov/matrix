//
// Created by Ярослав Гамаюнов on 2020-02-24.
//

#ifndef MATRIX_FIELD_H
#define MATRIX_FIELD_H

class Field {
public:
    virtual Field &getZero() = 0;

    virtual Field &getOne() = 0;

    virtual Field &getAddInverse() = 0;

    virtual Field &getMulInverse() = 0;
};

#endif //MATRIX_FIELD_H
