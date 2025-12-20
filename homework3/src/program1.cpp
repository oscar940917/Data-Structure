#include <iostream>
#include <cmath>
using namespace std;

/* ========== ChainNode ========== */
template<class T>
class ChainNode {
public:
    T element;
    ChainNode<T>* next;

    ChainNode() {
        next = nullptr;
    }

    ChainNode(const T& e, ChainNode<T>* n = nullptr) {
        element = e;
        next = n;
    }
};

/* ========== ChainIterator ========== */
template<class T>
class ChainIterator {
private:
    ChainNode<T>* current;

public:
    ChainIterator(ChainNode<T>* start = nullptr) {
        current = start;
    }

    T& operator*() const {
        return current->element;
    }

    T* operator->() const {
        return &(current->element);
    }

    ChainIterator& operator++() {
        current = current->next;
        return *this;
    }

    bool operator!=(const ChainIterator& other) const {
        return current != other.current;
    }
};

/* ========== Chain (circular list with header) ========== */
template<class T>
class Chain {
private:
    ChainNode<T>* head;

public:
    Chain() {
        head = new ChainNode<T>();
        head->next = head;
    }

    ~Chain() {
        ChainNode<T>* p = head->next;
        while (p != head) {
            ChainNode<T>* t = p;
            p = p->next;
            delete t;
        }
        delete head;
    }

    ChainNode<T>* getHead() const {
        return head;
    }

    void insertBack(const T& x) {
        ChainNode<T>* p = head;
        while (p->next != head)
            p = p->next;
        p->next = new ChainNode<T>(x, head);
    }

    ChainIterator<T> begin() const {
        return ChainIterator<T>(head->next);
    }

    ChainIterator<T> end() const {
        return ChainIterator<T>(head);
    }
};

/* ========== Term ========== */
struct Term {
    int coef;
    int exp;

    Term(int c = 0, int e = 0) {
        coef = c;
        exp = e;
    }
};

/* ========== Polynomial ========== */
class Polynomial {
private:
    Chain<Term> terms;

public:
    Polynomial() {}

    Polynomial(const Polynomial& p) {
        for (auto it = p.terms.begin(); it != p.terms.end(); ++it)
            terms.insertBack(*it);
    }

    Polynomial& operator=(const Polynomial& p) {
        if (this != &p) {
            this->~Polynomial();
            new(this) Polynomial(p);
        }
        return *this;
    }

    friend istream& operator>>(istream& is, Polynomial& p) {
        int c, e;
        while (true) {
            is >> c >> e;
            if (c == 0 && e == 0)
                break;
            p.terms.insertBack(Term(c, e));
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const Polynomial& p) {
        bool first = true;
        for (auto it = p.terms.begin(); it != p.terms.end(); ++it) {
            if (!first && it->coef > 0)
                os << "+";
            os << it->coef << "x^" << it->exp;
            first = false;
        }
        return os;
    }

    float Evaluate(float x) const {
        float ans = 0;
        for (auto it = terms.begin(); it != terms.end(); ++it) {
            ans += it->coef * pow(x, it->exp);
        }
        return ans;
    }

    Polynomial operator+(const Polynomial& b) const {
        Polynomial c;
        auto it1 = terms.begin();
        auto it2 = b.terms.begin();

        while (it1 != terms.end() && it2 != b.terms.end()) {
            if (it1->exp == it2->exp) {
                int sum = it1->coef + it2->coef;
                if (sum != 0)
                    c.terms.insertBack(Term(sum, it1->exp));
                ++it1;
                ++it2;
            }
            else if (it1->exp > it2->exp) {
                c.terms.insertBack(*it1);
                ++it1;
            }
            else {
                c.terms.insertBack(*it2);
                ++it2;
            }
        }

        while (it1 != terms.end()) {
            c.terms.insertBack(*it1);
            ++it1;
        }

        while (it2 != b.terms.end()) {
            c.terms.insertBack(*it2);
            ++it2;
        }

        return c;
    }

    Polynomial operator-(const Polynomial& b) const {
        Polynomial c;
        auto it1 = terms.begin();
        auto it2 = b.terms.begin();

        while (it1 != terms.end() && it2 != b.terms.end()) {
            if (it1->exp == it2->exp) {
                int diff = it1->coef - it2->coef;
                if (diff != 0)
                    c.terms.insertBack(Term(diff, it1->exp));
                ++it1;
                ++it2;
            }
            else if (it1->exp > it2->exp) {
                c.terms.insertBack(*it1);
                ++it1;
            }
            else {
                c.terms.insertBack(Term(-it2->coef, it2->exp));
                ++it2;
            }
        }

        while (it1 != terms.end()) {
            c.terms.insertBack(*it1);
            ++it1;
        }

        while (it2 != b.terms.end()) {
            c.terms.insertBack(Term(-it2->coef, it2->exp));
            ++it2;
        }

        return c;
    }
};

/* ========== main (測試用，可留可刪) ========== */
int main() {
    Polynomial p1, p2;

    cout << "Input polynomial 1 (coef exp ... 0 0): ";
    cin >> p1;

    cout << "Input polynomial 2 (coef exp ... 0 0): ";
    cin >> p2;

    cout << "P1 = " << p1 << endl;
    cout << "P2 = " << p2 << endl;

    Polynomial p3 = p1 + p2;
    cout << "P1 + P2 = " << p3 << endl;

    cout << "P1(2) = " << p1.Evaluate(2) << endl;

    return 0;
}
