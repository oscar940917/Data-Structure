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