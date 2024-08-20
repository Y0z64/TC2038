#include <iostream>
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

class HashTable {
private:
  int INITIAL_SIZE = 10;
  double LOAD_FACTOR = 0.8;

  std::vector<std::vector<std::pair<std::string, std::string>>> table;
  int size;
  int count;

  int hash(const std::string &key) {
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
        if (EVP_DigestUpdate(context, key.c_str(), key.length())) {
          if (EVP_DigestFinal_ex(context, hash, &lengthOfHash)) {
            EVP_MD_CTX_free(context);
            // We use bitwise operations to combine multiple bytes into a
            // single integer We then will be able to use this int as an index
            // for our hash table
            int result = 0;
            for (unsigned int i = 0; i < 8 && i < lengthOfHash; i++) {
              result = (result << 8) | hash[i];
            }
            return result % table.size();
          }
        }
      }
      EVP_MD_CTX_free(context);
    }

    // If we reach this, the hash failed
    return 0;
  }

  // Simple resize function that will double the size of the table
  void resize() {
    // Save the table into an old table
    std::vector<std::vector<std::pair<std::string, std::string>>> old_table = table;
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

public:
  // Initialize the Hashtable to 0 and resize it to the initial value
  HashTable() : size(INITIAL_SIZE), count(0) { table.resize(size); }

  // 
  void insert(const std::string &key, const std::string &value) {
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

  std::string get(const std::string &key) {
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

  void remove(const std::string &key) {
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

  int getSize() const { return size; }

  int getCount() const { return count; }

  double getCurrentLoadFactor() const { return double(count) / size; }
};

int main() {
  HashTable ht;

  ht.insert("key1", "value1");
  ht.insert("key2", "value2");
  ht.insert("key3", "value3");

  std::cout << "Value for key2: " << ht.get("key2") << std::endl;
  std::cout << "Current size: " << ht.getSize() << std::endl;
  std::cout << "Current count: " << ht.getCount() << std::endl;
  std::cout << "Current load factor: " << ht.getCurrentLoadFactor()
            << std::endl;

  return 0;
}