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

    long long m=256;
    long long radix_16 = 16;
    long long modular = 12289;
    long long FFT_data_in[m];
    //long long radix_8 = 8;


    long long FFT_data_BU[radix_16][radix_16];
    long long FFT_out_BU[radix_16][radix_16];

    long long FFT_stage1_input_BU[radix_16][radix_16];
    long long FFT_stage1_output_BU[radix_16][radix_16];

    
    long long FFT_out_final[m];
    
	long long prou_256 = find_prou(m, modular);
    long long prou_16 = ExpMod(prou_256, 16, modular);
    //long long prou_8 = ExpMod(prou_128, 16, modular);
	

    cout << "prou_256 = " << prou_256 << endl;
    cout << "prou_16 = " << prou_16 << endl;
   // cout << "prou_8 = " << prou_8 << endl;

    cout << "-------FFT_data_in--------" << endl;
	for(int i=0; i<m; i++){
		FFT_data_in[i] = i;
		cout << "FFT_data_in " << i << " = " << FFT_data_in[i] << endl;
	}

    cout << "-------------------------------" << endl;
    for(int i=0; i<16; i++){
        for(int j=0; j<16; j++){
            FFT_data_BU[i][j] = FFT_data_in[i+16*j];
            cout << "FFT_data_BU[" << i << "][" << j << "]" << " = " << FFT_data_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    for(int i=0; i<16; i++){
        FFT_no_bit_reverse(FFT_out_BU[i], FFT_data_BU[i], radix_16, prou_16, modular);
        for(int j=0; j<16; j++){
            cout << "FFT_out_BU[" << i << "][" << j << "]" << " = " << FFT_out_BU[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    BitOperate BU;
    for(int i=0; i<radix_16; i++){
       for(int j=0; j<radix_16; j++){
            long long index_rev = BU.BitReserve(j, 4); //how much index numbers in one BU, and reverse the index
            long long exp_prou256 = i*index_rev;
            FFT_out_BU[i][j] = MulMod(FFT_out_BU[i][j] ,ExpMod(prou_256, exp_prou256, modular), modular);
            cout << "FFT_out_BU[" << i << "][" << j << "]" << " = " << FFT_out_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    for(int i=0; i<16; i++){
        for(int j=0; j<16; j++){
            FFT_stage1_input_BU[i][j] = FFT_out_BU[j][i];
            cout << "FFT_stage1_input_BU[" << i << "][" << j << "]" << " = " << FFT_stage1_input_BU[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    for(int i=0; i<16; i++){
        FFT_no_bit_reverse(FFT_stage1_output_BU[i], FFT_stage1_input_BU[i], radix_16, prou_16, modular);
        for(int j=0; j<16; j++){
            cout << "FFT_stage1_output_BU[" << i << "][" << j << "]" << " = " << FFT_stage1_output_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    for(int i=0; i<16; i++){
        for(int j=0; j<16; j++){
            FFT_out_final[i*16+j] = FFT_stage1_output_BU[i][j];
            cout << "FFT_out_final[" << i << "][" << j << "]" << " = " << FFT_out_final[i*16+j] << endl;
        }
    }

    cout << "-----------FFT_out_final_bit_reverse------------" << endl;
    long long FFT_out_final_bit_reverse[m];
    for(int i=0; i<m;i++){
        long long index_rev = BU.BitReserve(i, 8);
        //cout << "index_rev = " << index_rev << endl;
        FFT_out_final_bit_reverse[index_rev] = FFT_out_final[i];
    }
    for(int i=0; i<m; i++)
        cout << FFT_out_final_bit_reverse[i] << endl;
    
    long long DFT_ans[m];
    DFT(DFT_ans, FFT_data_in, m, prou_256, modular);
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