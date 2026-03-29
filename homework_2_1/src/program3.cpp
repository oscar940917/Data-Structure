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