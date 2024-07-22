#include <iostream>
#include <vector>
#include <stdexcept>
#include <cassert>

using namespace std;

int n = 13;
int m = 29;

// ������͈͓��Ŏ擾����֐�
bool getEvenNumbersInRange(int start, int end, vector<int>& evens) {
    // �A�T�[�V����: start��end�ȉ��ł��邱�Ƃ��m�F
    assert(start <= end);

    if (start > end) {
        return false; // �G���[�������߂�l
    }

    for (int i = start; i <= end; i++) {
        if (i % 2 == 0) {
            evens.push_back(i);
        }
    }

    return true; // ����I���������߂�l
}

// �x�N�g���̓��e��\������֐�
bool displayVector(const vector<int>& v) {
    if (v.empty()) {
        return false; // �G���[�������߂�l
    }

    for (auto x : v) {
        cout << x << " ";
    }
    cout << endl;
    return true; // ����I���������߂�l
}

int main() {
    try {
        cout << n << " :start\n" << m << " :end\n";

        vector<int> evenNumbers;

        // �����̎擾
        if (!getEvenNumbersInRange(n, m, evenNumbers)) {
            throw invalid_argument("�J�n�l���I���l���傫���ł��B");
        }

        // �����̕\��
        if (!displayVector(evenNumbers)) {
            throw runtime_error("�x�N�g������ł��B");
        }

    }
    catch (const exception& e) {
        cerr << "�G���[: " << e.what() << endl;
        return 1;
    }

    return 0;
}