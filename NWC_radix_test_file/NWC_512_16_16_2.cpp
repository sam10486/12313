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
    long long radix_2 = 2;
    long long radix_16 = 16;
    long long n = 32;
    long long modular = 257;

    long long data_in[n];
    long long iteration_1_NTT[n];
    long long iteration_1_suffle[n];

    long long iteration_2_NTT[n];
    long long iteration_3_NTT[n];

    long long phi = find_phi(n, modular);

    BitOperate BR;

    cout << "input initial" << endl;
    for(int i=0; i<n; i++){
        data_in[i] = i;
        cout << data_in[i] << " ";
    }
    cout << endl;

    cout << "iteration 1 NTT" << endl;
    radix1_part_NWC(iteration_1_NTT, data_in, n, log2(radix_16), log2(radix_2), phi, modular);
    /*for(int i=0; i<n; i++)
        cout << iteration_1_NTT[i] << " ";
    cout << endl;*/

    for(int k=0; k<(n/(n/radix_16)); k++){
        cout << "group[" << k << "]" << endl; 
        for(int i=0; i<(n/radix_16); i++){
            cout << "iteration_1_NTT[" << (n/radix_16)*k+i << "] = " << iteration_1_NTT[(n/radix_16)*k+i] << endl;
        }
    }

    cout << "************************" << endl;
    int group_num = (n/(n/radix_16));
    for(int k=0; k<group_num; k++){
        //cout << "group[" << k << "]" << endl; 
        for(int i=0; i<(n/radix_16); i++){
            //cout << "k = " << k << endl;
            int idx = BR.BitReserve(k, log2(group_num));
            //cout << "idx = " << idx << endl;
            //cout << "(n/4)*idx+i = " << (n/4)*idx+i << endl;
            iteration_1_suffle[(n/radix_16)*k+i] = iteration_1_NTT[(n/radix_16)*idx+i];
            //cout << "iteration_1_suffle[" << (n/radix_16)*k+i << "] = " << iteration_1_suffle[(n/radix_16)*k+i] << endl;
        }
    }
 

    cout << "iteration 2 NTT" << endl;
    radix1_part_NWC(iteration_2_NTT, iteration_1_suffle, n, log2(radix_16), log2(radix_2), phi, modular);



    cout << "iteration 3 NTT" << endl;
    radix2_part_NWC(iteration_3_NTT, iteration_2_NTT, n, log2(radix_16), log2(radix_2), phi, modular);
    cout << "output" << endl;
    for(int i=0; i<n;i++){
        int idx = BR.BitReserve(i, log2(n));
        cout << iteration_3_NTT[idx] << " ";
    }
    cout << endl;
}