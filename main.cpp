#include <iostream>
#include "sizeEstimator.h"

using namespace std;

int main() {
    long count[9][9][9][9][9][9] = { 0 };
    sizeEstimator::setupEstimation();
    sizeEstimator::countWithSymmetries(true, true, false, count);
    long files[30] = { 0 };
    long efficiency[30] = { 0 };
    for (int bitSum = 1; bitSum < 30; bitSum++) {
        int bitLeft = bitSum;
        for (int i = 0; i < 9; i++) {
            if (i != 0) {
                bitLeft -= (i - 1);
            }
            for (int j = 0; j < 9; j++) {
                if (j != 0) {
                    bitLeft -= (j - 1);
                }
                for (int k = 0; k < 9; k++) {
                    if (k != 0) {
                        bitLeft -= (k - 1);
                    }
                    for (int l = 0; l < 9; l++) {
                        if (l != 0) {
                            bitLeft -= (l - 1);
                        }
                        for (int m = 0; m < 9; m++) {
                            if (m != 0) {
                                bitLeft -= (m - 1);
                            }
                            for (int n = 0; n < 9; n++) {
                                if (n != 0) {
                                    bitLeft -= (n - 1);
                                }
                                if ((bitLeft == 1) && count[i][j][k][l][m][n] != 0) {
                                    files[bitSum]++;
                                    efficiency[bitSum] += count[i][j][k][l][m][n];
                                    cout << i << j << k << l << m << n << ':' << ' ' << count[i][j][k][l][m][n] << endl;
                                }
                                if (n != 0) {
                                    bitLeft += n - 1;
                                }
                            }
                            if (m != 0) {
                                bitLeft += (m - 1);
                            }
                        }
                        if (l != 0) {
                            bitLeft += l - 1;
                        }
                    }
                    if (k != 0) {
                        bitLeft += k - 1;
                    }
                }
                if (j != 0) {
                    bitLeft += j - 1;
                }
            }
            if (i != 0) {
                bitLeft += (i - 1);
            }
        }
    }

    cout << endl << endl;
    long sumOfPositions = 0;
    long sumOfSpace = 0;
    long sumOfFiles = 0;
    for (int i = 0; i < 30; i++) {
        cout << i << ' ' << files[i] << ' ' << (files[i] > 0 ? ((1000 * efficiency[i] / files[i]) >> (i - 1)) : 0)
             << ' ' << efficiency[i] << endl;
        sumOfPositions += efficiency[i];
        sumOfSpace += files[i] << (i - 1);
        sumOfFiles += files[i];
    }

    cout << endl << endl << (sumOfPositions * 1000) / sumOfSpace << ' ' << sumOfPositions << ' ' << sumOfSpace << ' '
    << sumOfFiles << endl;
    return 0;
}