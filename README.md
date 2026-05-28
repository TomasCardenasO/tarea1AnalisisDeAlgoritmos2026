# Multiplicación de Matrices

Este repositorio contiene la implementación y evaluación de rendimiento de distintos algoritmos para la multiplicación de matrices cuadradas. Incluye el método **Clásico** y el algoritmo de **Strassen** (con la capacidad de ejecutarse en su variante pura o **Híbrida** mediante un umbral de corte $n_0$).

Además, se proporciona una suite de pruebas (basada en `uhr.cpp`) diseñada para realizar experimentos estandarizados, midiendo tiempos promedio de ejecución, desviaciones estándar y cuartiles estadísticos.

## Estructura del Proyecto

* `matrix_mul.h` / `matrix_mul.cpp`: Contiene la lógica matemática. Define la estructura de la matriz y aloja las funciones `multiplicacionClasica` y `strassent`.
* `utils.cpp`: Funciones utilitarias para la validación de argumentos en la consola, barra de progreso y cálculo estadístico de cuartiles.
* `uhr.cpp`: Suite de pruebas principal. Genera matrices aleatorias, ejecuta los algoritmos iterativamente, y exporta los tiempos a un archivo CSV.

## Requisitos Previos

Un compilador de C++ que soporte el estándar C++11 o superior (por ejemplo, GCC `g++` o Clang `clang++`).

## Compilación

Para compilar el proyecto, abrir terminal en el directorio del repositorio y ejecutar el siguiente comando. Se recomienda usar la bandera de optimización `-O3` para medir el rendimiento real de los algoritmos:

```bash
g++ -O3 -std=c++11 uhr.cpp matrix_mul.cpp -o matriz_test
