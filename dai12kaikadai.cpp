#include <iostream>
#include <vector>
#include <stdexcept>
#include <cassert>

using namespace std;

int n = 13;
int m = 29;

// 偶数を範囲内で取得する関数
bool getEvenNumbersInRange(int start, int end, vector<int>& evens) {
    // アサーション: startがend以下であることを確認
    assert(start <= end);

    if (start > end) {
        return false; // エラーを示す戻り値
    }

    for (int i = start; i <= end; i++) {
        if (i % 2 == 0) {
            evens.push_back(i);
        }
    }

    return true; // 正常終了を示す戻り値
}

// ベクトルの内容を表示する関数
bool displayVector(const vector<int>& v) {
    if (v.empty()) {
        return false; // エラーを示す戻り値
    }

    for (auto x : v) {
        cout << x << " ";
    }
    cout << endl;
    return true; // 正常終了を示す戻り値
}

int main() {
    try {
        cout << n << " :start\n" << m << " :end\n";

        vector<int> evenNumbers;

        // 偶数の取得
        if (!getEvenNumbersInRange(n, m, evenNumbers)) {
            throw invalid_argument("開始値が終了値より大きいです。");
        }

        // 偶数の表示
        if (!displayVector(evenNumbers)) {
            throw runtime_error("ベクトルが空です。");
        }

    }
    catch (const exception& e) {
        cerr << "エラー: " << e.what() << endl;
        return 1;
    }

    return 0;
}