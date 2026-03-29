#include <iostream>
#include <algorithm>

using namespace std;


template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T& x) = 0;
    virtual void Pop() = 0;
};


template <class T>
class MinHeap : public MinPQ<T> {
private:
    T* heap;
    int capacity;
    int heapSize;


    void filterUp(int start) {
        int curr = start;
        int parent = (curr - 1) / 2;
        T temp = heap[curr];
        while (curr > 0) {
            if (heap[parent] <= temp) break;
            heap[curr] = heap[parent];
            curr = parent;
            parent = (curr - 1) / 2;
        }
        heap[curr] = temp;
    }


    void filterDown(int start, int end) {
        int curr = start;
        int leftChild = 2 * curr + 1;
        T temp = heap[curr];
        while (leftChild <= end) {
            if (leftChild < end && heap[leftChild] > heap[leftChild + 1])
                leftChild++; 
            if (temp <= heap[leftChild]) break;
            heap[curr] = heap[leftChild];
            curr = leftChild;
            leftChild = 2 * curr + 1;
        }
        heap[curr] = temp;
    }

public:
    MinHeap(int cap = 100) {
        capacity = cap;
        heap = new T[capacity];
        heapSize = 0;
    }


    ~MinHeap() {
        delete[] heap;
    }

    bool IsEmpty() const override { return heapSize == 0; }

    const T& Top() const override {
        return heap[0];
    }

    void Push(const T& x) override {
        if (heapSize == capacity) return; 
        heap[heapSize] = x;
        filterUp(heapSize);
        heapSize++;
    }

    void Pop() override {
        if (IsEmpty()) return;
        heap[0] = heap[heapSize - 1];
        heapSize--;
        filterDown(0, heapSize - 1);
    }
};