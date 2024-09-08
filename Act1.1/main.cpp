#include <iostream>
#include <vector>
#include "mergeSort.h"

/*
    * Implementación de Merge Sort 
    * Nota: El programa espera que el archivo de texto tenga el formato correcto.
    * 
    * Autores: 
      - Daniel Alfredo Barreras Meraz
      - Yair Salvador Beltrán Ríos
    * Matrículas:
      - A01254805
      - A01254673
    * Fecha: 12 de agosto de 2024
*/

/**
 * Función merge
 * Complejidad Temporal: O(n), donde n es el número total de elementos en ambos vectores de entrada.
 * Esta función siempre compara y fusiona todos los elementos de ambos vectores de entrada.
 * 
 * Análisis:
 * - Inicialización de variables: O(1)
 * - Bucle principal de fusión: O(n)
 * - Bucles de limpieza para elementos restantes: O(k) y O(m) en el peor caso
 * 
 * Complejidad Espacial: O(n), donde n es la suma de los tamaños de leftArr y rightArr
 */
void merge(std::vector<double>& arr, int left, int middle, int right) {
    int leftSize = middle - left + 1;
    int rightSize = right - middle;

    std::vector<double> leftArr(leftSize);
    std::vector<double> rightArr(rightSize);

    for (int i = 0; i < leftSize; ++i) {
        leftArr[i] = arr[left + i];
    }
    for (int j = 0; j < rightSize; ++j) {
        rightArr[j] = arr[middle + 1 + j];
    }

    int leftIndex = 0, rightIndex = 0;
    int mergedIndex = left;

    while (leftIndex < leftSize && rightIndex < rightSize) {
        if (leftArr[leftIndex] <= rightArr[rightIndex]) {
            arr[mergedIndex] = leftArr[leftIndex++];
        } else {
            arr[mergedIndex] = rightArr[rightIndex++];
        }
        ++mergedIndex;
    }

    while (leftIndex < leftSize) {
        arr[mergedIndex++] = leftArr[leftIndex++];
    }

    while (rightIndex < rightSize) {
        arr[mergedIndex++] = rightArr[rightIndex++];
    }
}

/**
 * Función mergeSort
 * Análisis de Complejidad Temporal:
 * T(n) = 2T(n/2) + O(n)
 * Donde:
 *   - 2 es el número de subproblemas en cada paso de división
 *   - n/2 es el tamaño de cada subproblema
 *   - O(n) es el costo de dividir el problema y fusionar los resultados
 * 
 * Aplicando el Teorema Maestro:
 *   a = 2, b = 2, f(n) = O(n)
 *   log_b(a) = log_2(2) = 1
 *   f(n) = O(n^1)
 * 
 * Como f(n) = Θ(n^log_b(a)), estamos en el caso 2 del Teorema Maestro.
 * 
 * Por lo tanto, la complejidad temporal es:
 * Mejor Caso: O(n log n) - cuando el vector ya está ordenado o casi ordenado
 * Caso Promedio: O(n log n)
 * Peor Caso: O(n log n) - cuando el vector está en orden inverso o desordenado aleatoriamente
 */
void mergeSort(std::vector<double>& arr, int left, int right) {
    if (left >= right) {
        return;  // Caso base: un solo elemento está ordenado por definición
    }

    int middle = left + (right - left) / 2;

    // 1. y 2. Dividr el arreglo mediante indices y ordenar recursivamente los subvectores
    mergeSort(arr, left, middle);
    mergeSort(arr, middle + 1, right);

    // 3. Combinar los subvectores ordenados
    merge(arr, left, middle, right);
}


// Main function
int main() {
    int N;
    std::cin >> N;

    std::vector<double> arr(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> arr[i];
    }

    std::cout << "Array original:" << std::endl;
    for (const auto& num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    mergeSort(arr, 0, arr.size() - 1);

    std::cout << "Array ordenado:" << std::endl;
    for (const auto& num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}