#include "HashTableConflictMerge.h"
#include <openssl/evp.h>
#include <string>
#include <vector>

/**
 * Implementación de una Hash Table
 * Acepta pares de llave-valor y ambos de typo string
 *
 * Autores:
 *    - Daniel Alfredo Barreras Meraz
 *    - Yair Salvador Beltrán Ríos
 * Matrículas:
 *    - A01254805
 *    - A01254673
 * Fecha: 12 de agosto de 2024
 * Compilacion: g++ hash2thetable.cpp -lssl -lcrypto
 *    - Se necesita la libreria OpenSSL para compilar
 */

template <typename K, typename V> int HashTable<K, V>::hash(const K &key) {
  // Unsigned values are used since at 8-bits they go from 0-255 instead
  // of -128 to 127 Since hash values are usually represented as bytes, we can
  // use unsigned chars to represent bytes correctly as positive values
  unsigned char hash[EVP_MAX_MD_SIZE];
  unsigned int lengthOfHash = 0;

  // Using the OpenSSL EVP (ENVELOPE) library to hash the key with
  // sha256
  EVP_MD_CTX *context = EVP_MD_CTX_new();
  if (context != nullptr) {
    if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr)) {

      // If its a string
      if constexpr (std::is_same<K, std::string>::value) {
        EVP_DigestUpdate(context, key.c_str(), key.length());
      }

      // For other values interpret as byte arrays
      else {
        EVP_DigestUpdate(context, &key, sizeof(K));
      }

      if (EVP_DigestFinal_ex(context, hash, &lengthOfHash)) {
        EVP_MD_CTX_free(context);
 
        uint64_t high = 0;
        uint64_t low = 0;

        if (lengthOfHash >= 16) {
          for (unsigned int i = 0; i < 8; i++) {
            high = (high << 8) | hash[i];
          }

          for (unsigned int i = 8; i < 16; i++) {
            low = (low << 8) | hash[i];
          }
        }

        uint64_t result = high ^ low; // XOR the high and low parts of the hash
        return result % table.size();
      }
    }
    EVP_MD_CTX_free(context);
  }

  // If we reach this, the hash failed
  return 0;
}

// Simple resize function that will double the size of the table
template <typename K, typename V> void HashTable<K, V>::resize() {
  // Save the table into an old table
  std::vector<std::vector<std::pair<K, V>>> old_table = table;
  size = size * 2;
  // Clear the table and resize it to the new size
  table.clear();
  table.resize(size);
  count = 0;

  // Insert (bucket by bucket) the pairs from the old table into the new table
  for (const auto &bucket : old_table) {
    for (const auto &pair : bucket) {
      insert(pair.first, pair.second);
    }
  }
}

template <typename K, typename V>
void HashTable<K, V>::insert(const K &key, const V &value) {
  // Check if the load factor has been reached and resize if necessary
  if ((double)count / size >= LOAD_FACTOR) {
    resize();
  }

  // Generate a hash index
  int index = hash(key);
  // Iterate over pairs in the bucket obtained from the hash
  for (auto &pair : table[index]) {
    // Check if the key already exists and update the value
    if (pair.first == key) {
      pair.second = value;
      return;
    }
  }
  // If the key does not exist, add a new pair
  table[index].push_back({key, value});
  // Increment the count
  count++;
}

template <typename K, typename V> V HashTable<K, V>::get(const K &key) {
  // Convert the key to an index
  int index = hash(key);
  // Access the bucket at the index and return the value if the key exists
  for (const auto &pair : table[index]) {
    if (pair.first == key) {
      return pair.second;
    }
  }
  return "";
}

template <typename K, typename V> void HashTable<K, V>::remove(const K &key) {
  // Convert the key to an index
  int index = hash(key);
  // Access the bucket using the index in the table
  auto &bucket = table[index];
  // Iterate over the pairs in the bucket and remove the pair if the key
  // exists
  for (auto i = bucket.begin(); i != bucket.end(); ++i) {
    if (i->first == key) {
      bucket.erase(i);
      count--;
      return;
    }
  }
}

template <typename T> void printValue(const T &value) {
  if constexpr (std::is_arithmetic<T>::value || std::is_same<T, char>::value) {
    // Print the value directly if it's a number, string, or char
    std::cout << value;
  } else if (std::is_same<T, std::string>::value) {
    // Print the string with quotes
    std::cout << "\"" << value << "\"";
  } else {
    // For other types, print the type name or "Object"
    std::cout << typeid(T).name();
  }
}

template <typename K, typename V> void HashTable<K, V>::display() {
  // Iterate over the table and print the pairs in each bucket
  for (int i = 0; i < size; i++) {
    if (!table[i].empty()) { // Check if the bucket is not empty
      std::cout << i << ": ";
      for (const auto &pair : table[i]) {
        // Print the key
        printValue(pair.first);
        std::cout << ":";

        // Print the value
        printValue(pair.second);
        std::cout << " ; ";
      }
      std::cout << std::endl;
    }
  }
}

int main() {
  HashTable<std::string, std::string> hashTable;

  hashTable.insert("1", "1");
  hashTable.insert("2", "2");
  hashTable.insert("6", "6");
  hashTable.insert("99", "99");
  hashTable.insert("13123172837123812738172", "213123172837123812738172");

  hashTable.display();
}