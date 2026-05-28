#ifndef MATRIX_MUL_H
#define MATRIX_MUL_H

#include <vector>

using Matriz = std::vector<std::vector<double>>;

Matriz crearMatriz(int n);
Matriz sumar(const Matriz& A, const Matriz& B);
Matriz restar(const Matriz& A, const Matriz& B);
Matriz multiplicacionClasica(const Matriz& A, const Matriz& B);
Matriz strassent(const Matriz& A, const Matriz& B, int n0 = 1);

#endif
