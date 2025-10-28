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
