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

template <typename K, typename V>
struct HashTable<K, V>::Bucket
{
  K& key;
  V& value;
  bool isOccupied;

  Bucket() : isOccupied(false) {}
  Bucket(const  K& k, const V& v, bool occupied)
      : key(k), value(v), isOccupied(occupied) {}
};

template <typename K, typename V>
int HashTable<K, V>::hash(const K &k)
{
  return hasher.hash(k, size);
}

template <typename K, typename V>
void HashTable<K, V>::resize()
{
  std::vector<Bucket> old_table = table;
  size *= 2;
  table = std::vector<Bucket>(size);
  count = 0;

  for (const auto &entry : old_table)
  {
    if (entry.isOccupied)
    {
      insert(entry.key, entry.value);
    }
  }
}

template <typename K, typename V>
void HashTable<K, V>::insert(const K& key, const V& value)
{
  if (static_cast<double>(count) / size >= LOAD_FACTOR)
  {
    resize();
  }

  int index = hash(key);
  int i = 0;
  while (table[index].isOccupied && table[index].key != key)
  {
    i++;
    index = (index + i * i) % size; // Quadratic probing
  }

  if (!table[index].isOccupied)
  {
    count++;
  }
  table[index] = Bucket(key, value, true);
}

template <typename K, typename V>
std::optional<K> HashTable<K, V>::get(const K& key)
{
  int index = hash(key);
  int i = 0;
  while (table[index].isOccupied)
  {
    if (table[index].key == key)
    {
      return table[index].value;
    }
    i++;
    index = (index + i * i) % size; // Quadratic probing
  }
  return std::nullopt;
}

template <typename K, typename V>
void HashTable<K, V>::remove(const K& key)
{
  int index = hash(key);
  int i = 0;
  while (table[index].isOccupied)
  {
    if (table[index].key == key)
    {
      table[index].isOccupied = false;
      count--;
      return;
    }
    i++;
    index = (index + i * i) % size; // Quadratic probing
  }
}

template <typename K, typename V>
int HashTable<K, V>::getSize() { return size; }

template <typename K, typename V>
int HashTable<K, V>::getCount() { return count; }

template <typename K, typename V>
double HashTable<K,V>::getCurrentLoadFactor() { return static_cast<double>(count) / size; }
