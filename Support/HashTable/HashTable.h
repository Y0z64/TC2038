#include <vector>
#include <list>
#include <string>
#include <cmath>

template<typename K, typename V>
class HashTable {
private:
    static const int INITIAL_SIZE = 10;
    static constexpr double LOAD_FACTOR = 0.80;

    std::vector<std::list<std::pair<K, V>>> table;
    int size;
    int count;

    int hash(const K& key) const;
    void resize();

public:
    HashTable();
    void insert(const K& key, const V& value);
    bool remove(const K& key);
    bool get(const K& key, V& value) const;
    bool update(const K& key, const V& value);
    int getSize() const;
    int getCount() const;
    double getCurrentLoadFactor() const;
};

// Implementation

template<typename K, typename V>
HashTable<K, V>::HashTable() : size(INITIAL_SIZE), count(0) {
    table.resize(size);
}
// Time Complexity: O(1) - Constant time to initialize the vector
// Space Complexity: O(m) where m is the initial size of the table

template<typename K, typename V>
int HashTable<K, V>::hash(const K& key) const {
    return std::hash<K>{}(key) % size;
}
// Time Complexity: O(1) - Constant time hashing and modulo operation
// Space Complexity: O(1) - No additional space used

template<typename K, typename V>
void HashTable<K, V>::resize() {
    int newSize = size * 2;
    std::vector<std::list<std::pair<K, V>>> newTable(newSize);

    for (const auto& bucket : table) {
        for (const auto& pair : bucket) {
            int newIndex = std::hash<K>{}(pair.first) % newSize;
            newTable[newIndex].push_back(pair);
        }
    }

    table = std::move(newTable);
    size = newSize;
}
// Time Complexity: O(n) where n is the number of elements in the table
// - We iterate through all elements once
// Space Complexity: O(m) where m is the new size of the table (2 * original size)
// - We create a new table of double the size

template<typename K, typename V>
void HashTable<K, V>::insert(const K& key, const V& value) {
    if (getCurrentLoadFactor() >= LOAD_FACTOR) {
        resize();
    }

    int index = hash(key);
    for (auto& pair : table[index]) { // O(n) where n is the number of elements in the bucket
        if (pair.first == key) {
            pair.second = value;
            return;
        }
    }

    table[index].emplace_back(key, value);
    count++;
}
// Time Complexity: 
// - Average case: O(1) - Constant time for hashing and insertion at the end of the list
// - Worst case: O(n) if all elements hash to the same index, where n is the number of elements
// - Amortized: O(1) due to occasional resizing
// Space Complexity: O(1) - Only storing one new element

template<typename K, typename V>
bool HashTable<K, V>::remove(const K& key) {
    int index = hash(key);
    auto& bucket = table[index];

    for (auto it = bucket.begin(); it != bucket.end(); ++it) { 
        if (it->first == key) {
            bucket.erase(it);
            count--;
            return true;
        }
    }

    return false;
}
// Time Complexity:
// - Average case: O(1) - Constant time for hashing and typically short list traversal
// - Worst case: O(n) if all elements hash to the same index, where n is the number of elements
// Space Complexity: O(1) - No additional space used

template<typename K, typename V>
bool HashTable<K, V>::get(const K& key, V& value) const {
    int index = hash(key);
    const auto& bucket = table[index];

    for (const auto& pair : bucket) {
        if (pair.first == key) {
            value = pair.second;
            return true;
        }
    }

    return false;
}
// Time Complexity:
// - Average case: O(1) - Constant time for hashing and typically short list traversal
// - Worst case: O(n) if all elements hash to the same index, where n is the number of elements
// Space Complexity: O(1) - No additional space used

template<typename K, typename V>
bool HashTable<K, V>::update(const K& key, const V& value) {
    int index = hash(key);
    auto& bucket = table[index];

    for (auto& pair : bucket) {
        if (pair.first == key) {
            pair.second = value;
            return true;
        }
    }

    return false;
}
// Time Complexity:
// - Average case: O(1) - Constant time for hashing and typically short list traversal
// - Worst case: O(n) if all elements hash to the same index, where n is the number of elements
// Space Complexity: O(1) - No additional space used

template<typename K, typename V>
int HashTable<K, V>::getSize() const {
    return size;
}
// Time Complexity: O(1) - Constant time to return a value
// Space Complexity: O(1) - No additional space used

template<typename K, typename V>
int HashTable<K, V>::getCount() const {
    return count;
}
// Time Complexity: O(1) - Constant time to return a value
// Space Complexity: O(1) - No additional space used

template<typename K, typename V>
double HashTable<K, V>::getCurrentLoadFactor() const {
    return static_cast<double>(count) / size;
}
// Time Complexity: O(1) - Constant time for division
// Space Complexity: O(1) - No additional space used
