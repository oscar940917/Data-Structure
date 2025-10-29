# 41343129

作業二
# Problem : Polynomial

## 解題說明

1. 資料結構設計：
    - 建立 Term 類別（包含係數與指數），並在 Polynomial 類別中用陣列儲存多項式的所有項。

2. 輸入與輸出：
    - 使用運算子多載 >>、<<，方便以數學式方式輸入與輸出多項式。

3. 加法（Add）：
    - 透過比較兩個多項式中每一項的指數，將指數相同的項合併，不同的項則直接加入結果。

4. 乘法（Mult）：
    - 對第一個多項式的每一項與第二個多項式的每一項相乘，
    - 若有相同指數則將係數相加，最後形成新的多項式結果。

5. 代入求值（Eval）：
    - 將輸入的 x 值代入每一項，計算coef * 𝑥^exp並累加得出結果。
### 解題想法

    本題的目標是實作一個能進行多項式基本運算的程式，包括加法、乘法及代入變數求值。
    因為多項式由多項（係數與指數）組成，因此設計上採用物件導向方式，將每一項定義為 Term 類別，並由 Polynomial 類別統一管理。
    透過運算子多載，可讓輸入與輸出更自然，像數學式一樣操作；
    在加法與乘法的部分，利用指數比對來合併或展開項，達成正確的運算結果。


### 範例說明

若輸入：

    a(x): 3x^2 + 2x^1 + 1
    b(x): x^1 + 4
    x=10

則輸出：

    a + b = 3x^2 + 3x^1 + 5
    a * b = 3x^3 + 14x^2 + 9x^1 + 4
    a(10) = 321
    b(10) = 14


## 程式實作

以下為主程式

```cpp
#include <iostream>
#include <cmath>
using namespace std;

class Term {
public:
    float coef;
    int exp;
    Term(float c = 0, int e = 0) : coef(c), exp(e) {}
};

class Polynomial {
private:
    Term* term;
    int terms;
public:
    Polynomial(int cap = 20) { term = new Term[cap]; terms = 0; }
    ~Polynomial() { delete[] term; }

    friend istream& operator>>(istream& in, Polynomial& p) {
        cout << "項數: "; in >> p.terms;
        for (int i = 0; i < p.terms; i++) {
            cout << "輸入係數 指數: ";
            in >> p.term[i].coef >> p.term[i].exp;
        }
        return in;
    }

    friend ostream& operator<<(ostream& out, const Polynomial& p) {
        for (int i = 0; i < p.terms; i++) {
            if (i && p.term[i].coef > 0) out << " + ";
            if (p.term[i].coef < 0) out << " - ";
            float c = fabs(p.term[i].coef);
            out << c << "x^" << p.term[i].exp;
        }
        return out;
    }

    Polynomial Add(const Polynomial& b) const {
        Polynomial r;
        int i = 0, j = 0, k = 0;
        while (i < terms && j < b.terms) {
            if (term[i].exp > b.term[j].exp) r.term[k++] = term[i++];
            else if (term[i].exp < b.term[j].exp) r.term[k++] = b.term[j++];
            else r.term[k++] = Term(term[i].coef + b.term[j].coef, term[i].exp), i++, j++;
        }
        while (i < terms) r.term[k++] = term[i++];
        while (j < b.terms) r.term[k++] = b.term[j++];
        r.terms = k;
        return r;
    }

    Polynomial Mult(const Polynomial& b) const {
        Polynomial r;
        for (int i = 0; i < terms; i++)
            for (int j = 0; j < b.terms; j++) {
                int e = term[i].exp + b.term[j].exp;
                float c = term[i].coef * b.term[j].coef;
                bool found = false;
                for (int k = 0; k < r.terms; k++)
                    if (r.term[k].exp == e) { r.term[k].coef += c; found = true; break; }
                if (!found) r.term[r.terms++] = Term(c, e);
            }
        return r;
    }

    float Eval(float x) const {
        float sum = 0;
        for (int i = 0; i < terms; i++) sum += term[i].coef * pow(x, term[i].exp);
        return sum;
    }
};

int main() {
    Polynomial a, b;
    cout << "輸入 a(x):\n"; cin >> a;
    cout << "輸入 b(x):\n"; cin >> b;

    Polynomial sum = a.Add(b);
    Polynomial prod = a.Mult(b);

    float x;
    cout << "輸入 x 的值: "; cin >> x;

    cout << "\na(x) = " << a
         << "\n\nb(x) = " << b
         << "\n\na + b = " << sum
         << "\n\na * b = " << prod
         << "\n\na(" << x << ") = " << a.Eval(x)
         << "\n\nb(" << x << ") = " << b.Eval(x) << endl;

    return 0;
}


```

## 效能分析

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




### 效能影響因素

1. 輸入項數（terms）多寡

    - 項數越多，加法、乘法時間會隨比例增加。
    - 對乘法而言，項數成平方關係成長。

2. 重複指數的出現次數
    - 若多項式乘法中有許多相同指數項，會花時間搜尋合併（線性搜尋）。
    - 若改用 map 或 hash 儲存，可降至 O(m × n) → O(m × n log n)。

3. pow() 函數使用
    - Eval() 每次呼叫 pow(x, exp)，若改用**逐步乘法（Horner’s Rule）**可加快效能。
    
## 測試與驗證

### 輸入

輸入 a(x):
項數: 3
輸入係數 指數: 2 3
輸入係數 指數: 2 2
輸入係數 指數: 4 1

輸入 b(x):
項數: 2
輸入係數 指數: 3 1
輸入係數 指數: 4 0

輸入 x 的值: 2


### 則輸出

   a(x) = 2x^3 + 2x^2 + 4x^1

b(x) = 3x^1 + 4

a + b = 2x^3 + 2x^2 + 7x^1 + 4

a * b = 6x^4 + 14x^3 + 20x^2 + 16x^1

a(2) = 32

b(2) = 10

## 申論及開發報告

### 心得討論

在這次多項式運算程式的開發過程中，我更深入了解了 C++ 的物件導向觀念，尤其是類別設計與運算子多載的應用。
一開始在設計 Polynomial 類別時，需要考慮如何有效地儲存每一項的係數與指數，最後採用了 Term 類別搭配陣列的方式，讓整體結構清晰且方便操作。

在實作加法與乘法時，我發現多項式的運算邏輯和一般數字不同，必須根據指數進行比對與合併。乘法的部分特別容易出錯，因為要同時處理每一項的展開與合併，讓我體會到演算法規劃的重要性。

整體來說，這個程式不僅讓我熟悉了類別封裝、動態記憶體配置與運算子多載的使用，也培養了我拆解問題與分析效能的能力。雖然程式仍可優化（例如使用 vector 或 Horner’s Rule），但我已能理解物件導向在數學運算中的應用價值，這是相當有收穫的一次練習。

### 總結

這次作業不僅強化了我對運算子多載與類別設計的理解，也讓我學會如何將數學概念轉化成可執行的程式邏輯，體會到物件導向在工程上的靈活性。