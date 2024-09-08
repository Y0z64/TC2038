#include <iostream>
#include <string>
#include <limits>
#include "../HashTable/HashTable.h"

unsigned long long fibMemoized(int n, HashTable<int, unsigned long long>& memo, bool& overflowed);

unsigned long long fibonacci(int n) {
    HashTable<int, unsigned long long> memo;
    bool overflowed = false;
    unsigned long long result = fibMemoized(n, memo, overflowed);
    if (overflowed) {
        std::cout << "Stack sobrecargado" << std::endl;
    }
    return result;
}

unsigned long long fibTabulated(int n) {
    if (n <= 1) return n;

    HashTable<int, unsigned long long> memo;
    memo.insert(0, 0);
    memo.insert(1, 1);

    for (int i = 2; i <= n; i++) {
        unsigned long long prev1, prev2;
        memo.get(i - 1, prev1);
        memo.get(i - 2, prev2);
        memo.insert(i, prev1 + prev2);
    }

    unsigned long long value;
    memo.get(n, value);
    return value;
}

unsigned long long fibMemoized(int n, HashTable<int, unsigned long long>& memo, bool& overflowed) {
    if (overflowed) return 0;
    if (n <= 1) return n;

    unsigned long long result;
    if (memo.get(n, result)) {
        return result;
    }

    bool overflow1 = false, overflow2 = false;
    unsigned long long prev1 = fibMemoized(n - 1, memo, overflow1);
    unsigned long long prev2 = fibMemoized(n - 2, memo, overflow2);

    overflowed = overflow1 || overflow2;

    if (!overflowed && prev1 > std::numeric_limits<unsigned long long>::max() - prev2) {
        overflowed = true;
        return 0; 
    }

    if (!overflowed) {
        result = prev1 + prev2;
        memo.insert(n, result);
    }

    return overflowed ? 0 : result;
}

int main() {
    int n;
    std::cout << "Ingresa un número de de la seria de Fibonacci a calcular: ";
    std::cin >> n;

    unsigned long long result = fibonacci(n);
    if (result > 0) {
        std::cout << "Resultado memoizado: " << result << std::endl;
    } else {
        std::cout << "El " << n << "Resultado memoizado es demasiado grande para representar con unsigned long long." << std::endl;
    }

    int n2;
    std::cout << "Ingresa un número de la serie de Fibonacci a calcular usando tabulación: ";
    std::cin >> n2;
    
    unsigned long long result2 = fibTabulated(n2);
    if (result2 > 0) {
        std::cout << "Resultado memoizado: " << result << std::endl;
    } else {
        std::cout << "El " << n << "Resultado es demasiado grande para representar con unsigned long long." << std::endl;
    }

    return 0;
}