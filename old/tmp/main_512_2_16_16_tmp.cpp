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

    long long m=512;
    long long radix_16 = 16;
    long long radix_2 = 2;
    long long modular = 12289;
    long long FFT_data_in[m];


    long long FFT_stage0_BU_input[256][radix_2];
    long long FFT_stage0_BU_output[256][radix_2];

    long long FFT_stage1_input_BU_tmp[m];
    long long FFT_stage1_input_BU[32][radix_16];
    long long FFT_stage1_output_BU[32][radix_16];

    long long FFT_stage2_input_BU_tmp[m];
    long long FFT_stage2_input_BU[32][radix_16];
    long long FFT_stage2_output_BU[32][radix_16];


    long long FFT_out_final[m];
    
	long long prou_512 = find_prou(m, modular);
    long long prou_16 = ExpMod(prou_512, 32, modular);
    long long prou_2 = ExpMod(prou_512, 256, modular);
	

    cout << "prou_512 = " << prou_512 << endl;
    cout << "prou_16 = " << prou_16 << endl;
    cout << "prou_2 = " << prou_2 << endl;

    cout << "-------FFT_data_in--------" << endl;
	for(int i=0; i<m; i++){
		FFT_data_in[i] = i;
		cout << "FFT_data_in " << i << " = " << FFT_data_in[i] << endl;
	}

    cout << "-------------------------------" << endl;
    for(int i=0; i<256; i++){
        for(int j=0; j<2; j++){
            FFT_stage0_BU_input[i][j] = FFT_data_in[i+256*j];
            cout << "FFT_stage0_BU_input[" << i << "][" << j << "]" << " = " << FFT_stage0_BU_input[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    for(int i=0; i<256; i++){
        FFT_no_bit_reverse(FFT_stage0_BU_output[i], FFT_stage0_BU_input[i], radix_2, prou_2, modular);
        for(int j=0; j<2; j++){
            cout << "FFT_stage0_BU_output[" << i << "][" << j << "]" << " = " << FFT_stage0_BU_output[i][j] << endl;
        }
    }

    /*cout << "-------------------------------" << endl;
    BitOperate BU;
    for(int i=0; i<256; i++){
       for(int j=0; j<radix_2; j++){
            long long index_rev = BU.BitReserve(j, 1);
            long long exp_prou512 = i*index_rev;
            FFT_stage0_BU_output[i][j] = MulMod(FFT_stage0_BU_output[i][j] ,ExpMod(prou_512, exp_prou512, modular), modular);
            cout << "FFT_out_BU[" << i << "][" << j << "]" << " = " << FFT_out_BU[i][j] << endl;
        }
    }
    cout << "------------stage1_input----------------" << endl;
    for(int i=0; i<16; i++){
        cout << "--------------------------" << endl;
        for(int j=0; j<16; j++){
            FFT_stage1_input_BU[][] = FFT_stage0_BU_output[][];
        }
    }*/

    return 0;
}