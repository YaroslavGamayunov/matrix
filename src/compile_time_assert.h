//
// Created by Ярослав Гамаюнов on 2020-02-18.
//

#ifndef MATRIX_STATIC_ASSERT_H
#define MATRIX_STATIC_ASSERT_H

template<bool C>
struct compileTimeAssert {
    static void apply() {
        static const char err[C ? 1 : -1];
    }
};

template<>
struct compileTimeAssert<true> {
    static void apply() {}
};

#define COMPILE_ASSERT(expr) compileTimeAssert<expr>::apply();
#endif //MATRIX_STATIC_ASSERT_H
