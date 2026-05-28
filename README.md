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
```

## Ejecución de Pruebas

El ejecutable requiere pasar 5 argumentos obligatorios por línea de comandos para definir el tamaño de la prueba y la generación de datos.

### Sintaxis:
```bash
./matriz_test <filename> <RUNS> <LOWER> <UPPER> <STEP>
```
## Explicación de los parámetros:

* `<filename>`: Nombre del archivo de salida (recomendado usar extensión .csv). El archivo se creará automáticamente.
* `<RUNS>`: Número de repeticiones por cada tamaño de matriz. Debe ser un número mayor o igual a 4 (se recomienda 32 para buena validez estadística).
* `<LOWER>`: Límite inferior del tamaño de la matriz (n). Debe ser potencia de 2.
* `<UPPER>`: Límite superior del tamaño de la matriz. Debe ser potencia de 2.
* `<STEP>`: Factor multiplicativo. Define cómo crecerá n en cada iteración (normalmente se usa 2 para ir doblando el tamaño).

## Ejemplo de uso:

Si se quiere probar matrices desde 2 x 2 hasta 512 x 512, duplicando el tamaño en cada paso, y realizando 32 ejecuciones por tamaño para guardar todo en resultados.csv, se debe ejecutar:

```bash
./matriz_test resultados.csv 32 2 512 2
```

## Configuración Avanzada

Para cambiar el comportamiento de las pruebas, se debe editar las variables `constexpr` dentro del archivo uhr.cpp (línea 25 en adelante) y volver a compilar:

* `kExperiment`: Define el tipo de relleno de matriz (1 = reales aleatorios [0,1], 2 = enteros [-10, 10], 3 = identidad, 4 = constantes).
* `kCheckCorrectness`: Valida si el algoritmo de Strassen produce los mismos resultados que el clásico.
* `kSweepN0`: Si se establece en true, el programa busca el mejor tamaño de corte híbrido (n_0) automáticamente probando valores desde kN0Min hasta kN0Max.

Para intercambiar cuál algoritmo medir en el benchmark (Clásico, Strassen puro, o Híbrido), comentar/descomentar las respectivas líneas Matriz C = ... (cerca de la línea 133) en uhr.cpp.

La suite uhr.cpp fue creada por nuestro excelente ayudante Leonardo Lovera. Para más detalles visitar el siguiente repositorio: https://github.com/leonardlover/uhr.git
