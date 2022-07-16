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

    long long m=16;
    long long radix_4 = 4;
    long long modular = 257;
    long long FFT_data_in[m];

    long long stage0_BU_input[4][radix_4];
    long long stage0_BU_output[4][radix_4];

    long long stage1_BU_input[4][radix_4];
    long long stage1_BU_output[4][radix_4];

    long long FFT_out_final[m];

	long long prou_16 = find_prou(m, modular);
    long long prou_4 = ExpMod(prou_16, 4, modular);
	

    cout << "prou_16 = " << prou_16 << endl;
    cout << "prou_4 = " << prou_4 << endl;

    cout << "-------FFT_data_in--------" << endl;
	for(int i=0; i<m; i++){
		FFT_data_in[i] = i;
		cout << "FFT_data_in " << i << " = " << FFT_data_in[i] << endl;
	}

    cout << "----------stage 0 input------------" << endl;
    for(int i=0; i<4 ; i++){
        for(int j=0; j<4 ; j++){
            stage0_BU_input[i][j] = FFT_data_in[i+4*j];
            cout << "stage0_BU_input[" << i << "][" << j << "]" << " = " << stage0_BU_input[i][j] << endl;
        }
    }

    cout << "-----------------------" << endl;
    for(int i=0; i<4 ; i++){
        cout << "-----------------------" << endl;
        FFT_no_bit_reverse(stage0_BU_output[i], stage0_BU_input[i], radix_4, prou_4, modular);
        for(int j=0; j<4; j++){
            cout << "stage0_BU_output[" << i << "][" << j << "]" << " = " << stage0_BU_output[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    BitOperate BU;
    for(int i=0; i<radix_4; i++){
        for(int j=0; j<radix_4; j++){
            long long index_rev = BU.BitReserve(j, 2);
            long long exp_prou16 = i*index_rev;
            stage0_BU_output[i][j] = MulMod(stage0_BU_output[i][j] ,ExpMod(prou_16, exp_prou16, modular), modular);
        } 
    }

    cout << "------------stage 1 input ---------" << endl;
    for(int i=0; i<4; i++){
        cout << "-----------------------" << endl;
        for(int j=0; j<4; j++){
           stage1_BU_input[i][j] = stage0_BU_output[j][i];
           cout << "stage1_BU_input[" << i << "][" << j << "]" << " = " << stage1_BU_input[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    for(int i=0; i<4; i++){
        FFT_no_bit_reverse(stage1_BU_output[i], stage1_BU_input[i], radix_4, prou_4, modular);
        for(int j=0; j<4; j++){
            cout << "stage1_BU_input[" << i << "][" << j << "]" << " = " << stage1_BU_output[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            FFT_out_final[i*4+j] = stage1_BU_output[i][j];
            cout << "FFT_out_final[" << i << "][" << j << "]" << " = " << FFT_out_final[i*4+j] << endl;
        }
    }
    cout << "-----------FFT_out_final_bit_reverse------------" << endl;
    long long FFT_out_final_bit_reverse[m];
    for(int i=0; i<m;i++){
        long long index_rev = BU.BitReserve(i, 4);
        //cout << "index_rev = " << index_rev << endl;
        FFT_out_final_bit_reverse[index_rev] = FFT_out_final[i];
    }
    for(int i=0; i<m; i++)
        cout << FFT_out_final_bit_reverse[i] << endl;

    long long DFT_ans[m];
    DFT(DFT_ans, FFT_data_in, m, prou_16, modular);
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