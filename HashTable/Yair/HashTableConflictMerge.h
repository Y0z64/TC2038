#include "Hash.h"
#include <functional>
#include <iostream>
#include <openssl/evp.h>
#include <optional>
#include <string>
#include <vector>

template <typename K, typename V> class HashTable {
private:
  int INITIAL_SIZE = 10;
  double LOAD_FACTOR = 0.8;

  bool no_hash;

  std::vector<std::vector<std::pair<K, V>>> table;
  int size;
  int count;
  Hash<K> hasher;

  std::function<int(const K &)> hash;

  int defaultHash(const K &k);
  int noHash(const K &k);

  // Simple resize function that will double the size of the table
  void resize();

public:
  // Initialize the Hashtable to 0 and resize it to the initial value
  HashTable()
      : size(INITIAL_SIZE), count(0), hasher(),
        hash([this](const K &k) { return hasher.hash(k, table.size()); }) {
    table.resize(size);
  };

  void insert(const K &key, const V &value);

  std::optional<V> get(const K &key);

  void remove(const K &key);

  int getSize() const { return size; };

  int getCount() const { return count; };

  double getCurrentLoadFactor() const { return double(count) / size; };

  std::vector<std::vector<std::pair<K, V>>> getTable() const { return table; };

  void display();
};

template <typename K, typename V> int HashTable<K, V>::defaultHash(const K &k) {
  return hasher.hash(k, table.size());
}

template <typename K, typename V> int HashTable<K, V>::noHash(const K &k) {
  if (std::is_integral<K>::value) {
    return k;
  } else {
    return 0;
  }
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

template <typename K, typename V>
std::optional<V> HashTable<K, V>::get(const K &key) {
  // Convert the key to an index
  int index = hash(key);
  // Access the bucket at the index and return the value if the key exists
  for (const std::vector<std::pair<K, V>> vec : table[index]) {
    for (const std::pair pair : vec)
    {
      if(pair->first == key)
        return pair->second;
    }
  }
  return nullptr;
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