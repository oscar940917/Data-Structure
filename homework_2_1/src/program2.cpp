#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>

using namespace std;

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

    TreeNode* remove(TreeNode* node, int k) {
        if (node == NULL) return NULL;
        if (k < node->key) node->left = remove(node->left, k);
        else if (k > node->key) node->right = remove(node->right, k);
        else {
            if (node->left == NULL) {
                TreeNode* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == NULL) {
                TreeNode* temp = node->left;
                delete node;
                return temp;
            }
            TreeNode* temp = node->right;
            while (temp->left != NULL) temp = temp->left;
            node->key = temp->key;
            node->right = remove(node->right, temp->key);
        }
        return node;
    }
};

int main() {
    BST tree;
    int data[] = {50, 30, 70, 20, 40, 60, 80};
    for(int x : data) tree.root = tree.insert(tree.root, x);

    cout << "BST Height: " << tree.height(tree.root) << endl;
    
    cout << "Removing node 30..." << endl;
    tree.root = tree.remove(tree.root, 30);
    cout << "New BST Height: " << tree.height(tree.root) << endl;

    return 0;
}