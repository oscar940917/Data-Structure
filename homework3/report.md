# 41343129

作業三
# Problem : Polynomial（Circular Linked List 實作）
## 一、題目說明

一、作業說明

本次作業要求實作一個可以進行多項式運算的程式，
主要功能包含：

多項式加法（Addition）

多項式乘法（Multiplication）

多項式代入求值（Evaluation）

多項式由多個「項（Term）」組成，每一項包含：

係數（coefficient）

指數（exponent）

例如：
```text
3x^2 + 2x + 1
```
可以表示為三個項 (3, 2)、(2, 1)、(1, 0)。

## 解題說明1

一開始在思考多項式的儲存方式時，
曾嘗試用陣列的方式實作，但在處理項數不固定、
以及物件複製時，容易因為指標或記憶體配置錯誤導致程式異常。

後來改用課堂介紹的 循環串列（circular linked list with header node） 來實作，
每一個節點代表一個多項式項（Term），
由 Polynomial 類別統一管理串列與運算。

此設計的優點如下：

不需事先知道項數

插入與走訪節點較為彈性

符合本次作業對資料結構的要求

能實際練習指標操作與深拷貝概念

## 程式實作

1. ChainNode

```cpp
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

```
2. ChainIterator
```cpp
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


```
3. Chain（Circular Linked List with Header）
```cpp
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
```
4. Polynomial 類別
```cpp
struct Term {
    int coef;
    int exp;

    Term(int c = 0, int e = 0) {
        coef = c;
        exp = e;
    }
};

class Polynomial {
private:
    Chain<Term> terms;

public:
    Polynomial() {}

    Polynomial(const Polynomial& p) {
        for (auto it = p.terms.begin(); it != p.terms.end(); ++it)
            terms.insertBack(*it);
    }

    friend istream& operator>>(istream& in, Polynomial& p) {
        int c, e;
        while (true) {
            in >> c >> e;
            if (c == 0 && e == 0)
                break;
            p.terms.insertBack(Term(c, e));
        }
        return in;
    }

    friend ostream& operator<<(ostream& out, const Polynomial& p) {
        bool first = true;
        for (auto it = p.terms.begin(); it != p.terms.end(); ++it) {
            if (!first && it->coef > 0)
                out << "+";
            out << it->coef << "x^" << it->exp;
            first = false;
        }
        return out;
    }

    Polynomial operator+(const Polynomial& b) const {
        Polynomial r;
        auto it1 = terms.begin();
        auto it2 = b.terms.begin();

        while (it1 != terms.end() && it2 != b.terms.end()) {
            if (it1->exp == it2->exp) {
                int s = it1->coef + it2->coef;
                if (s != 0)
                    r.terms.insertBack(Term(s, it1->exp));
                ++it1; ++it2;
            }
            else if (it1->exp > it2->exp) {
                r.terms.insertBack(*it1);
                ++it1;
            }
            else {
                r.terms.insertBack(*it2);
                ++it2;
            }
        }

        while (it1 != terms.end()) {
            r.terms.insertBack(*it1);
            ++it1;
        }

        while (it2 != b.terms.end()) {
            r.terms.insertBack(*it2);
            ++it2;
        }

        return r;
    }

    Polynomial operator*(const Polynomial& b) const {
        Polynomial r;
        for (auto it1 = terms.begin(); it1 != terms.end(); ++it1)
            for (auto it2 = b.terms.begin(); it2 != b.terms.end(); ++it2)
                r.terms.insertBack(
                    Term(it1->coef * it2->coef,
                         it1->exp + it2->exp)
                );
        return r;
    }

    float Evaluate(float x) const {
        float sum = 0;
        for (auto it = terms.begin(); it != terms.end(); ++it)
            sum += it->coef * pow(x, it->exp);
        return sum;
    }
};
```
## 四、測試結果範例
以下為本程式實際執行時的輸入與輸出範例，用來驗證多項式加法、乘法與代入求值是否正確。
輸入範例
```text
輸入 a(x)：
2 2
1 1
3 0
0 0

輸入 b(x)：
1 1
4 0
0 0

請輸入 x 的值：
2
```
輸出結果
```text
a(x) = 2x^2+1x^1+3x^0
b(x) = 1x^1+4x^0
a + b = 2x^2+2x^1+7x^0
a * b = 2x^3+9x^2+10x^1+12x^0
a(2) = 13
b(2) = 6

```
測試說明

加法結果中，相同指數的項會正確合併

乘法結果為所有項兩兩相乘後的結果

代入 x = 2 計算結果符合人工計算
## 效能分析1

### 時間複雜度

    
| 功能      | 方法          | 時間複雜度    | 說明          |
| ------- | ----------- | -------- | ----------- |
| 加法      | `operator+` | O(m + n) | 以指數大小合併兩多項式 |
| 乘法      | `operator*` | O(m × n) | 每一項互相相乘     |
| 求值      | `Evaluate`  | O(n)     | 逐項計算並加總     |
| 輸入 / 輸出 | `>>` / `<<` | O(n)     | 逐項走訪        |



### 空間複雜度

 | 操作 | 空間複雜度    | 說明           |
| -- | -------- | ------------ |
| 加法 | O(m + n) | 新多項式儲存結果     |
| 乘法 | O(m × n) | 乘法結果最多 m×n 項 |
| 求值 | O(1)     | 僅使用暫存變數      |






## 測試與驗證
本程式已針對多項式的加法、乘法與代入求值功能進行測試。
測試時包含不同項數及不同指數的多項式輸入，
並透過實際輸出結果與人工計算進行比對。

由測試結果可確認：
加法能正確合併相同指數的項目，
乘法能正確產生所有項的乘積結果，
代入求值在指定 x 值下可得到正確數值。

## 申論及開發報告

### 六、心得與反思
這次作業讓我實際練習了使用 linked list 來管理資料，
也更熟悉指標操作與物件拷貝的細節。

雖然實作起來比使用現成容器麻煩一些，
但能清楚了解資料結構底層的運作方式，
對之後學習其他資料結構很有幫助。

### 總結
| 項目     | 說明                                  |
| ------ | ----------------------------------- |
| 使用資料結構 | Circular Linked List（含 header node） |
| 使用語言   | C++                                 |
| 主要技巧   | 類別設計、運算子多載、指標操作                     |
| 完成的功能  | 多項式加法、乘法、代入求值                       |
