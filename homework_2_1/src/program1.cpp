#include <iostream>
#include <algorithm>
#include <cmath>

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
        return max(height(node->left), height(node->right)) + 1;
    }
};

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
        header = new PolyNode(0, -1);
        header->next = header;
    }

    void insertTerm(int c, int e) {
        if (c == 0) return;
        PolyNode* prev = header;
        PolyNode* curr = header->next;
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

int main() {
    MinHeap<int> mh(10);
    mh.Push(30);
    mh.Push(10);
    mh.Push(20);
    cout << "MinHeap Top: " << mh.Top() << endl;

    BST tree;
    tree.root = tree.insert(tree.root, 50);
    tree.root = tree.insert(tree.root, 30);
    tree.root = tree.insert(tree.root, 70);
    cout << "BST Height: " << tree.height(tree.root) << endl;

    PolyList poly;
    poly.insertTerm(3, 2);
    poly.insertTerm(5, 1);
    poly.insertTerm(2, 2);
    cout << "Polynomial: ";
    poly.display();

    return 0;
}