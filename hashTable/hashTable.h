#include <iostream>
#include <openssl/evp.h>
#include <string>
#include <vector>
#include <optional>

/**
 * Implementation of a Hash Table with quadratic probing
 * Accepts key-value pairs, both of type string
 *
 * Authors:
 *    - Daniel Alfredo Barreras Meraz
 *    - Yair Salvador Beltrán Ríos
 * Student IDs:
 *    - A01254805
 *    - A01254673
 * Date: August 12, 2024
 * Compilation: g++ hash2thetable.cpp -lssl -lcrypto
 *    - OpenSSL library is required for compilation
 */
class HashTable
{
private:
  struct Entry;

  std::vector<Entry> table;
  int size;
  int count;
  const double LOAD_FACTOR = 0.8;

  int hash(const std::string &key);
  void resize();

public:
  HashTable(int initial_size = 10);

  void insert(const std::string &key, const std::string &value);

  std::optional<std::string> get(const std::string &key);

  void remove(const std::string &key);

  int getSize();
  int getCount();
  double getCurrentLoadFactor();
};