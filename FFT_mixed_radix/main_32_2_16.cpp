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

    long long FFT_data_in[m];

    long long stage0_data_in[16][radix_2];
    long long stage0_data_out_tmp[16][radix_2];
    
    long long stage1_data_in[2][radix_16];
    long long stage1_data_out_tmp[2][radix_16];

    long long FFT_out_final[m];

    long long prou_32 = find_prou(m, modular);
    long long prou_16 = ExpMod(prou_32, 2, modular);
    long long prou_2 = ExpMod(prou_32, 16, modular);

    long long group_th;
    for(int i=0; i<m; i++){
        FFT_data_in[i] = i;
        cout << "FFT_data_in" << i << " = " << FFT_data_in[i] << endl;
    }

    cout << "----------stage0_data_in-----------" << endl;
    for(int i=0; i<16; i++){
        for(int j=0; j<2; j++){
            stage0_data_in[i][j] = FFT_data_in[i+16*j];
            cout << "stage0_data_in[" << i << "][" << j << "]" << " = " << stage0_data_in[i][j] << endl;
        }
    }

    cout << "----------stage0_fft------------" << endl;
    for(int i=0; i<16; i++){
        for(int j=0; j<2; j++){
            FFT_no_bit_reverse(stage0_data_out_tmp[i], stage0_data_in[i], radix_2, prou_2, modular);
            cout << "stage0_data_out_tmp[" << i << "][" << j << "]" << " = " << stage0_data_out_tmp[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    BitOperate BU;
    for(int i=0; i<16; i++){
       for(int j=0; j<2; j++){
            long long index_rev = BU.BitReserve(j, 1);
            long long exp_prou32 = i*index_rev;
            stage0_data_out_tmp[i][j] = MulMod(stage0_data_out_tmp[i][j] ,ExpMod(prou_32, exp_prou32, modular), modular);
            cout << "stage0_data_out_tmp_twiddle[" << i << "][" << j << "]" << " = " << stage0_data_out_tmp[i][j] << endl;
        }
    }

    cout << "----------stage1_data_in-----------" << endl;
    for(int i=0; i<2; i++){
        cout << "--------------------------------" << endl;
        for(int j=0; j<16; j++){
            stage1_data_in[i][j] = stage0_data_out_tmp[j][i];
            cout << "stage1_data_in[" << i << "][" << j << "]" << " = " << stage1_data_in[i][j] << endl;
        }
    }


    cout << "-----------stage1_fft----------" << endl;
    for(int i=0; i<2; i++){
        cout << "--------------------------------" << endl;
        for(int j=0; j<16; j++){
            FFT_no_bit_reverse(stage1_data_out_tmp[i], stage1_data_in[i], radix_16, prou_16, modular);
            cout << "stage1_data_out_tmp[" << i << "][" << j << "]" << " = " << stage1_data_out_tmp[i][j] << endl;
        }
    }

    cout << "-----------fft out----------" << endl;
    for(int i=0; i<2; i++){
        for(int j=0; j<16; j++){
            FFT_out_final[16*i+j] = stage1_data_out_tmp[i][j];
            cout << "FFT_out_final" << 16*i+j << " = " << FFT_out_final[16*i+j] << endl;
        }
    }

    cout << "-----------FFT_out_final_bit_reverse------------" << endl;
    long long FFT_out_final_bit_reverse[m];
    for(int i=0; i<m;i++){
        long long index_rev = BU.BitReserve(i, 5);
        //cout << "index_rev = " << index_rev << endl;
        FFT_out_final_bit_reverse[index_rev] = FFT_out_final[i];
    }
    for(int i=0; i<m; i++)
        cout << FFT_out_final_bit_reverse[i] << endl;

    long long DFT_ans[m];
    DFT(DFT_ans, FFT_data_in, m, prou_32, modular);
    cout << "-------DFT---------------" << endl;
    for(int i=0; i<m; i++)
        cout << DFT_ans[i] << endl;

    for(int i=0; i<m;i++){
        if(FFT_out_final_bit_reverse[i] == DFT_ans[i]){
            cout << "correct !! " << FFT_out_final_bit_reverse[i] << " = " << DFT_ans[i] << endl;
        } else{
            cout << "failed !! " << FFT_out_final_bit_reverse[i] << " != " << DFT_ans[i] << endl;
        }
    }
    return 0;
}