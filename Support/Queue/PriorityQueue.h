#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdexcept>
#include <vector>
#include <functional>
#include <algorithm>


template<typename T, typename Compare = std::less<T>>
class PriorityQueue {
private:
    std::vector<T> heap;
    Compare comp;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (comp(heap[parent], heap[index])) {
                std::swap(heap[parent], heap[index]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int leftChild = 2 * index + 1;
            int rightChild = 2 * index + 2;
            int largest = index;

            if (leftChild < size && comp(heap[largest], heap[leftChild])) {
                largest = leftChild;
            }
            if (rightChild < size && comp(heap[largest], heap[rightChild])) {
                largest = rightChild;
            }

            if (largest != index) {
                std::swap(heap[index], heap[largest]);
                index = largest;
            } else {
                break;
            }
        }
    }

public:
    PriorityQueue() : comp(Compare()) {}
    explicit PriorityQueue(const Compare& compare) : comp(compare) {}

    void push(const T& value) {
        heap.push_back(value);
        heapifyUp(heap.size() - 1);
    }

    void pop() {
        if (empty()) return;
        heap[0] = heap.back();
        heap.pop_back();
        if (!empty()) {
            heapifyDown(0);
        }
    }

    const T& top() const {
        if (empty()) throw std::out_of_range("PriorityQueue is empty");
        return heap[0];
    }

    bool empty() const {
        return heap.empty();
    }

    size_t size() const {
        return heap.size();
    }

    void clear() {
        heap.clear();
    }
};

#endif // PRIORITY_QUEUE_H