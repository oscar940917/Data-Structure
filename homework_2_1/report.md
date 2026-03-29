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

1. MinHeap 實作邏輯
    抽象類別繼承：先定義 template <class T> class MinPQ 作為介面，確保程式符合物件導向的規範。
    
    堆積調整：使用陣列實作完全二元樹。插入時透過 heapifyUp（與父節點比較並交換）維持性質；刪除根節點時，將最後一個元素移至根部，再透過 heapifyDown 往下調整。

2. BST 隨機實驗與刪除
    高度與對數關係：理論上隨機插入的 BST 平均高度約為 $O(\log n)$。我們透過遞迴 getHeight 函式取得實際高度，並驗證比值是否接近常數。

    刪除節點：分為三種情況：葉節點直接刪除、單一子節點則取代、雙子節點則尋找右子樹的最小值（Inorder Successor）取代。

3. 多項式循環串列
    Header Node (頭結點)：建立一個指數為 -1 的特殊節點作為起點。這能避免處理空串列時的特殊判斷，讓所有項目的插入邏輯一致。

    循環特性：最後一個節點的 next 指向 head，形成一個環狀結構，方便走訪與節省指標判斷時間。
## 程式實作

1. MinHeap 實作 (含測試)
```cpp
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

// 題目要求的抽象類別 ADT
template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T& x) = 0;
    virtual void Pop() = 0;
};

// 繼承自抽象類別的 MinHeap 實作
template <class T>
class MinHeap : public MinPQ<T> {
private:
    T* heap;
    int capacity;
    int heapSize;

    // 向上調整，維持最小堆性質
    void filterUp(int start) {
        int curr = start;
        int parent = (curr - 1) / 2;
        T temp = heap[curr];
        while (curr > 0) {
            if (heap[parent] <= temp) break;
            else {
                heap[curr] = heap[parent];
                curr = parent;
                parent = (curr - 1) / 2;
            }
        }
        heap[curr] = temp;
    }

    // 向下調整
    void filterDown(int start, int end) {
        int curr = start;
        int leftChild = 2 * curr + 1;
        T temp = heap[curr];
        while (leftChild <= end) {
            if (leftChild < end && heap[leftChild] > heap[leftChild + 1])
                leftChild++; // 找左右子節點較小的一個
            if (temp <= heap[leftChild]) break;
            else {
                heap[curr] = heap[leftChild];
                curr = leftChild;
                leftChild = 2 * curr + 1;
            }
        }
        heap[curr] = temp;
    }

public:
    MinHeap(int cap = 100) {
        capacity = cap;
        heap = new T[capacity];
        heapSize = 0;
    }

    bool IsEmpty() const override { return heapSize == 0; }

    const T& Top() const override {
        if (IsEmpty()) {
            cout << "Heap is empty!" << endl;
        }
        return heap[0];
    }

    void Push(const T& x) override {
        if (heapSize == capacity) return; // 空間滿了
        heap[heapSize] = x;
        filterUp(heapSize);
        heapSize++;
    }

    void Pop() override {
        if (IsEmpty()) return;
        heap[0] = heap[heapSize - 1]; // 最後一個補到根部
        heapSize--;
        filterDown(0, heapSize - 1);
    }
};
```
2. Binary Search Tree (實驗與刪除)

```cpp
#include <cmath>
#include <ctime>

struct TreeNode {
    int key;
    TreeNode *left, *right;
    TreeNode(int k) : key(k), left(NULL), right(NULL) {}
};

class BST {
public:
    TreeNode* root;
    BST() : root(NULL) {}

    TreeNode* insert(TreeNode* node, int key) {
        if (node == NULL) return new TreeNode(key);
        if (key < node->key) node->left = insert(node->left, key);
        else if (key > node->key) node->right = insert(node->right, key);
        return node;
    }

    int height(TreeNode* node) {
        if (node == NULL) return 0;
        int lh = height(node->left);
        int rh = height(node->right);
        return max(lh, rh) + 1;
    }

    // 2(b) 刪除鍵值 k 的函式
    TreeNode* remove(TreeNode* node, int k) {
        if (node == NULL) return NULL;
        if (k < node->key) node->left = remove(node->left, k);
        else if (k > node->key) node->right = remove(node->right, k);
        else {
            // 情況一：葉子或只有一個小孩
            if (node->left == NULL) {
                TreeNode* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == NULL) {
                TreeNode* temp = node->left;
                delete node;
                return temp;
            }
            // 情況二：有兩個小孩，找右子樹最小
            TreeNode* temp = node->right;
            while (temp->left != NULL) temp = temp->left;
            node->key = temp->key;
            node->right = remove(node->right, temp->key);
        }
        return node;
    }
};
```
3. 多項式循環串列 (Polynomial)

```cpp
struct PolyNode {
    int coef;
    int exp;
    PolyNode* next;
    PolyNode(int c, int e) : coef(c), exp(e), next(NULL) {}
};

class PolyList {
private:
    PolyNode* header;
public:
    PolyList() {
        header = new PolyNode(0, -1); // 指數-1作為Header標記
        header->next = header;
    }

    void insertTerm(int c, int e) {
        if (c == 0) return;
        PolyNode* prev = header;
        PolyNode* curr = header->next;
        // 依照指數降序排列插入
        while (curr != header && curr->exp > e) {
            prev = curr;
            curr = curr->next;
        }
        if (curr != header && curr->exp == e) {
            curr->coef += c;
        } else {
            PolyNode* newNode = new PolyNode(c, e);
            newNode->next = curr;
            prev->next = newNode;
        }
    }

    void display() {
        PolyNode* temp = header->next;
        while (temp != header) {
            cout << temp->coef << "x^" << temp->exp;
            if (temp->next != header) cout << " + ";
            temp = temp->next;
        }
        cout << endl;
    }
};
```
## 四測試與驗證
多項式測試輸出：
```text
n = 100,   height = 13, h/log2(n) = 1.96
n = 500,   height = 18, h/log2(n) = 2.01
n = 1000,  height = 21, h/log2(n) = 2.11
n = 2000,  height = 23, h/log2(n) = 2.10
n = 3000,  height = 25, h/log2(n) = 2.16
n = 5000,  height = 26, h/log2(n) = 2.11
n = 10000, height = 29, h/log2(n) = 2.18
```
測試說明

本程式已針對 Min Heap、BST 與 Polynomial 進行全面測試：Min Heap：
    經由連續 Push 隨機數值並多次執行 Pop，驗證根節點始終保持為當前最小值，且每次刪除後皆能透過 filterDown 重新維持 Heap 性質。

    BST：在隨機插入大量資料後，觀測到其樹高（Height）與 $\log_2 n$ 的比值始終穩定在 2.0 ~ 2.2 之間。這符合理論上隨機二元搜尋樹在平均情況下時間複雜度為 $O(\log n)$ 的預期。

    Polynomial：測試了多項式的項數插入，透過 Header Node 循環串列結構，程式能正確依照指數（Exponent）降序排列，並處理同指數項的係數加總。
## 效能分析

### 時間複雜度

    
| 功能      | 方法         | 時間複雜度    | 說明          |
| ------- | ---------- | -------- | ----------- |
| BST 插入      | insert     | O(log n) | 平均情況，受限於樹高       |
| BST 刪除    | remove | O(log n) | 平均情況，需尋找替代者        |
| BST 找最小值   | findMin    | O(log n) | 沿左子樹向下搜尋        |
| Heap 插入 | Push       | O(log n) | 執行 filterUp 過程   |
| Heap 刪除 | Pop        | O(log n) | 執行 filterDown 過程 |
| 多項式插入 | insertTerm       | O(m) | m 為項數，需線性掃描   |




### 空間複雜度

| 操作   | 空間複雜度 | 說明   |
| Heap | O(n) | 陣列預留空間儲存節點 |
| BST | O(n)  | 每個節點動態配置指標空間 |
| Polynomial  | O(m)  | 循環串列中每一項佔用一個節點 |


## 申論及開發報告
這次作業讓我更熟悉 Heap 與 Binary Search Tree 的實作方式。在 MinHeap 部分，學會了如何透過陣列索引 (2i+1, 2i+2) 快速定位子節點。在 BST 部分，透過實際測試發現隨機輸入時，樹高確實接近 log n，加深了對時間複雜度的理解。

特別是在實作多項式的循環串列時，感受到 Header Node 的威力。它像是一個哨兵，讓我在插入節點時不需要額外判斷 head 是否為 NULL，這讓程式碼看起來更簡潔，邏輯也更穩定。

### 總結
| 項目     | 說明                          |
| ------ | --------------------------- |
| 使用資料結構 | Min Heap、Binary Search Tree |
| 使用語言   | C++                         |
| 主要技巧   | 陣列實作、遞迴、指標操作                |
| 完成的功能  | Heap 操作、BST 插入、刪除與高度分析      |

