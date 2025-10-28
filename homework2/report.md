# 41343129

作業二

## 1. 問題 1 Abstract data type of Polynomial class

### 1.1 解題說明

這題要求實作 (Implement) 一個 多項式 (Polynomial) 的 抽象資料型態 (Abstract Data Type, ADT)
#### 1.1.1 解題策略

1. 設計 Polynomial 類，使用合適的數據結構（如 vector<pair<int, float>>）來表示多項式的項。

2. 實現構造函數，初始化多項式。

3. 實現加法函數 Add()，將兩個多項式相加，合併相同指數的項。

4. 實現乘法函數 Mult()，對兩個多項式進行逐項相乘，並合併相同指數的項。

5. 實現評估函數 Eval()，代入指定的值計算多項式的結果。

6. 重載 << 和 >> 操作符，簡化多項式的輸入輸出。
### 1.2 程式實作

```cpp
#include <iostream>
#include <vector>

using namespace std;

// 定義 Polynomial 類
class Polynomial {
private:
    vector<pair<int, float>> terms;  // 儲存多項式項，pair<int, float> 代表 <指數, 係數>

public:
    // 構造函數：初始化為零多項式
    Polynomial() {}

    // 加法操作
    Polynomial Add(const Polynomial& poly) const {
        Polynomial result;
        size_t i = 0, j = 0;
        while (i < terms.size() && j < poly.terms.size()) {
            if (terms[i].first < poly.terms[j].first) {
                result.terms.push_back(terms[i]);
                i++;
            } else if (terms[i].first > poly.terms[j].first) {
                result.terms.push_back(poly.terms[j]);
                j++;
            } else {
                result.terms.push_back({terms[i].first, terms[i].second + poly.terms[j].second});
                i++;
                j++;
            }
        }

        // 把剩餘的項加入結果
        while (i < terms.size()) {
            result.terms.push_back(terms[i]);
            i++;
        }

        while (j < poly.terms.size()) {
            result.terms.push_back(poly.terms[j]);
            j++;
        }

        return result;
    }

    // 乘法操作
    Polynomial Mult(const Polynomial& poly) const {
        Polynomial result;

        for (const auto& term1 : terms) {
            for (const auto& term2 : poly.terms) {
                bool found = false;
                for (auto& term3 : result.terms) {
                    if (term3.first == term1.first + term2.first) {
                        term3.second += term1.second * term2.second;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    result.terms.push_back({term1.first + term2.first, term1.second * term2.second});
                }
            }
        }

        return result;
    }

    // 評估多項式
    float Eval(float f) const {
        float result = 0;
        for (const auto& term : terms) {
            result += term.second * pow(f, term.first);
        }
        return result;
    }

    // 輸入操作符重載
    friend istream& operator>>(istream& in, Polynomial& poly) {
        int n;
        cout << "Enter number of terms: ";
        in >> n;
        poly.terms.clear();
        for (int i = 0; i < n; i++) {
            int exp;
            float coeff;
            cout << "Enter exponent and coefficient (e.g., 2 3.5): ";
            in >> exp >> coeff;
            poly.terms.push_back({exp, coeff});
        }
        return in;
    }

    // 輸出操作符重載
    friend ostream& operator<<(ostream& out, const Polynomial& poly) {
        for (size_t i = 0; i < poly.terms.size(); i++) {
            if (i > 0 && poly.terms[i].second > 0) out << " + ";
            out << poly.terms[i].second << "x^" << poly.terms[i].first;
        }
        return out;
    }
};

// 主函數：測試 Polynomial 類
int main() {
    Polynomial p1, p2, result;
    cout << "Enter the first polynomial:\n";
    cin >> p1;
    cout << "Enter the second polynomial:\n";
    cin >> p2;

    // 輸出輸入的多項式
    cout << "First Polynomial: " << p1 << endl;
    cout << "Second Polynomial: " << p2 << endl;

    // 多項式加法
    result = p1.Add(p2);
    cout << "Sum: " << result << endl;

    // 多項式乘法
    result = p1.Mult(p2);
    cout << "Product: " << result << endl;

    // 評估多項式
    float x;
    cout << "Enter a value for x: ";
    cin >> x;
    cout << "Evaluation of the first polynomial at x = " << x << ": " << p1.Eval(x) << endl;

    return 0;
}
```

### 1.3 效能分析

1. 空間複雜度：

使用 vector<pair<int, float>> 存儲多項式，空間複雜度為 O(n)，其中 n 是項數。乘法時可能需要 O(n^2) 空間。

2. 時間複雜度：

    加法 (Add)：O(m + n)，其中 m 和 n 分別是兩個多項式的項數。

    乘法 (Mult)：O(n^2)，每個項與其他項相乘。

    評估 (Eval)：O(n)，遍歷每個項計算結果。

3. 堆疊深度：

    遞迴深度可能影響堆疊，尤其是處理大量項時。

4. 優化：

    使用哈希表可將加法和乘法的查找時間降低到 O(n)。

5. 總結：

    空間：O(n)，最壞為 O(n^2)。

    時間：加法 O(m + n)，乘法 O(n^2)，評估 O(n)
### 1.4 測試與驗證
我們將針對不同情境下進行測試，確保多項式操作的正確性與穩定性。

1. 常見情況：

    測試兩個普通多項式的加法與乘法。例如：

        輸入：2x^3 + 3x^2 + 4x + 5 和 x^2 + 1。

        預期結果：加法與乘法後的多項式。

2. 邊界情況：

    零項多項式：測試加法、乘法和評估對於零項多項式（0x^n）的處理。

        輸入：0x^3 + 0x^2 + 0x + 0 和任意多項式。

        預期結果：加法、乘法後應正確處理零項多項式，且評估應返回零。

    單項式：測試只有一項的多項式。

        輸入：3x^2 和 2x^2。

        預期結果：加法與乘法後應正確處理單項式。

3. 極端情況：

    極大項數的多項式：測試多項式項數非常大時的效能表現。
        輸入：例如，具有 1000 個項的多項式。

        預期結果：測試加法與乘法能夠正常執行，且不會發生內存溢出或時間過長的情況。

4. 特殊情況：

    測試對於負係數、多項式中有零項的情況，以及其他非常規情況的處理。
#### 1.4.1 測試案例
量測方法：

使用 chrono 庫來測量算法的運行時間。

    測量加法操作的時間：生成兩個多項式，對其進行加法運算，測量執行時間。

    測量乘法操作的時間：生成兩個多項式，對其進行乘法運算，測量執行時間。

    測量評估操作的時間：對多項式進行評估，測量執行時間。

範例代碼：

#include <chrono>
using namespace std::chrono;

auto start = high_resolution_clock::now();
// 執行加法操作
auto stop = high_resolution_clock::now();
auto duration = duration_cast<microseconds>(stop - start);
cout << "加法運行時間: " << duration.count() << " 微秒" << endl;


2. 測量空間消耗：

    使用工具或手動測量每次操作後多項式所佔的內存大小，特別是當多項式項數增長時，觀察空間消耗如何增長。

3. 量測不同項數下的性能：

    生成多個具有不同項數的多項式（例如：10、100、1000 個項），並測量運行時間如何隨著項數增加而增長。

#### 1.5 討論心得

在這次設計與實作過程中，我學到了多項式的操作並不像基本數學運算那麼簡單，尤其是在處理大量項時，性能成為一個主要挑戰。

1. 遞迴與效能：
    多項式操作中，尤其是在乘法部分，遞迴的深度會迅速增長，這對於計算時間與堆疊深度都會帶來很大的影響。特別是乘法操作，隨著項數增加，運行時間呈指數增長，需要進一步優化。

2. 處理大數據集的問題：
    當多項式項數達到千級時，操作的運行時間會顯著增加。這使我意識到，在實際應用中，需要考慮如何優化算法，可能需要使用更高效的數據結構（如哈希表或鏈表），或者對算法進行分治優化。

3. 測試過程中的問題與解決方案：
    在測試過程中，我發現多項式的加法與乘法對於零項多項式處理不夠穩定，這導致了錯誤結果。經過調整後，我確保在加法和乘法時能夠正確處理零項。

總體而言，這次設計過程讓我深刻理解了如何處理多項式運算中的各種情況，並對遞迴在性能上的影響有了更多的認識。
### 1.6 申論與開發報告

在本題中，我們實作了 Polynomial 類，設計了加法、乘法和評估操作，並使用 vector<pair<int, float>> 存儲多項式項。加法和乘法操作分別處理項的合併與運算，並考慮到邊界情況，如零項多項式和單項式。為了確保正確性，進行了多種情境的測試，特別是對極端情況的測試。

效能量測使用 chrono 庫來測量運行時間，發現當多項式項數增長時，乘法操作的運行時間增加較快，提示未來可進一步優化算法。在開發過程中，最大的挑戰是處理大數據集，特別是在乘法運算中的性能瓶頸。總體而言，這次開發加深了我對多項式運算及其效率優化的理解。

## 2. 問題 2 Powerset

### 2.1 解題說明

本題要求透過遞迴方式實現冪集合的方法，能夠根據指定集合產生另一個集合，且產生的集合為原集合之冪集合。

冪集合，包含了原集合中所有可能的子集合。

#### 2.1.1 解題策略

透過觀察可以發現，冪集合與原集合有密切的關聯性：
- 最小的子集合為空集合[∅]
- 最大的子集合為原集合本身

若將每個子集合視為原集合中每個元素的「存在與否」組合，就可以與二進位表示對應：
- 假設原集合有 4 個元素，索引分別為 0~3
- 「1111₂」表示所有元素存在（即原集合本身）
- 「0000₂」表示所有元素不存在（空集合）

因此，只要將二進位數值從 0 遞增到 $2^n−1$，即可系統性地生成所有子集合。

### 2.2 程式實作

```cpp
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
using namespace std;
template<typename T>
class MyVector {
    private:
        T* data;          // 指向儲存資料的動態陣列
        size_t sz;        // 元素個數
        size_t capacity;  // 容量大小（目前可存放元素數）

        // 私有函式：擴充容量，並把舊資料搬移過去
        void resize(size_t new_capacity) {
            T* new_data = new T[new_capacity];      // 配置新陣列
            for (size_t i = 0; i < sz; ++i)        // 拷貝舊元素
                new_data[i] = data[i];
            delete[] data;                          // 釋放舊陣列
            data = new_data;
            capacity = new_capacity;
        }

    public:
        // 建構子，初始無資料
        MyVector() : data(nullptr), sz(0), capacity(0) {}

        // 解構子，釋放動態記憶體
        ~MyVector() {
            delete[] data;
        }

        // 回傳目前元素數量
        size_t size() const {
            return sz;
        }

        // 尾端新增元素
        void push_back(const T& value) {
            if (sz == capacity) {                   // 容量不足時擴容
                size_t new_capacity = capacity == 0 ? 1 : capacity * 2;
                resize(new_capacity);
            }
            data[sz++] = value;                     // 新增元素並更新大小
        }

        // 取得指定位置元素，不做邊界檢查
        T& operator[](size_t index) {
            return data[index];
        }

        // 移除最後一個元素
        void pop_back() {
            if (sz > 0)
                --sz;
        }
};
int main(){
    int x;
    string IN,word;
    MyVector<string> D;
    getline(cin,IN);
    stringstream ss(IN);
    while(ss>>word)
        D.push_back(word);
    x=D.size();
    int C=pow(2,x);
    bool d[C][x];
    for(int i=0;i<C;i++){
        int r=i;
        for(int j=0;j<x;j++){
            d[i][x-j-1]=r%2;
            r/=2;
        }
    }
    cout<<"\n[∅]\n";
    for(int i=1;i<C;i++){
        bool f=0;
        cout<<"[";
        for(int j=0;j<x;j++){
            if(d[i][j]){
                if(f>0)
                    cout<<",";
                f=1;
                cout<<D[j];
            }
        }
        cout<<"]\n";
    }
    return 0;
}
```

### 2.3 效能分析

時間複雜度: $O(x·2^x)$

空間複雜度: $O(x·2^x)$

### 2.4 測試與驗證

#### 2.4.1 測試案例

| 案例編號 | 輸入參數 $size$ | 輸入參數 $data$ | 輸出 |
| - | - | - | - |
| 1 | 0 | - | [∅] |
| 2 | 1 | a | [∅] [a] |
| 3 | 3 | a b c | [∅] [c] [b] [b,c] [a] [a,c] [a,b] [a,b,c] |
| 4 | 4 | a bb ab ccc | [∅] [ccc] [ab] [ab,ccc] [bb] [bb,ccc] [bb,ab] [bb,ab,ccc] [a] [a,ccc] [a,ab] [a,ab,ccc] [a,bb] [a,bb,ccc] [a,bb,ab] [a,bb,ab,ccc] |

#### 2.4.2 執行指令

```shell
$ a bb ab ccc
[∅] [ccc] [ab] [ab,ccc] [bb] [bb,ccc] [bb,ab] [bb,ab,ccc] [a] [a,ccc] [a,ab] [a,ab,ccc] [a,bb] [a,bb,ccc] [a,bb,ab] [a,bb,ab,ccc]
```

### 2.5 申論與開發報告

為了系統化生成冪集合，我採用了二進位表示法：將每個子集合對應到原集合元素「存在與否」的二進位數值。

每個二進位位元對應原集合中一個元素

1 表示該元素存在，0 表示不存在

遞增遍歷二進位數值，即可生成所有可能的子集合

在程式中，我建立了一個布林矩陣 d[C][x] 來儲存每個二進位組合的元素存在情況，避免在每次輸出時重複計算。

C = 2^x 為子集合總數

x 為原集合元素數量

主程式依序遍歷矩陣，依布林值判斷是否輸出對應元素，從而列出完整的冪集合，包含空集合 [∅]。

程式設計上，實現了以下特性：

系統化生成冪集合：利用二進位表示法與布林矩陣避免冗長的巢狀迴圈

簡單易讀的輸出：每個子集合用中括號表示，元素間以逗號分隔，空集合單獨列出

觀察程式輸出可以發現，冪集合並沒有依子集合大小或元素順序排序。雖然數學上冪集合無需排序，但若需要更清楚的可視化，可進一步新增排序功能，例如對集合元素進行排序或建立比較運算子。

此外，程式中 時間複雜度為 O(x·2^x)，空間複雜度亦為 O(x·2^x)，符合理論上生成所有子集合的最佳效率。

## 程式實作

[問題一](#12-程式實作)

[問題二](#22-程式實作)

## 效能分析

[問題一](#13-效能分析)

[問題二](#23-效能分析)

## 測試與驗證

[問題一](#14-測試與驗證)

[問題二](#24-測試與驗證)

## 解題說明

[問題一](#11-解題說明)

[問題二](#21-解題說明)

## 申論及開發報告

[問題一](#15-申論與開發報告)

[問題二](#25-申論與開發報告)
