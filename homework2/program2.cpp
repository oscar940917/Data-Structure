#include <iostream>
using namespace std;

class Polynomial;  // forward declaration

class Term {
    friend class Polynomial;                       // Polynomial 是友元
    friend ostream& operator<<(ostream&, const Polynomial&); // operator<< 是友元
private:
    float coef;
    int exp;
public:
    Term() : coef(0), exp(0) {}
    Term(float c, int e) : coef(c), exp(e) {}
};

class Polynomial {
private:
    Term *termArray;
    int capacity;
    int terms;

public:
    Polynomial(int cap = 10) : capacity(cap), terms(0) {
        termArray = new Term[capacity];
    }

    ~Polynomial() {
        delete[] termArray;
    }

    // 輸入運算子 >>
    friend istream& operator>>(istream& in, Polynomial& p) {
        int n;
        cout << "請輸入多項式的項數: ";
        in >> n;

        if (n > p.capacity) {  // 若項數大於容量，重新配置
            delete[] p.termArray;
            p.capacity = n;
            p.termArray = new Term[p.capacity];
        }

        p.terms = n;
        for (int i = 0; i < n; ++i) {
            float coef;
            int exp;
            cout << "請輸入第 " << i + 1 << " 項 (係數 指數): ";
            in >> coef >> exp;
            p.termArray[i] = Term(coef, exp);
        }
        return in;
    }

    // 輸出運算子 <<
    friend ostream& operator<<(ostream& out, const Polynomial& p) {
        if (p.terms == 0) {
            out << "0";
            return out;
        }

        bool first = true;
        for (int i = 0; i < p.terms; ++i) {
            float coef = p.termArray[i].coef;
            int exp = p.termArray[i].exp;

            if (coef == 0) continue;

            // 處理符號
            if (first) {
                if (coef < 0) out << "-";
                first = false;
            } else {
                if (coef > 0) out << " + ";
                else out << " - ";
            }

            float absCoef = (coef < 0) ? -coef : coef;

            // 決定係數輸出
            if (absCoef != 1 || exp == 0) out << absCoef;

            // 指數輸出
            if (exp > 0) {
                if (absCoef != 1) out << "x";
                else out << "x";
                if (exp > 1) out << "^" << exp;
            }
        }
        return out;
    }
};

int main() {
    Polynomial p;
    cin >> p;
    cout << "輸入的多項式是: " << p << endl;
    return 0;
}
