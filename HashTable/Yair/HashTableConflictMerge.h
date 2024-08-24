#include "Hash.h"
#include <iostream>
#include <openssl/evp.h>
#include <optional>
#include <string>
#include <vector>

template <typename K, typename V> class HashTable {
private:
  int INITIAL_SIZE = 10;
  double LOAD_FACTOR = 0.8;

  std::vector<std::vector<std::pair<K, V>>> table;
  int size;
  int count;
  Hash<K> hasher;

  int hash(const K &k);

  // Simple resize function that will double the size of the table
  void resize();

public:
  // Initialize the Hashtable to 0 and resize it to the initial value
  HashTable() : size(INITIAL_SIZE), count(0), hasher() { table.resize(size); };

  void insert(const K &key, const V &value);

  std::optional<V> get(const K &key);

  void remove(const K &key);

  int getSize() const { return size; };

  int getCount() const { return count; };

  double getCurrentLoadFactor() const { return double(count) / size; };

  void display();
};
