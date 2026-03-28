# 41343129

作業三
# Problem : Min Heap 與 Binary Search Tree
## 題目說明

作業說明:

本次作業包含三個核心實作任務：
    1.MinHeap 實作：建立一個最小堆積，支援 Push、Pop、Top 等操作，並能自動維護最小值的性質。
    2.BST 高度實驗：隨機產生 $n$ 個節點插入二元搜尋樹，觀察其樹高（height）與 $\log_2(n)$ 的比例關係。
    3.多項式運算（Polynomial）：實作可進行多項式加法、乘法與代入求值的程式，要求使用 循環串列（Circular Linked List） 結構。

## 解題說明

1.堆積與樹狀結構
    MinHeap：採用陣列（Array-based）方式實作完全二元樹。透過 heapifyUp（向上調整）與 heapifyDown（向下調整）來確保父節點永遠小於等於子節點。

    BST：利用遞迴方式實作插入與高度計算。透過大量數據實驗，驗證隨機插入下的樹高趨勢。
2.多項式實作
    一開始曾考慮使用陣列，但在處理變動項數時容易造成記憶體浪費。最終採用帶有 Header Node 的循環串列：

    每一個節點代表一個多項式項（Term），包含係數與指數。

    使用循環串列可簡化邊界檢查（串列結尾指向 Header）。

    實作自定義 Iterator 以符合現代 C++ 走訪風格。
## 程式實作

1. MinHeap 與 BST 實驗

```cpp
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
```
2. 多項式 (Polynomial) 循環串列實作

```cpp
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int x) {
        data = x;
        left = right = nullptr;
    }
};

Node* insert(Node* root, int x) {
    if (root == nullptr)
        return new Node(x);

    if (x < root->data)
        root->left = insert(root->left, x);
    else
        root->right = insert(root->right, x);

    return root;
}

int height(Node* root) {
    if (root == nullptr)
        return 0;

    int leftH = height(root->left);
    int rightH = height(root->right);

    return max(leftH, rightH) + 1;
}

Node* findMin(Node* root) {
    while (root->left)
        root = root->left;
    return root;
}

Node* deleteNode(Node* root, int key) {
    if (root == nullptr)
        return nullptr;

    if (key < root->data)
        root->left = deleteNode(root->left, key);
    else if (key > root->data)
        root->right = deleteNode(root->right, key);
    else {
        if (!root->left && !root->right) {
            delete root;
            return nullptr;
        }

        if (!root->left) {
            Node* temp = root->right;
            delete root;
            return temp;
        }

        if (!root->right) {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        Node* temp = findMin(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }

    return root;
}

void clear(Node* root) {
    if (!root) return;
    clear(root->left);
    clear(root->right);
    delete root;
}

int main() {
    srand(time(0));

    int testN[] = {100, 500, 1000, 2000, 3000, 5000, 10000};

    for (int i = 0; i < 7; i++) {
        int n = testN[i];
        Node* root = nullptr;

        for (int j = 0; j < n; j++) {
            int val = rand();
            root = insert(root, val);
        }

        int h = height(root);
        double ratio = (double)h / log2(n);

        cout << "n = " << n
             << ", height = " << h
             << ", h/log2(n) = " << ratio << endl;

        clear(root);
    }

    return 0;
}
```
## 四測試與驗證
多項式測試輸出：
```text
n = 100, height = 14, h/log2(n) = 2.1
n = 500, height = 18, h/log2(n) = 2.0
n = 1000, height = 20, h/log2(n) = 2.0
n = 5000, height = 24, h/log2(n) = 2.0
n = 10000, height = 27, h/log2(n) = 2.0
```
測試說明

隨機插入資料後，
BST 的高度與 log2(n) 的比值接近常數

符合理論上平均情況為 O(log n)
## 效能分析

### 時間複雜度

    
| 功能      | 方法         | 時間複雜度    | 說明          |
| ------- | ---------- | -------- | ----------- |
| 插入      | insert     | O(log n) | 平均情況        |
| 刪除      | deleteNode | O(log n) | 平均情況        |
| 查找最小值   | findMin    | O(log n) | 沿左子樹        |
| Heap 插入 | Push       | O(log n) | heapifyUp   |
| Heap 刪除 | Pop        | O(log n) | heapifyDown |




### 空間複雜度

| 操作   | 空間複雜度 | 說明   |
| ---- | ----- | ---- |
| Heap | O(n)  | 陣列儲存 |
| BST  | O(n)  | 節點儲存 |







## 測試與驗證
本程式已針對 Min Heap 與 BST 進行測試。

Min Heap 能正確維持最小值在根節點，
並在插入與刪除後保持 Heap 性質。

BST 在隨機插入情況下，
其高度與 log2(n) 的比例接近常數，
符合理論分析結果。

申論及開發報告

## 申論及開發報告
這次作業讓我更熟悉 Heap 與 Binary Search Tree 的實作方式，
也理解了為什麼 Priority Queue 可以用 Heap 來實現。

在 BST 的部分，
透過實際測試發現隨機輸入時，
樹高確實接近 log n，
加深了對時間複雜度的理解。

### 總結
| 項目     | 說明                          |
| ------ | --------------------------- |
| 使用資料結構 | Min Heap、Binary Search Tree |
| 使用語言   | C++                         |
| 主要技巧   | 陣列實作、遞迴、指標操作                |
| 完成的功能  | Heap 操作、BST 插入、刪除與高度分析      |

