#include <iostream>
#include <string>
#include "HashTable.h"

// Use case 1: Phone book
void phoneBookExample() {
    HashTable<std::string, std::string> phoneBook;

    // Insert contacts
    phoneBook.insert("Alice", "123-456-7890");
    phoneBook.insert("Bob", "234-567-8901");
    phoneBook.insert("Charlie", "345-678-9012");

    // Retrieve a contact
    std::string number;
    if (phoneBook.get("Bob", number)) {
        std::cout << "Bob's number: " << number << std::endl;
    } else {
        std::cout << "Bob not found in the phone book." << std::endl;
    }

    // Update a contact
    phoneBook.update("Alice", "987-654-3210");

    // Remove a contact
    phoneBook.remove("Charlie");

    // Try to retrieve a removed contact
    if (phoneBook.get("Charlie", number)) {
        std::cout << "Charlie's number: " << number << std::endl;
    } else {
        std::cout << "Charlie not found in the phone book." << std::endl;
    }

    std::cout << "Phone book size: " << phoneBook.getSize() << std::endl;
    std::cout << "Number of contacts: " << phoneBook.getCount() << std::endl;
    std::cout << "Current load factor: " << phoneBook.getCurrentLoadFactor() << std::endl;
}

// Use case 2: Word frequency counter
void wordFrequencyExample() {
    HashTable<std::string, int> wordFrequency;

    std::string text = "the quick brown fox jumps over the lazy dog";
    std::string word;

    // Count word frequencies
    for (char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                int count;
                if (wordFrequency.get(word, count)) {
                    wordFrequency.update(word, count + 1);
                } else {
                    wordFrequency.insert(word, 1);
                }
                word.clear();
            }
        } else {
            word += c;
        }
    }

    // Count the last word
    if (!word.empty()) {
        int count;
        if (wordFrequency.get(word, count)) {
            wordFrequency.update(word, count + 1);
        } else {
            wordFrequency.insert(word, 1);
        }
    }

    // Print word frequencies
    std::vector<std::string> words = {"the", "quick", "brown", "fox", "jumps", "over", "lazy", "dog"};
    for (const auto& w : words) {
        int frequency;
        if (wordFrequency.get(w, frequency)) {
            std::cout << "Frequency of '" << w << "': " << frequency << std::endl;
        } else {
            std::cout << "Word '" << w << "' not found." << std::endl;
        }
    }

    std::cout << "Word frequency table size: " << wordFrequency.getSize() << std::endl;
    std::cout << "Number of unique words: " << wordFrequency.getCount() << std::endl;
    std::cout << "Current load factor: " << wordFrequency.getCurrentLoadFactor() << std::endl;
}

int main() {
    std::cout << "Phone Book Example:" << std::endl;
    phoneBookExample();

    std::cout << "\nWord Frequency Example:" << std::endl;
    wordFrequencyExample();

    return 0;
}