
#include <cstdio>
#include "../../HashTable/Yair//HashTableConflictMerge.h"
#include <string>

int main()
{
    HashTable<std::string, std::string> hashTable;
    hashTable.insert("1", "1");
    hashTable.insert("2", "2");
    
    hashTable.display();
}