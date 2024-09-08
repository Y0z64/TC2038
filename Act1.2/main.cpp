    #include <iostream>
    #include <vector>
    #include <algorithm>
    #include <string>
    #include <chrono>
    #include <functional>
    #include <cmath>
    #include "../Support/HashTable/HashTable.h"

    /*
        * Implementación de Programación Dinámica y Programación Voraz para calcular el cambio
        * Nota: El programa espera que el archivo de texto tenga el formato correcto.
        * 
        * Autores: 
        - Daniel Alfredo Barreras Meraz
        - Yair Salvador Beltrán Ríos
        * Matrículas:
        - A01254805
        - A01254673
        * Fecha: 29 de agosto de 2024
    */

    // Función para medir el tiempo de ejecución
    // Complejidad: O(1) - Tiempo constante para medir el tiempo
    template<typename Func, typename... Args>
    double measureExecutionTime(Func func, Args&&... args) {
        auto start = std::chrono::high_resolution_clock::now();
        func(std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        return duration.count();
    }

    // Función principal para calcular el cambio
    // Complejidad: O(C * N) en el peor caso, donde C es el tamano del cambio y N es el número de denominaciones
    // Utilizando el Teorema Maestro: T(n) = aT(n/b) + f(n), donde a = 1, b > 1, f(n) = O(n)
    // Caso 3 del Teorema Maestro: T(n) = Θ(n log n)
    std::vector<int> calculateChange(const std::vector<double>& denominations, double change, 
                                    std::vector<int>& supply, HashTable<std::string, std::vector<int>>& cache) {
        // Redondear el cambio a 2 decimales para evitar problemas de precisión de punto flotante
        change = std::round(change * 100.0) / 100.0;
        
        // Crear la clave de caché
        std::string key = std::to_string(change);

        // Verificar si la solución está en caché
        std::vector<int> result;
        if (cache.get(key, result)) {
            return result; // Retorna la solución cacheada si existe
        }

        int N = denominations.size();
        result = std::vector<int>(N, 0);

        // Caso base: si el cambio es 0, retornar resultado vacío
        if (change < 1e-9) {
            cache.insert(key, result);
            return result;
        }

        // Intentar usar cada denominación
        // Este bucle implementa un enfoque de programación dinámica con backtracking
        for (int i = 0; i < N; ++i) {
            if (denominations[i] <= change && supply[i] > 0) {
                // Usar esta denominación
                supply[i]--;
                std::vector<int> subResult = calculateChange(denominations, change - denominations[i], supply, cache);
                supply[i]++; // Restaurar el suministro para backtracking

                // Si se encontró una solución válida para el cambio restante
                if (subResult[N-1] != -1) {
                    result = subResult;
                    result[i]++;
                    
                    // Cachear esta sub-solución
                    cache.insert(key, result);
                    return result;
                }
            }
        }

        // Si no se encontró una solución válida, cachear este hecho
        result[N-1] = -1; // Usar el último elemento como bandera para solución inválida
        cache.insert(key, result);
        return result;
    }

    // Función greedy para calcular el cambio
    // Complejidad: O(N), donde N es el número de denominaciones
    // Esta función utiliza un enfoque voraz (greedy) para encontrar una solución local
    std::vector<int> calculateChangeGreedy(const std::vector<double>& denominations, double change, 
                                        std::vector<int>& supply, HashTable<std::string, std::vector<int>>& cache) {
        // Redondear el cambio a 2 decimales para evitar problemas de precisión de punto flotante
        change = std::round(change * 100.0) / 100.0;
        
        // Crear la clave de caché
        std::string key = std::to_string(change);

        // Verificar si la solución está en caché
        std::vector<int> result;
        if (cache.get(key, result)) {
            return result; // Retorna la solución cacheada si existe
        }

        int N = denominations.size();
        result = std::vector<int>(N, 0);
        double remainingChange = change;

        // Iterar sobre las denominaciones de mayor a menor
        for (int i = 0; i < N && remainingChange > 1e-9; ++i) {
            // Calcular cuántas monedas/billetes de esta denominación podemos usar
            int count = std::min(static_cast<int>(remainingChange / denominations[i]), supply[i]);
            result[i] = count;
            remainingChange -= count * denominations[i];
            remainingChange = std::round(remainingChange * 100.0) / 100.0; // Redondear para evitar errores de punto flotante
        }

        // Si queda cambio por dar, la solución greedy no pudo encontrar una solución válida
        if (remainingChange > 1e-9) {
            result[N-1] = -1; // Indicar que no se encontró solución
        }

        // Almacenar el resultado en caché
        cache.insert(key, result);

        return result;
    }

    int main() {
        int N;
        double P, Q;
        std::cin >> N;

        std::vector<double> denominations(N);
        std::vector<int> supply(N);

        // Leer las denominaciones
        for (int i = 0; i < N; i++) {
            std::cin >> denominations[i];
        }

        // Leer el precio y el pago
        std::cin >> P >> Q;

        // Leer el suministro de cada denominación
        for (int i = 0; i < N; i++) {
            std::cin >> supply[i];
        }

        double change = Q - P;

        HashTable<std::string, std::vector<int>> cache;
        HashTable<std::string, std::vector<int>> cacheGreedy;

        std::vector<double> executionTimesOptimal;
        std::vector<double> executionTimesGreedy;
        std::vector<double> inputSizes;

        // Ejecutar ambos algoritmos varias veces para medir su comportamiento
        for (int i = 0; i < 5; ++i) {
            // Solución óptima con programación dinámica y backtracking
            double executionTimeOptimal = measureExecutionTime([&]() {
                std::vector<int> result = calculateChange(denominations, change, supply, cache);

                if (i == 0) {
                    std::cout << "Solución Óptima:" << std::endl;
                    if (result[N-1] == -1) {
                        std::cout << "No se encontró solución óptima" << std::endl;
                    } else {
                        for (int j = 0; j < N; j++) {
                            if (result[j] > 0) {
                                std::cout << result[j] << " x " << denominations[j] << " pesos" << std::endl;
                            }
                        }
                    }
                }
            });

            // Solución greedy (voraz)
            double executionTimeGreedy = measureExecutionTime([&]() {
                std::vector<int> result = calculateChangeGreedy(denominations, change, supply, cacheGreedy);

                if (i == 0) {
                    std::cout << "\nSolución Greedy:" << std::endl;
                    if (result[N-1] == -1) {
                        std::cout << "No se encontró solución greedy" << std::endl;
                    } else {
                        for (int j = 0; j < N; j++) {
                            if (result[j] > 0) {
                                std::cout << result[j] << " x " << denominations[j] << " pesos" << std::endl;
                            }
                        }
                    }
                }
            });

            executionTimesOptimal.push_back(executionTimeOptimal);
            executionTimesGreedy.push_back(executionTimeGreedy);
            inputSizes.push_back(change * (i + 1));  // Simular un aumento en el tamaño de entrada

            std::cout << "Ejecución " << i+1 << " - Tiempo Óptimo: " << executionTimeOptimal 
                    << " ms, Tiempo Greedy: " << executionTimeGreedy << " ms" << std::endl;
        }

        return 0;
    }

    /* Despues de hacer varias pruebas en el codigo nos dimos cuenta de que:
        el algoritmo Greedy suele ser más rápido que el de backtracking, 
        pero no garantiza la solución óptima en todos los casos.

        Aquí hay algunas diferencias clave entre los enfoques Greedy y de backtracking:

        1. Complejidad algorítmica:
        - El algoritmo Greedy tiene una complejidad de O(N).
        - El algoritmo de backtracking tiene una complejidad de O(C * N) en el peor caso.
        2. Enfoque de toma de decisiones:
        - Greedy toma decisiones localmente óptimas en cada paso, sin reconsiderar.
        - Backtracking explora múltiples combinaciones, lo que puede llevar más tiempo.

        3. Uso de memoria:
        - Greedy utiliza menos memoria, ya que no necesita almacenar estados intermedios.
        - Backtracking puede requerir más memoria debido a la recursión y almacenamiento de estados.

        4. Recorrido del árbol de decisiones:
        - Greedy sigue un único camino en el árbol de decisiones.
        - Backtracking explora múltiples ramas, lo que aumenta el tiempo de ejecución.

        5. Eficiencia en sistemas monetarios bien diseñados:
        - Greedy suele encontrar la solución óptima rápidamente.
        - Backtracking garantiza la optimizacion, pero a costa de explorar más posibilidades.

        6. Impacto de la caché:
        - Aunque ambos algoritmos usan caché, Greedy se beneficia más debido a su naturaleza.
        - Backtracking puede tener un patrón de acceso a caché menos predecible.

        7. Estructura de datos y operaciones:
        - Greedy utiliza principalmente operaciones simples y acceso secuencial a datos.
        - Backtracking involucra más operaciones complejas como la recursión.

        8. Pruning (poda) en backtracking:
        - Aunque el backtracking implementa poda, aún explora más posibilidades que Greedy.
        - Greedy naturalmente 'poda' al tomar decisiones inmediatas sin retroceder.

        9. Predictibilidad del flujo de ejecución:
        - El flujo de Greedy es más predecible, lo que puede llevar a mejor optimización por parte del compilador.
        - Backtracking tiene un flujo menos predecible debido a la naturaleza recursiva y las múltiples ramas.

        10. Localidad de referencia:
            - Greedy tiende a tener mejor localidad de referencia, aprovechando mejor la caché del procesador.
            - Backtracking puede sufrir más fallos de caché debido a su naturaleza recursiva y no lineal.

        Es importante notar que, aunque Greedy es generalmente más rápido, no siempre garantiza la solución óptima
        en todos los casos posibles, especialmente en sistemas con restricciones complejas.
        */