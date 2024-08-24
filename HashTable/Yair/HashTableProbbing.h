#include <iostream>
#include <openssl/evp.h>
#include <string>
#include <vector>
#include <optional>
#include "Hash.h"


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
template <typename K, typename V>
class HashTable
{
private:
  struct Bucket;

  std::vector<Bucket> table;
  int size;
  int count;
  
  int INITIAL_SIZE = 10;
  const double LOAD_FACTOR = 0.8;

  Hash<K> hasher;

  int hash(const K &k);

  void resize();

public:
  HashTable() : size(INITIAL_SIZE), count(0), hasher() { table.resize(size); };
  HashTable(int initialSize) : size(initialSize), count(0), hasher() { table.resize(size); };

  void insert(const K& key, const V& value);

  std::optional<K> get(const K& key);

  void remove(const K& key);

  int getSize();
  int getCount();
  double getCurrentLoadFactor();
};