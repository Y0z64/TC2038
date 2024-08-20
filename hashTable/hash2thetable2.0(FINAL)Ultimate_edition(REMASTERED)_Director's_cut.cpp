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
  struct Entry
  {
    std::string key;
    std::string value;
    bool isOccupied;

    Entry() : isOccupied(false) {}
    Entry(const std::string &k, const std::string &v, bool occupied)
        : key(k), value(v), isOccupied(occupied) {}
  };

  std::vector<Entry> table;
  int size;
  int count;
  const double LOAD_FACTOR = 0.5;

  int hash(const std::string &key)
  {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;

    EVP_MD_CTX *context = EVP_MD_CTX_new();
    if (context != nullptr)
    {
      if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr))
      {
        if (EVP_DigestUpdate(context, key.c_str(), key.length()))
        {
          if (EVP_DigestFinal_ex(context, hash, &lengthOfHash))
          {
            EVP_MD_CTX_free(context);
            int result = 0;
            for (unsigned int i = 0; i < 8 && i < lengthOfHash; i++)
            {
              result = (result << 8) | hash[i];
            }
            return result % size;
          }
        }
      }
      EVP_MD_CTX_free(context);
    }
    return 0;
  }

  void resize()
  {
    std::vector<Entry> old_table = table;
    size *= 2;
    table = std::vector<Entry>(size);
    count = 0;

    for (const auto &entry : old_table)
    {
      if (entry.isOccupied)
      {
        insert(entry.key, entry.value);
      }
    }
  }

public:
  HashTable(int initial_size = 10) : size(initial_size), count(0)
  {
    table.resize(size);
  }

  void insert(const std::string &key, const std::string &value)
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
    table[index] = Entry(key, value, true);
  }

  std::optional<std::string> get(const std::string &key)
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

  void remove(const std::string &key)
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

  int getSize() const { return size; }
  int getCount() const { return count; }
  double getCurrentLoadFactor() const { return static_cast<double>(count) / size; }
};

int main()
{
  HashTable ht;

  ht.insert("key1", "value1");
  ht.insert("key2", "value2");
  ht.insert("key3", "value3");

  auto value = ht.get("key2");
  if (value)
  {
    std::cout << "Value for key2: " << *value << std::endl;
  }
  else
  {
    std::cout << "key2 not found" << std::endl;
  }

  std::cout << "Current size: " << ht.getSize() << std::endl;
  std::cout << "Current count: " << ht.getCount() << std::endl;
  std::cout << "Current load factor: " << ht.getCurrentLoadFactor() << std::endl;

  return 0;
}