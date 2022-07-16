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
    long long radix_8 = 8;
    long long m = 16;
    long long modular = 257;
    BitOperate BR;

    long long input_vector[m];
    long long stage0_data_in[radix_2][radix_8];
    long long stage0_data_fft[radix_2][radix_8];

    long long stage1_data_in[8][radix_2];
    long long stage1_data_fft[8][radix_2];

    long long phi_16 = find_phi(m, modular);
    long long phi_8 = ExpMod(phi_16, 2, modular);
    long long phi_2 = ExpMod(phi_16, 8, modular);

    long long prou_16 = ExpMod(phi_16, 2, modular);
    long long prou_8 = ExpMod(prou_16, 2, modular);
    long long prou_2 = ExpMod(prou_16, 8, modular);

    cout << "phi_16 = " << phi_16 << endl;
    cout << "prou_16 = " << prou_16 << endl;

    cout << "phi_8 = " << phi_8 << endl;
    cout << "prou_8 = " << prou_8 << endl;

    cout << "phi_2 = " << phi_2 << endl;
    cout << "prou_2 = " << prou_2 << endl;

    for(int i=0; i<m; i++){
        long long idx_BR = BR.BitReserve(i, log2(m));
        input_vector[i] = idx_BR;
        cout << "input_vector[" << i  << "] "<< " = " << input_vector[i] << endl;
    }
    
    cout << "----------stage0_data_in-----------" << endl;
    for(int i=0; i<2; i++){
        cout << "-----------------" << i << "-------------------" << endl;
        for(int j=0; j<8; j++){
            stage0_data_in[i][j] = input_vector[i*8+j];
            cout << "stage0_data_in[" << i << "][" << j << "]" << " = " << stage0_data_in[i][j] << endl;
        }
    }

    cout << "----------stage0_fft------------" << endl;
    for(int i=0; i<2; i++){
        cout << "#############- i = " << i << "-###########---" << endl;
        //NWC_FFT_no_bit_reverse(stage0_data_fft[i], stage0_data_in[i], radix_8, prou_8, phi_8, modular);
        for(int j=0; j<8; j++){
            NWC_FFT_no_bit_reverse(stage0_data_fft[i], stage0_data_in[i], radix_8, prou_8, phi_8, modular);
            cout << "stage0_data_fft[" << i << "][" << j << "]" << " = " << stage0_data_fft[i][j] << endl;
        }
    }

    cout << "------------stage_1 data input----------" << endl;
    for(int i=0; i<8; i++){
        cout << "-----------------------------" << endl;
        for(int j=0; j<2; j++){
            stage1_data_in[i][j] = stage0_data_fft[j][i];
            cout << "stage1_data_in[" << i << "][" << j << "]" << " = " << stage1_data_in[i][j] << endl;
        }
    }

    cout << "----------stage_1 data times twiddle---------" << endl;
    for(int i=0; i<8; i++){
        cout << "-----------------------------" << endl;
        for(int j=0; j<2; j++){
            long long index_rev = BR.BitReserve(j, 1);
            long long exp_prou16 = i*index_rev;
            long long exp_phi16 = index_rev;
            long long twiddle = MulMod(ExpMod(prou_16, exp_prou16, modular), ExpMod(phi_16, exp_phi16, modular), modular);
            stage1_data_in[i][j] = MulMod(stage1_data_in[i][j], twiddle, modular);
            cout << "stage_1 data times twiddle[" << i << "][" << j << "]" << " = " << stage1_data_in[i][j] << endl;
        }
    }
    cout << "----------stage_1 fft---------" << endl;
    for(int i=0; i<8; i++){
        cout << "-----------------------------" << endl;
        for(int j=0; j<2; j++){
            NWC_FFT_no_bit_reverse(stage1_data_fft[i], stage1_data_in[i], radix_2, prou_2, phi_16, modular);
            cout << "stage1_data_fft[" << i << "][" << j << "]" << " = " << stage1_data_fft[i][j] << endl;
        }
    }


 
}