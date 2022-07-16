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

    long long m=32;
    long long radix_8 = 8;
    long long modular = 257;
    long long FFT_data_in[m];
    long long radix_4 = 4;


    long long FFT_data_BU[radix_8][radix_4];
    long long FFT_out_BU[radix_8][radix_4];

    long long FFT_stage1_input_BU[radix_4][radix_8];
    long long FFT_stage1_output_BU[radix_4][radix_8];

    
    long long FFT_out_final[m];
    
	long long prou_32 = find_prou(m, modular);
    long long prou_8 = ExpMod(prou_32, 4, modular);
    long long prou_4 = ExpMod(prou_32, 8, modular);
	

    cout << "prou_8 = " << prou_8 << endl;
    cout << "prou_32 = " << prou_32 << endl;
    cout << "prou_4 = " << prou_4 << endl;

    cout << "-------FFT_data_in--------" << endl;
	for(int i=0; i<m; i++){
		FFT_data_in[i] = i;
		cout << "FFT_data_in " << i << " = " << FFT_data_in[i] << endl;
	}

    cout << "-------------------------------" << endl;
    for(int i=0; i<8; i++){
        for(int j=0; j<4; j++){
            FFT_data_BU[i][j] = FFT_data_in[i+8*j];
            cout << "FFT_data_BU[" << i << "][" << j << "]" << " = " << FFT_data_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    for(int i=0; i<8; i++){
        FFT_no_bit_reverse(FFT_out_BU[i], FFT_data_BU[i], radix_4, prou_4, modular);
        for(int j=0; j<4; j++){
            cout << "FFT_out_BU[" << i << "][" << j << "]" << " = " << FFT_out_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    BitOperate BU;
    for(int i=0; i<radix_8; i++){
       for(int j=0; j<radix_4; j++){
            long long index_rev = BU.BitReserve(j, 2);
            long long exp_prou32 = i*index_rev;
            FFT_out_BU[i][j] = MulMod(FFT_out_BU[i][j] ,ExpMod(prou_32, exp_prou32, modular), modular);
            cout << "FFT_out_BU[" << i << "][" << j << "]" << " = " << FFT_out_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    for(int i=0; i<4; i++){
        for(int j=0; j<8; j++){
            FFT_stage1_input_BU[i][j] = FFT_out_BU[j][i];
            cout << "FFT_stage1_input_BU[" << i << "][" << j << "]" << " = " << FFT_stage1_input_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    for(int i=0; i<4; i++){
        FFT_no_bit_reverse(FFT_stage1_output_BU[i], FFT_stage1_input_BU[i], radix_8, prou_8, modular);
        for(int j=0; j<8; j++){
            cout << "FFT_stage1_output_BU[" << i << "][" << j << "]" << " = " << FFT_stage1_output_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    for(int i=0; i<4; i++){
        for(int j=0; j<8; j++){
            FFT_out_final[i*8+j] = FFT_stage1_output_BU[i][j];
            cout << "FFT_out_final[" << i << "][" << j << "]" << " = " << FFT_out_final[i*8+j] << endl;
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