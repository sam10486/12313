#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include <bitset>
#include "NWC_math.h"
#include "BitOperate.h"
#include "math.h"
#include "NWC.h"
#include <fstream>

using namespace std;

void bitrev(){
    int n = 256 ;
    vector<long long> rev_idx_arr(n);
    BitOperate rev;

    ofstream bit_rev("./tb_data/SPMB_tw/rev_idx_arr.txt");

    for(long long i=0; i<n; i++){
        rev_idx_arr.at(i) = rev.BitReserve(i, log2(n));
    }

    for(int i=0; i<n; i++){
        bit_rev << "i = " << i << ", i_rev = " << rev_idx_arr[i] << endl;
    }
}