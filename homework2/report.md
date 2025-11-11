# 41343129

作業二
# Problem : Polynomial
## 一、題目說明

這次作業要我們設計一個能夠做多項式運算的程式，
包含三個主要功能：

加法（Add）

乘法（Multiply）

代入求值（Evaluate）

多項式是由很多「項」組成的，每一項都有「係數（coefficient）」和「指數（exponent）」。
像是 3x^2 + 2x + 1，其實就是三個項組成的。
這題的重點在於，讓我們用「物件導向」的方式把這些項管理起來，
同時練習 C++ 的類別設計與運算子多載。

## 二、解題想法

我一開始是想照課本那種 Term 陣列的做法寫，
但試著跑的時候常常出現 Process finished with exit code 1，
後來發現是因為記憶體越界或複製物件時沒做好深拷貝。

所以我乾脆改成用 vector，讓 C++ 自己幫我管理記憶體，
也比較彈性，不用擔心項數太多。
整體架構大概分三部分：

Term（結構） → 存每一項的係數跟指數

Polynomial（類別） → 負責所有多項式的操作

main() → 讓使用者輸入 a(x)、b(x)，然後輸出結果

## 程式實作

以下為主程式

```cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

struct Term {
    double coef;
    int exp;
    Term(double c = 0, int e = 0) : coef(c), exp(e) {}
};

// 多項式類別
class Polynomial {
private:
    vector<Term> terms;

    // 自動合併同指數項
    void simplify() {
        sort(terms.begin(), terms.end(), [](const Term& a, const Term& b) {
            return a.exp > b.exp;
        });

        vector<Term> result;
        for (auto& t : terms) {
            if (!result.empty() && result.back().exp == t.exp)
                result.back().coef += t.coef;
            else
                result.push_back(t);
        }

        // 移除係數為 0 的項
        terms.clear();
        for (auto& t : result)
            if (fabs(t.coef) > 1e-6)
                terms.push_back(t);
    }

public:
    // 讀入運算子
    friend istream& operator>>(istream& in, Polynomial& p) {
        int n;
        cout << "請輸入項數：";
        in >> n;
        p.terms.clear();
        for (int i = 0; i < n; i++) {
            double c; int e;
            cout << "輸入第 " << i + 1 << " 項 (係數 指數)：";
            in >> c >> e;
            p.terms.push_back(Term(c, e));
        }
        p.simplify();
        return in;
    }

    // 輸出運算子
    friend ostream& operator<<(ostream& out, const Polynomial& p) {
        if (p.terms.empty()) {
            out << "0";
            return out;
        }

        for (int i = 0; i < p.terms.size(); i++) {
            double c = p.terms[i].coef;
            int e = p.terms[i].exp;

            if (i > 0 && c > 0) out << " + ";
            else if (c < 0) out << " - ";

            out << fabs(c);
            if (e > 0) out << "x^" << e;
        }
        return out;
    }

    // 多項式加法
    Polynomial operator+(const Polynomial& b) const {
        Polynomial r = *this;
        for (auto& t : b.terms)
            r.terms.push_back(t);
        r.simplify();
        return r;
    }

    // 多項式乘法
    Polynomial operator*(const Polynomial& b) const {
        Polynomial r;
        for (auto& aTerm : terms)
            for (auto& bTerm : b.terms)
                r.terms.push_back(Term(aTerm.coef * bTerm.coef, aTerm.exp + bTerm.exp));
        r.simplify();
        return r;
    }

    // 代入求值
    double evaluate(double x) const {
        double sum = 0;
        for (auto& t : terms)
            sum += t.coef * pow(x, t.exp);
        return sum;
    }
};

int main() {
    Polynomial a, b;

    cout << "輸入 a(x)：\n";
    cin >> a;
    cout << "輸入 b(x)：\n";
    cin >> b;

    Polynomial sum = a + b;
    Polynomial prod = a * b;

    double x;
    cout << "請輸入 x 的值：";
    cin >> x;

    cout << "\n====== 結果 ======\n";
    cout << "a(x) = " << a << endl;
    cout << "b(x) = " << b << endl;
    cout << "a + b = " << sum << endl;
    cout << "a * b = " << prod << endl;
    cout << "a(" << x << ") = " << a.evaluate(x) << endl;
    cout << "b(" << x << ") = " << b.evaluate(x) << endl;
}



```
## 四、測試結果範例
```scss
輸入 a(x)：
請輸入項數：3
輸入第 1 項 (係數 指數)：2 2
輸入第 2 項 (係數 指數)：1 1
輸入第 3 項 (係數 指數)：3 0

輸入 b(x)：
請輸入項數：2
輸入第 1 項 (係數 指數)：1 1
輸入第 2 項 (係數 指數)：4 0

請輸入 x 的值：2

```

## 五、效能分析

### 時間複雜度

    
| 函式                          | 功能    | 時間複雜度        | 說明                          |
| --------------------------- | ----- | ------------ | --------------------------- |
| `Add(const Polynomial& b)`  | 多項式加法 | **O(m + n)** | 逐項比較兩多項式（m、n 為項數），一次線性合併。   |
| `Mult(const Polynomial& b)` | 多項式乘法 | **O(m × n)** | 每一項都與另一個多項式的每一項相乘。          |
| `Eval(float x)`             | 多項式求值 | **O(n)**     | 逐項計算 ( c_i × x^{e_i} ) 後相加。 |
| `operator>>` / `operator<<` | 輸入輸出  | **O(n)**     | 逐項讀寫資料。                     |



### 空間複雜度

   | 操作 / 函式                   | 空間複雜度    | 說明                                         |
|--------------------------------|--------------|--------------------------------------------|
| `Add(const Polynomial& b)` / 加法  | O(m + n)     | 輸出結果需要新多項式存放所有項。           |
| `Mult(const Polynomial& b)` / 乘法 | O(m × n)     | 乘法結果可能有最多 m×n 項（未合併前）。   |
| `Eval(float x)` / 評估           | O(1)         | 只需一個暫存變數存結果。                  |
| `operator>>` / `operator<<` / 輸入輸出 | O(n)      | 逐項存取讀寫資料。                         |





## 測試與驗證


## 申論及開發報告

### 六、心得與反思

這次作業讓我重新體會到 C++ 記憶體管理的重要性。
一開始我用 new 去開陣列，結果一不小心就爆掉（exit code 1），
後來才學到用 vector 真的省很多麻煩。

我也覺得運算子多載很有趣，
它讓我們能用 a + b、a * b 這種方式直接操作物件，
寫起來真的比較「像數學」。

整體來說，這題的程式讓我更熟悉物件導向、vector 的使用方式，
也更了解程式架構設計的重要性。
我覺得雖然內容不難，但能練習邏輯思考與設計能力，是滿實用的一題。

### 總結

| 功能    | 主要概念     | 學到的東西            |
| ----- | -------- | ---------------- |
| 多項式運算 | 類別與運算子多載 | 怎麼讓物件像變數一樣運算     |
| 加法與乘法 | 比對指數合併項  | 學到如何用排序與合併處理資料   |
| 代入求值  | pow() 計算 | 實作出能帶入變數的多項式     |
| 整體架構  | 物件導向思維   | 程式設計不只是能跑，而是要有結構 |
