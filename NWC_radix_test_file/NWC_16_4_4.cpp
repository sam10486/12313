#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include "NWC_math.h"
#include "BitOperate.h"
#include "math.h"
#include "NWC.h"
#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

int main(){
    long long radix_4 = 4;
    long long n = 16;
    long long modular = 97;

    long long data_in[n];

    long long iteration_1_NTT[n];
    long long iteration_2_NTT[n];

    long long phi = find_phi(n, modular);

    BitOperate BR;


    cout << "input initial" << endl;
    for(int i=0; i<n; i++){
        data_in[i] = i;
        cout << data_in[i] << " ";
    }
    cout << endl;

   

    cout << "iteration 1 NTT" << endl;
    radix1_part_NWC(iteration_1_NTT, data_in, n, log2(radix_4), log2(radix_4), phi, modular);
    /*for(int i=0; i<n; i++)
        cout << iteration_1_NTT[i] << " ";
    cout << endl;*/


    cout << "iteration 2 NTT" << endl;
    radix2_part_NWC(iteration_2_NTT, iteration_1_NTT, n, log2(radix_4), log2(radix_4), phi, modular);

    cout << "output" << endl;
    for(int i=0; i<n;i++){
        int idx = BR.BitReserve(i, log2(n));
        cout << iteration_2_NTT[idx] << " ";
    }
    
    cout << endl;
}