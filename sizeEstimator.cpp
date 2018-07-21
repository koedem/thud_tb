//
// Created by kolja on 7/19/18.
//

#include "sizeEstimator.h"
#include <array>
#include <iostream>

static int bitTable[257];
static int symmetryTable[3][164] = {
        { 4, 3, 2, 1, 0, 11, 10, 9, 8, 7, 6, 5, 20, 19, 18, 17, 16, 15, 14, 13, 12, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21,
          44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 74, 73,
          72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 103, 102, 101,
          100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104,
          131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 151,
          150, 149, 148, 147, 146, 145, 144, 143, 158, 157, 156, 155, 154, 153, 152, 163, 162, 161, 160, 159 },  // squares from top to bottom, right to left
                  { 45, 60, 75, 89, 104, 32, 46, 61, 76, 90, 105, 119, 21, 33, 47, 62, 77, 91, 106, 120, 132, 12, 22, 34, 48, 63, 78,
                    92, 107, 121, 133, 143, 5, 13, 23, 35, 49, 64, 79, 93, 108, 122, 134, 144, 152, 0, 6, 14, 24, 36, 50, 65, 80, 94,
                    109, 123, 135, 145, 153, 159, 1, 7, 15, 25, 37, 51, 66, 81, 95, 110, 124, 136, 146, 154, 160, 2, 8, 16, 26, 38,
                    52, 67, 96, 111, 125, 137, 147, 155, 161, 3, 9, 17, 27, 39, 53, 68, 82, 97, 112, 126, 138, 148, 156, 162, 4, 10,
                    18, 28, 40, 54, 69, 83, 98, 113, 127, 139, 149, 157, 163, 11, 19, 29, 41, 55, 70, 84, 99, 114, 128, 140, 150, 158,
                    20, 30, 42, 56, 71, 85, 100, 115, 129, 141, 151, 31, 43, 57, 72, 86, 101, 116, 130, 142, 44, 58, 73, 87, 102, 117,
                    131, 59, 74, 88, 103, 118 }, // squares from left to right, top to bottom
                            { 104, 89, 75, 60, 45, 119, 105, 90, 76, 61, 46, 32, 132, 120, 106, 91, 77, 62, 47, 33, 21, 143, 133, 121,
                              107, 92, 78, 63, 48, 34, 22, 12, 152, 144, 134, 122, 108, 93, 79, 64, 49, 35, 23, 13, 5, 159, 153, 145,
                              135, 123, 109, 94, 80, 65, 50, 36, 24, 14, 6, 0, 160, 154, 146, 136, 124, 110, 95, 81, 66, 51, 37, 25,
                              15, 7, 1, 161, 155, 147, 137, 125, 111, 96, 67, 52, 38, 26, 16, 8, 2, 162, 156, 148, 138, 126, 112, 97,
                              82, 68, 53, 39, 27, 17, 9, 3, 163, 157, 149, 139, 127, 113, 98, 83, 69, 54, 40, 28, 18, 10, 4, 158, 150,
                              140, 128, 114, 99, 84, 70, 55, 41, 29, 19, 11, 151, 141, 129, 115, 100, 85, 71, 56, 42, 30, 20, 142, 130,
                              116, 101, 86, 72, 57, 43, 31, 131, 117, 102, 87, 73, 58, 44, 118, 103, 88, 74, 59 } }; // squares from left to right, bottom to top

void sizeEstimator::setupEstimation() {
    sizeEstimator::setupBitTable();
}

void sizeEstimator::setupBitTable() {
    bitTable[0] = -1; // this shouldn't happen
    int index = 1;
    for (int value = 0; value < 9; value++) {
        for (int range = 0; range < (1 << (value - 1)); range++) {
            index++;
            bitTable[index] = value;
        }
    }
}

void sizeEstimator::zeroArray(long count[9][9][9][9][9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            for (int k = 0; k < 9; k++) {
                for (int l = 0; l < 9; l++) {
                    for (int m = 0; m < 9; m++) {
                        for (int n = 0; n < 9; n++) {
                            count[i][j][k][l][m][n] = 0;
                        }
                    }
                }
            }
        }
    }
}

int sizeEstimator::countWithSymmetries(bool simpleSymmetry, bool reverseSymmetry, bool fullSymmetry, long count[9][9][9][9][9][9]) {
    sizeEstimator::zeroArray(count);
    int diff[7] = { 0 };
    int tempDiff[7] = { 0 };

    for (int i = 0; i < 159; i++) {
        for (int j = i + 1; j < 160; j++) {
            std::cout << i << ' ' << j << std::endl;
            for (int k = j + 1; k < 161; k++) {
                for (int l = k + 1; l < 162; l++) {
                    for (int m = l + 1; m < 163; m++) {
                        for (int n = m + 1; n < 164; n++) {
                            diff[0] = i - (-1);
                            diff[1] = j - i;
                            diff[2] = k - j;
                            diff[3] = l - k;
                            diff[4] = m - l;
                            diff[5] = n - m;
                            diff[6] = 164 - n;

                            if (simpleSymmetry && (diff[6] < diff[0] || diff[6] == diff[0] && (diff[5] < diff[1]
                                    || diff[5] == diff[1] && diff[4] < diff[2]))) {
                                tempDiff[6] = diff[0]; // reverse the array
                                tempDiff[5] = diff[1];
                                tempDiff[4] = diff[2];
                                tempDiff[3] = diff[3];
                                tempDiff[2] = diff[4];
                                tempDiff[1] = diff[5];
                                tempDiff[0] = diff[6];
                                for (int a = 0; a < 7; a++) {
                                    diff[a] = tempDiff[a];
                                }
                            }

                            if (reverseSymmetry) {
                                tempDiff[0] = symmetryTable[0][i] - (-1);
                                tempDiff[1] = symmetryTable[0][j] - symmetryTable[0][i];
                                tempDiff[2] = symmetryTable[0][k] - symmetryTable[0][j];
                                tempDiff[3] = symmetryTable[0][l] - symmetryTable[0][k];
                                tempDiff[4] = symmetryTable[0][m] - symmetryTable[0][l];
                                tempDiff[5] = symmetryTable[0][n] - symmetryTable[0][m];
                                tempDiff[6] = symmetryTable[0][163] + 1 - symmetryTable[0][n];


                            }



                            count[bitTable[diff[0]]][bitTable[diff[1]]][bitTable[diff[2]]][bitTable[diff[3]]]
                            [bitTable[diff[4]]][bitTable[diff[5]]]++;
                        }
                    }
                }
            }
        }
    }
    return 0;
}