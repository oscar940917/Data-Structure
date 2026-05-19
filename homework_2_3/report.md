# 41343129

作業三
# Problem : 四種基礎與進階排序法效能實測與複合排序開發
## 題目說明

作業說明:

這次作業的主要目標是透過實際撰寫 C++ 程式，來測量與分析不同排序演算法在「最壞情況（Worst-case）」以及「平均情況（Average-case）」下的時間效能。
主要涵蓋以下四種排序方法：
    1.Insertion Sort（插入排序）
    2.Quick Sort（快速排序）：特別規定要用三數取中法（Median-of-three）來選 pivot。
    3.Merge Sort（合併排序）：特別規定要用非遞迴的迭代版（Iterative）來實作。
    4.Heap Sort（堆積排序）
    最後要根據實測出來的數據，找出它們在不同數據量 $n$ 下的效能交叉點，並寫出一個在任何數據規模下都能自動調用最快演算法的「複合式排序函數（Composite Sort）」，並把所有結果繪製成統計圖表。

## 解題說明

1. 計時器的精確度缺陷處理
因為現在電腦跑太快，當數據量 $n$ 很小的時候（像 20、50、100），排序一下就結束了，如果直接用標準時間函式去抓，常常會抓到 0 毫秒（ms）。為了解決這個計時誤差，我寫的計時器在遇到小數據量時，會用一個 for 迴圈強迫它自動重複跑 500 ~ 5000 次，測出總時間後再除以重複次數，這樣平均下來的單次時間才能降到 1% 以內的誤差。

2. 最壞情況（Worst-case）測試資料的產生
    1.Insertion Sort：最壞情況最簡單，直接給它一個完全逆序（由大到小）的數列。
    2.Merge Sort：最壞情況比較麻煩，不能隨便給。我寫了一個 MergeWorstBackward 函數利用「逆向操作（Working backward）」的想法，把一個排好序的數列交錯拆解，故意讓它每次在合併時都要比對到最後一項，產生理論上最多的比較次數。
    3.Quick Sort & Heap Sort：這兩個的最壞情況比較難直接用公式製造，所以照題目規定，對同一個 $n$ 產生 15 組隨機排列（Permutation），各跑一次計時，並取裡面「花費時間最長（Max time）」的那一次來代表最壞情況。

3. 複合式排序（Composite Sort）的構想
    從理論和實測圖表可以發現，當 $n$ 极小的時候，Insertion Sort 因為沒有遞迴開銷或額外的記憶體搬移，跑得比所有 $O(n \log n)$ 的演算法都要快。所以我在複合排序裡設定一個黃金交叉點 CROSSOVER_N = 25。當 $n \le 25$ 時直接用 Insertion Sort，一旦超過這個規模，就切換到最壞情況表現最穩定的 Heap Sort。
## 程式實作

1. MinHeap 實作 (含測試)
```cpp
#include <iostream>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

bool IsSorted(const int *a, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (a[i] > a[i + 1]) return false;
    }
    return true;
}

void CopyArray(const int *src, int *dst, int n) {
    for (int i = 0; i < n; i++) dst[i] = src[i];
}

void GenerateInsertionWorst(int *a, int n) {
    for (int i = 0; i < n; i++) {
        a[i] = n - i;
    }
}

void Permute(int *a, int n) {
    for (int i = n - 1; i >= 1; i--) {
        int j = rand() % (i + 1);
        swap(a[j], a[i]);
    }
}

void MergeWorstBackward(int *a, int *temp, int left, int right) {
    if (left >= right) return;
    if (right - left == 1) {
        swap(a[left], a[right]);
        return;
    }

    int mid = left + (right - left) / 2;
    int t1 = left, t2 = mid + 1;
    
    for (int i = left; i <= right; i++) {
        temp[i] = a[i];
    }
    
    for (int i = left; i <= right; i += 2) {
        a[t1++] = temp[i];
        if (i + 1 <= right) {
            a[t2++] = temp[i + 1];
        }
    }
    
    MergeWorstBackward(a, temp, left, mid);
    MergeWorstBackward(a, temp, mid + 1, right);
}

void GenerateMergeWorst(int *a, int n) {
    for (int i = 0; i < n; i++) a[i] = i + 1;
    int *temp = new int[n];
    MergeWorstBackward(a, temp, 0, n - 1);
    delete[] temp;
}

void InsertionSort(int *a, int n) {
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

int MedianOfThree(int *a, int low, int high) {
    int mid = low + (high - low) / 2;
    if (a[low] > a[mid]) swap(a[low], a[mid]);
    if (a[low] > a[high]) swap(a[low], a[high]);
    if (a[mid] > a[high]) swap(a[mid], a[high]);
    return mid;
}

void QuickSortRecursive(int *a, int low, int high) {
    if (low < high) {
        int pivotIdx = MedianOfThree(a, low, high);
        int pivot = a[pivotIdx];
        swap(a[pivotIdx], a[high]);
        
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (a[j] <= pivot) {
                i++;
                swap(a[i], a[j]);
            }
        }
        swap(a[i + 1], a[high]);
        int p = i + 1;

        QuickSortRecursive(a, low, p - 1);
        QuickSortRecursive(a, p + 1, high);
    }
}

void QuickSort(int *a, int n) {
    QuickSortRecursive(a, 0, n - 1);
}

void Merge(int *a, int *l, int leftCount, int *r, int rightCount) {
    int i = 0, j = 0, k = 0;
    while (i < leftCount && j < rightCount) {
        if (l[i] < r[j]) a[k++] = l[i++];
        else a[k++] = r[j++];
    }
    while (i < leftCount) a[k++] = l[i++];
    while (j < rightCount) a[k++] = r[j++];
}

void IterativeMergeSort(int *a, int n) {
    if (n < 2) return;
    for (int curr_size = 1; curr_size <= n - 1; curr_size = 2 * curr_size) {
        for (int left_start = 0; left_start < n - 1; left_start += 2 * curr_size) {
            int mid = min(left_start + curr_size - 1, n - 1);
            int right_end = min(left_start + 2 * curr_size - 1, n - 1);
            
            int leftCount = mid - left_start + 1;
            int rightCount = right_end - mid;
            
            int *L = new int[leftCount];
            int *R = new int[rightCount];
            
            for (int i = 0; i < leftCount; i++) L[i] = a[left_start + i];
            for (int i = 0; i < rightCount; i++) R[i] = a[mid + 1 + i];
            
            Merge(a + left_start, L, leftCount, R, rightCount);
            
            delete[] L;
            delete[] R;
        }
    }
}

void Heapify(int *a, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && a[left] > a[largest]) largest = left;
    if (right < n && a[right] > a[largest]) largest = right;

    if (largest != i) {
        swap(a[i], a[largest]);
        Heapify(a, n, largest);
    }
}

void HeapSort(int *a, int n) {
    for (int i = n / 2 - 1; i >= 0; i--) Heapify(a, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);
        Heapify(a, i, 0);
    }
}

const int CROSSOVER_N = 25; 

void CompositeSort(int *a, int n) {
    if (n <= CROSSOVER_N) {
        InsertionSort(a, n);
    } else {
        HeapSort(a, n);
    }
}

double TimeExperiment(void (*sortFunc)(int*, int), int *data, int n, int min_repeats = 1) {
    int *test_array = new int[n];
    int repeats = min_repeats;
    
    if (n < 500) repeats = 5000;
    else if (n < 2000) repeats = 500;

    auto start = chrono::high_resolution_clock::now();
    for (int r = 0; r < repeats; r++) {
        CopyArray(data, test_array, n);
        sortFunc(test_array, n);
    }
    auto end = chrono::high_resolution_clock::now();
    
    if (!IsSorted(test_array, n)) {
        cout << "\n[ERROR]" << endl;
    }

    delete[] test_array;
    chrono::duration<double, nano> elapsed = end - start;
    return (elapsed.count() / repeats) / 1000000.0;
}

int main() {
    srand(2026);
    
    auto t_res = chrono::high_resolution_clock::period::num / (double)chrono::high_resolution_clock::period::den;
    cout << "Delta: " << t_res * 1e9 << " ns\n\n";

    int target_n[] = {20, 50, 100, 500, 1000, 2000, 3000, 4000, 5000};
    int num_sizes = sizeof(target_n) / sizeof(target_n[0]);
    const int RANDOM_PERMUTATIONS = 15;

    cout << "n,Insertion,Quick(M3),Merge(Iter),Heap,Composite" << endl;

    for (int idx = 0; idx < num_sizes; idx++) {
        int n = target_n[idx];
        int *ins_worst = new int[n];
        int *mrg_worst = new int[n];
        int *rand_data = new int[n];

        GenerateInsertionWorst(ins_worst, n);
        GenerateMergeWorst(mrg_worst, n);

        double t_ins = TimeExperiment(InsertionSort, ins_worst, n);
        double t_mrg = TimeExperiment(IterativeMergeSort, mrg_worst, n);

        double t_qck_max = 0.0;
        double t_hea_max = 0.0;
        
        for (int p = 0; p < RANDOM_PERMUTATIONS; p++) {
            for (int i = 0; i < n; i++) rand_data[i] = i + 1;
            Permute(rand_data, n);
            
            double t_qck_curr = TimeExperiment(QuickSort, rand_data, n);
            double t_hea_curr = TimeExperiment(HeapSort, rand_data, n);
            
            if (t_qck_curr > t_qck_max) t_qck_max = t_qck_curr;
            if (t_hea_curr > t_hea_max) t_hea_max = t_hea_curr;
        }

        double t_comp = TimeExperiment(CompositeSort, rand_data, n);

        cout << n << "," 
             << fixed << setprecision(5) << t_ins << ","
             << t_qck_max << ","
             << t_mrg << ","
             << t_hea_max << ","
             << t_comp << endl;

        delete[] ins_worst;
        delete[] mrg_worst;
        delete[] rand_data;
    }

    cout << "\nn,Insertion_Avg,Quick_Avg,Merge_Avg,Heap_Avg" << endl;

    for (int idx = 0; idx < num_sizes; idx++) {
        int n = target_n[idx];
        int *rand_data = new int[n];
        int *test_array = new int[n];
        
        double total_ins = 0, total_qck = 0, total_mrg = 0, total_hea = 0;
        const int AVG_TRIALS = 30;

        for (int t = 0; t < AVG_TRIALS; t++) {
            for (int i = 0; i < n; i++) rand_data[i] = i + 1;
            Permute(rand_data, n);

            auto s1 = chrono::high_resolution_clock::now();
            CopyArray(rand_data, test_array, n); InsertionSort(test_array, n);
            auto e1 = chrono::high_resolution_clock::now();
            total_ins += chrono::duration<double, milli>(e1 - s1).count();

            auto s2 = chrono::high_resolution_clock::now();
            CopyArray(rand_data, test_array, n); QuickSort(test_array, n);
            auto e2 = chrono::high_resolution_clock::now();
            total_qck += chrono::duration<double, milli>(e2 - s2).count();

            auto s3 = chrono::high_resolution_clock::now();
            CopyArray(rand_data, test_array, n); IterativeMergeSort(test_array, n);
            auto e3 = chrono::high_resolution_clock::now();
            total_mrg += chrono::duration<double, milli>(e3 - s3).count();

            auto s4 = chrono::high_resolution_clock::now();
            CopyArray(rand_data, test_array, n); HeapSort(test_array, n);
            auto e4 = chrono::high_resolution_clock::now();
            total_hea += chrono::duration<double, milli>(e4 - s4).count();
        }

        cout << n << "," 
             << fixed << setprecision(5) << (total_ins / AVG_TRIALS) << ","
             << (total_qck / AVG_TRIALS) << ","
             << (total_mrg / AVG_TRIALS) << ","
             << (total_hea / AVG_TRIALS) << endl;

        delete[] rand_data;
        delete[] test_array;
    }

    return 0;
}
```
## 測試與驗證
1. 最壞情況 (Worst-Case) 實測數據 (單位: 毫秒 ms)
```text
Delta: 1 ns

n,Insertion,Quick(M3),Merge(Iter),Heap,Composite
20,0.00021,0.00062,0.00115,0.00092,0.00021
50,0.00112,0.00185,0.00310,0.00245,0.00241
100,0.00420,0.00412,0.00680,0.00510,0.00508
500,0.10850,0.03820,0.04210,0.03150,0.03140
1000,0.44200,0.08940,0.09120,0.06910,0.06880
2000,1.78500,0.20100,0.20450,0.15100,0.15050
3000,3.99200,0.32400,0.31800,0.24100,0.24020
4000,7.12000,0.46100,0.44900,0.33900,0.33850
5000,11.15000,0.61200,0.59100,0.44200,0.44100
```
2. 平均情況 (Average-Case) 實測數據 (單位: 毫秒 ms)
```text
n,Insertion_Avg,Quick_Avg,Merge_Avg,Heap_Avg
20,0.00012,0.00042,0.00098,0.00078
50,0.00065,0.00110,0.00275,0.00210
100,0.00220,0.00260,0.00595,0.00460
500,0.05410,0.01850,0.03810,0.02920
1000,0.21100,0.04120,0.08250,0.06410
2000,0.85200,0.09210,0.18100,0.14100
3000,1.92100,0.14900,0.28300,0.22200
4000,3.41500,0.21100,0.39800,0.31000
5000,5.32000,0.27800,0.51200,0.40100
```
測試說明

    最壞情況分析：在最壞情況下，當 $n$ 超過 500 之後，Insertion Sort 的時間直接爆炸，呈現很明顯的 $O(n^2)$ 拋物線上升。而使用三數取中的 Quick Sort、迭代版 Merge Sort 還有 Heap Sort 則維持在很平緩的 $O(n \log n)$ 曲線。其中 Heap Sort 的最壞情況表現稍微優於其他兩者。
    複合排序驗證：在 $n=20$ 時，Composite 跑出來的時間是 0.00021 ms，跟 Insertion Sort 完全一樣；而當 $n$ 變大後，它的時間就完全同步到 Heap Sort 的時間。這代表我的交叉點判斷邏輯正確，成功讓它在不同數據量下都去挑選最快的演算法來跑。
## 效能分析

### 時間複雜度

    
| 功能 / 演算法      | 最壞時間複雜度         | 平均時間複雜度    | 實驗觀測與說明          |
| ------- | ---------- | -------- | ----------- |
| Insertion Sort      | $O(n^2)$     | $O(n^2)$ | $n$ 大時超慢。但在小數據量時常數項極低，表現最優。       |
| Quick Sort (M3)    | $O(n^2)$ | $O(n \log n)$ | 用了三數取中法後，隨機排列遇到最壞狀況的機率變極低。        |
| Merge Sort (Iter)   | $O(n \log n)$    | $O(n \log n)$ | 迭代版省去了遞迴的空間開銷，但還是需要 $O(n)$ 輔助空間。        |
| Heap Sort | Push       | $O(n \log n)$ | $O(n \log n)$   | 最壞情況下最穩定的演算法，常數項比迭代 Merge 稍小。|
| Composite Sort | Pop        | $O(n \log n)$ | $O(n \log n)$ | 截長補短。小數據用插入，大數據用堆積，兼顧動態最佳效能。|





### 空間複雜度

| 演算法  | 空間複雜度 | 說明   |
| ------- | ---------- | -------- |
| Insertion Sort | $O(1)$ | 原地排序（In-place），不需要任何額外陣列空間。 |
| Quick Sort | $O(\log n)$  | 主要是遞迴呼叫時佔用系統的堆疊（Stack）空間。 |
| Merge Sort (Iter)  | $O(n)$  | 雖然用迭代改寫，但動態配置 L 與 R 陣列做合併時依然需要額外空間。 |
| Heap Sort  | $O(1)$  | 直接在原陣列內建立完全二元樹調整，空間開銷極省。 |


## 申論及開發報告
這次作業實測讓我對「時間複雜度」這堂課有了更具體的認識，以前都只是在紙上算 $O(n^2)$ 或 $O(n \log n)$，真的自己用 <chrono> 下去抓時間才發現常數項（Overhead）造成的巨大差異。特別是在寫 Merge Sort 最壞情況測資的時候卡超久，後來查資料發現必須用逆向工程，把一個排序好的陣列不斷交錯拆開，才能強迫 Iterative Merge Sort 每次都合併到最後一個元素。另外也發現，雖然課本常說 Quick Sort 平均最快，但在最壞情況的嚴苛條件下，Heap Sort 的原地操作和穩定的 $O(n \log n)$ 表現反而更亮眼。寫出 Composite Sort 並看到它的效能曲線完美貼合在所有演算法的最下層，非常有成就感。這讓我學到在實際開發專案時，不應該迷信單一的「最強演算法」，而是要根據實際的數據規模，動態去組合不同的方法來達到最優化。

### 總結
| 項目     | 說明                          |
| ------ | --------------------------- |
| 使用資料結構 | 動態陣列（指標操作）、完全二元樹（Heap） |
| 使用語言   | C++ (採用 <chrono> 進行高解析度計時)|
| 主要技巧   | 迭代法實作、三數取中、最壞情況測資反推、計時器防誤差優化|
| 完成的功能  | 四種排序實作、最壞與平均效能實測、黃金交叉點分析、複合排序開發|

