#ifndef QUEUE_H
#define QUEUE_H

#include <vector>
#include <stdexcept>

template<typename T>
class Queue {
private:
    std::vector<T> data;
    size_t frontIndex;

public:
    Queue() : frontIndex(0) {}

    void enqueue(const T& item) {
        data.push_back(item);
    }

    T dequeue() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        T item = data[frontIndex];
        frontIndex++;
        if (frontIndex >= data.size() / 2) {
            data.erase(data.begin(), data.begin() + frontIndex);
            frontIndex = 0;
        }
        return item;
    }

    T& front() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data[frontIndex];
    }

    const T& front() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data[frontIndex];
    }

    bool empty() const {
        return frontIndex == data.size();
    }

    size_t size() const {
        return data.size() - frontIndex;
    }
};

#endif