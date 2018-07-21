//
// Created by kolja on 7/19/18.
//

#ifndef THUD_TB_SIZEESTIMATOR_H
#define THUD_TB_SIZEESTIMATOR_H


class sizeEstimator {
public:
    static int countWithSymmetries(bool simpleSymmetry, bool reverseSymmetry, bool rotationSymmetry, long count[9][9][9][9][9][9]);
    static void setupEstimation();

private:
    static void setupBitTable();
    static void zeroArray(long count[9][9][9][9][9][9]);
};


#endif //THUD_TB_SIZEESTIMATOR_H
