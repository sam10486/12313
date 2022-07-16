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
    long long m = 32;
    long long modular = 257;
    BitOperate BU;

    vector<long long > input_vector(m);
    vector<vector<long long > > stage0_data_in(radix_2, vector<long long > (radix_16, 0));
    vector<vector<long long > > stage0_data_fft(radix_2, vector<long long > (radix_16, 0));

    vector<vector<long long > > stage1_data_in(16, vector<long long > (radix_2, 0));
    vector<vector<long long > > stage1_data_fft(16, vector<long long > (radix_2, 0));

    long long phi_32 = find_phi(m, modular);
    long long phi_16 = ExpMod(phi_32, 2, modular);
    long long phi_2 = ExpMod(phi_32, 16, modular);

    long long prou_32 = ExpMod(phi_32, 2, modular);
    long long prou_16 = ExpMod(prou_32, 2, modular);
    long long prou_2 = ExpMod(prou_16, 8, modular);

    cout << "phi_16 = " << phi_16 << endl;
    cout << "prou_16 = " << prou_16 << endl;

    for(int i=0; i<m; i++){
        input_vector[i] = i;
        cout << "input_vector[" << i  << "] "<< " = " << input_vector[i] << endl;
    }

    cout << "----------stage0_data_in-----------" << endl;
    for(int i=0; i<2; i++){
        for(int j=0; j<16; j++){
            stage0_data_in[i][j] = input_vector[i+2*j];
            cout << "stage0_data_in[" << i << "][" << j << "]" << " = " << stage0_data_in[i][j] << endl;
        }
    }

    cout << "----------stage0_fft------------" << endl;
    for(int i=0; i<2; i++){
        for(int j=0; j<16; j++){
            stage0_data_fft[i] = NWC_forward_DIT(stage0_data_in[i], radix_16, phi_16, modular);
            cout << "stage0_data_fft[" << i << "][" << j << "]" << " = " << stage0_data_fft[i][j] << endl;
        }
    }

    cout << "------------stage_1 data input----------" << endl;
    for(int i=0; i<16; i++){
        cout << "-----------------------------" << endl;
        for(int j=0; j<2; j++){
            stage1_data_in[i][j] = stage0_data_fft[j][i];
            cout << "stage1_data_in[" << i << "][" << j << "]" << " = " << stage1_data_in[i][j] << endl;
        }
    }

    cout << "----------stage_1 data times twiddle---------" << endl;
    for(int i=0; i<16; i++){
        cout << "-----------------------------" << endl;
        for(int j=0; j<2; j++){
            long long phi_32_dg2 = ExpMod(phi_32, 2, modular);
            long long phi_32_degree = ExpMod(phi_32_dg2, i, modular);
            stage1_data_in[i][j] = MulMod(stage1_data_in[i][j], ExpMod(phi_32_degree, j, modular), modular);
            cout << "phi_32_degree = " << phi_32_degree << endl;
            cout << "stage_1 data times twiddle[" << i << "][" << j << "]" << " = " << stage1_data_in[i][j] << endl;
        }
    }

    cout << "----------stage_1 fft---------" << endl;
    for(int i=0; i<16; i++){
        cout << "-----------------------------" << endl;
        for(int j=0; j<2; j++){
            stage1_data_fft[i] = NWC_forward_DIT(stage1_data_in[i], radix_2, phi_32, modular);
            cout << "stage1_data_fft[" << i << "][" << j << "]" << " = " << stage1_data_fft[i][j] << endl;
        }
    }

    cout << "----------flatten--------------" << endl;
    vector<long long > NWC_flatten(m);
    for(int i=0; i<16; i++){
        for(int j=0; j<2; j++){
            NWC_flatten[i+j*16] =  stage1_data_fft[i][j];
            cout << "NWC_flatten[" << i+j*8 << "]" << " = " << NWC_flatten[i+j*16] << endl;
        }
    }

    cout << "----------Bit Reverse--------------" << endl;
    vector<long long > NWC_flatten_BR(m);
    BitOperate BR;
    for(int i=0; i<m; i++){
        long long idx_BR = BR.BitReserve(i, 5);
        NWC_flatten_BR[i] = NWC_flatten[idx_BR];
        cout << "NWC_flatten_BR[" << i << "]" << " = " << NWC_flatten_BR[i] << endl;
    }

    cout <<"-------------------------" << endl;
    int err = 0;
    vector<long long > NWC_ans(m);
    NWC_ans = NWC(input_vector, m, phi_32, modular);

    for(int i=0; i<m; i++){
        cout << "NWC_ans = " << NWC_ans[i] << endl;
        if(NWC_flatten_BR[i] != NWC_ans[i]){
            err++;
        }else{
            err= err;
        }
    }

    if(err == 0)
        cout << "correct " << endl;
    else
        cout << "error!!" << endl;
}