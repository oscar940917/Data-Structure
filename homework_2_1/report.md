# 41343129

作業三
# Problem : Min Heap 與 Binary Search Tree
## 題目說明

作業說明如下:

本次作業分為兩個部分：

第一部分為 Min Heap（最小優先佇列），
需實作一個 MinHeap 類別，並支援以下操作：

IsEmpty（判斷是否為空）

Top（回傳最小值）

Push（插入元素）

Pop（刪除最小元素）

且需符合 Priority Queue 的時間複雜度要求。

第二部分為 Binary Search Tree（BST），
主要包含兩個子題：

(a) 建立一棵空的 BST，並進行隨機插入，
計算樹高（height），並輸出：

height / log2(n)

測試 n = 100, 500, 1000, ..., 10000，
觀察該比值是否接近常數（約為 2）。

(b) 實作刪除節點的函式，
並分析其時間複雜度。


## 解題說明

在本次作業中，主要練習兩種資料結構：
Heap 與 Binary Search Tree。

在 Min Heap 的部分，
一開始曾考慮使用 STL（例如 vector），
但因題目限制不可使用，
因此改採用「陣列」來實作完全二元樹。

透過 index 的關係來表示父子節點：

parent = (i - 1) / 2  
left = 2 * i + 1  
right = 2 * i + 2  

並透過 heapifyUp 與 heapifyDown
維持最小堆積（Min Heap）的性質。

在 Binary Search Tree 的部分，
使用遞迴方式進行插入與高度計算，
並透過隨機數測試樹的平均高度。

刪除節點則依照三種情況處理：

無子節點

單一子節點

兩個子節點（使用右子樹最小值取代）


## 程式實作

1. MinHeap 類別（陣列實作）

```cpp
class MinHeap {
private:
    int heap[100000];
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
        return heap[0];
    }

    void Push(int x) {
        heap[size] = x;
        heapifyUp(size);
        size++;
    }

    void Pop() {
        heap[0] = heap[size - 1];
        size--;
        heapifyDown(0);
    }
};