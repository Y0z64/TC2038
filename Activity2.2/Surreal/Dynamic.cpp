
#include "../../HashTable/Yair//HashTableConflictMerge.h"
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

std::vector<int> sumVectors(const std::vector<int> &vec1,
                            const std::vector<int> &vec2) {
  if (vec1.size() != vec2.size()) {
    throw std::invalid_argument("Vectors must be of the same size");
  }

  std::vector<int> result(vec1.size());
  for (size_t i = 0; i < vec1.size(); ++i) {
    result[i] = vec1[i] + vec2[i];
  }

  return result;
}

int main() {
  HashTable<unsigned long, std::vector<int>> Table;

  unsigned int n;
  unsigned long price;
  unsigned int paid;
  unsigned long change;
  std::vector<unsigned long> denoms;
  std::vector<int> combination;

  std::cout << "Enter the number of denominations: ";
  std::cin >> n;

  std::cout << "Enter the denominations: " << std::endl;
  for (int i = 0; i < n; i++) {
    std::cin >> denoms[i];
  }

  std::sort(denoms.begin(), denoms.end(), std::greater<long>());

  // Insert the default values and combinations
  for (int i = 0; i < n; i++) {
    std::vector<int> combination(n, 0);

    combination[i] = 1;
    Table.insert(denoms[i], combination);
  };

  std::cout << std::endl
            << "Denominaciones guardadas correctamente!" << std::endl;
  std::cout << "Ingresa el precio del producto a comprar: ";
  std::cin >> price;
  std::cout << "Ingresa el monto pagado: ";
  std::cin >> paid;

  change = price - paid;

  // Edge case
  if (change < 0) {
    std::cout << "Cambio: Literalmente me debes bro";
    return 0;
  }

  std::vector<unsigned long> denomsCopy = denoms;
  while (change != 0) {
    // Check if change combination was already generated
    auto combinationCheck = Table.get(change);
    if (combinationCheck.has_value()) {
      combination = sumVectors(combination, combinationCheck.value());
      change -= change;
    } else {
      // Check that the denoms array is not empty
      if (!denomsCopy.empty()) {
        for (unsigned int dem : denomsCopy) {
          // If no cache was found check with each denomination until change is lesser and substract
          if (change > dem) {
            auto denominationCombination = Table.get(dem);
            // We do not check for has_value because its assured it will be
            // there save the current combination and repeat
            combination =
                sumVectors(combination, denominationCombination.value());
            change -= dem;
            break;
          // For each case the change is not less, pop the highest value and repeat loop
          } else {
            denomsCopy.erase(denomsCopy.begin());
          }
        }
      // If for loop is empty there is no posible change combination left and you are fucked
      } else {
        std::cout << "No es posible dar el cambio :(" << std::endl
                  << "Nos falataron " << change << "pesos :((()))";
        return 1;
      }
    }
  }

  // If change == 0 save current combination with price as index
  Table.insert(price, combination);

  // Print combination in a cool way
  for(int i = 0; i < n; i++)
  {
    unsigned long sum = 0;
    int k = combination[i];
    for (int j = 0; j < k; j++)
    {
      sum += denoms[j];
    }
  }
};

// Figure out a way to save the hash table