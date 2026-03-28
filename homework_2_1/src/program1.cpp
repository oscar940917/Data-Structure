#include <iostream>
using namespace std;

class MinHeap {
private:
    int heap[100000]; // 固定大小陣列
    int size;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[parent] > heap[index]) {
                swap(heap[parent], heap[index]);
                index = parent;
            } else break;
        }
    }

    void heapifyDown(int index) {
        while (2 * index + 1 < size) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = left;

            if (right < size && heap[right] < heap[left])
                smallest = right;

            if (heap[index] > heap[smallest]) {
                swap(heap[index], heap[smallest]);
                index = smallest;
            } else break;
        }
    }

public:
    MinHeap() {
        size = 0;
    }

    bool IsEmpty() {
        return size == 0;
    }

    int Top() {
        if (IsEmpty()) {
            cout << "Heap is empty!\n";
            return -1;
        }
        return heap[0];
    }

    void Push(int x) {
        heap[size] = x;
        heapifyUp(size);
        size++;
    }

    void Pop() {
        if (IsEmpty()) {
            cout << "Heap is empty!\n";
            return;
        }

        heap[0] = heap[size - 1];
        size--;
        heapifyDown(0);
    }
};

int main() {
    MinHeap h;

    h.Push(10);
    h.Push(5);
    h.Push(20);
    h.Push(3);

    cout << "Top: " << h.Top() << endl;

    h.Pop();
    cout << "Top after pop: " << h.Top() << endl;

    return 0;
}