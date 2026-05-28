#include "matrix_mul.h"

Matriz crearMatriz(int n) {
    return Matriz(n, std::vector<double>(n, 0.0));
}

Matriz sumar(const Matriz& A, const Matriz& B) {
    int n = A.size();
    Matriz C = crearMatriz(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

Matriz restar(const Matriz& A, const Matriz& B) {
    int n = A.size();
    Matriz C = crearMatriz(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

Matriz multiplicacionClasica(const Matriz& A, const Matriz& B) {
    int n = A.size();
    Matriz C = crearMatriz(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

Matriz strassent(const Matriz& A, const Matriz& B, int n0) {
    int n = A.size();

    if (n <= n0) {
        return multiplicacionClasica(A, B);
    }

    int m = n / 2;
    Matriz A11 = crearMatriz(m), A12 = crearMatriz(m), A21 = crearMatriz(m), A22 = crearMatriz(m);
    Matriz B11 = crearMatriz(m), B12 = crearMatriz(m), B21 = crearMatriz(m), B22 = crearMatriz(m);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + m];
            A21[i][j] = A[i + m][j];
            A22[i][j] = A[i + m][j + m];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + m];
            B21[i][j] = B[i + m][j];
            B22[i][j] = B[i + m][j + m];
        }
    }

    Matriz M1 = strassent(sumar(A11, A22), sumar(B11, B22), n0);
    Matriz M2 = strassent(sumar(A21, A22), B11, n0);
    Matriz M3 = strassent(A11, restar(B12, B22), n0);
    Matriz M4 = strassent(A22, restar(B21, B11), n0);
    Matriz M5 = strassent(sumar(A11, A12), B22, n0);
    Matriz M6 = strassent(restar(A21, A11), sumar(B11, B12), n0);
    Matriz M7 = strassent(restar(A12, A22), sumar(B21, B22), n0);

    Matriz C11 = sumar(restar(sumar(M1, M4), M5), M7);
    Matriz C12 = sumar(M3, M5);
    Matriz C21 = sumar(M2, M4);
    Matriz C22 = sumar(restar(sumar(M1, M3), M2), M6);

    Matriz C = crearMatriz(n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            C[i][j] = C11[i][j];
            C[i][j + m] = C12[i][j];
            C[i + m][j] = C21[i][j];
            C[i + m][j + m] = C22[i][j];
        }
    }

    return C;
}
