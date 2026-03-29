#include <iostream>

using namespace std;

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
        if (temp == header) {
            cout << "0" << endl;
            return;
        }
        while (temp != header) {
            cout << temp->coef << "x^" << temp->exp;
            if (temp->next != header) cout << " + ";
            temp = temp->next;
        }
        cout << endl;
    }
};

int main() {
    PolyList poly;
    poly.insertTerm(3, 2);
    poly.insertTerm(4, 5);
    poly.insertTerm(2, 2); 
    poly.insertTerm(-1, 0);

    cout << "Polynomial Expression: ";
    poly.display();

    return 0;
}