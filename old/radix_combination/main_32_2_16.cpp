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
    long long radix_16 = 16;
    long long modular = 257;
    long long FFT_data_in[m];
    long long radix_2 = 2;


    long long FFT_data_BU[radix_16][radix_2];
    long long FFT_out_BU[radix_16][radix_2];

    long long FFT_stage1_input_BU[radix_2][radix_16];
    long long FFT_stage1_output_BU[radix_2][radix_16];

    
    long long FFT_out_final[m];
    
	long long prou_32 = find_prou(m, modular);
    long long prou_16 = ExpMod(prou_32, 2, modular);
    long long prou_2 = ExpMod(prou_32, 16, modular);
	

    cout << "prou_16 = " << prou_16 << endl;
    cout << "prou_32 = " << prou_32 << endl;
    cout << "prou_2 = " << prou_2 << endl;

    cout << "-------FFT_data_in--------" << endl;
	for(int i=0; i<m; i++){
		FFT_data_in[i] = i;
		cout << "FFT_data_in " << i << " = " << FFT_data_in[i] << endl;
	}

    cout << "-------------------------------" << endl;
    for(int i=0; i<16; i++){
        for(int j=0; j<2; j++){
            FFT_data_BU[i][j] = FFT_data_in[i+16*j];
            cout << "FFT_data_BU[" << i << "][" << j << "]" << " = " << FFT_data_BU[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    for(int i=0; i<16; i++){
        FFT_no_bit_reverse(FFT_out_BU[i], FFT_data_BU[i], radix_2, prou_2, modular);
        for(int j=0; j<2; j++){
            cout << "FFT_out_BU[" << i << "][" << j << "]" << " = " << FFT_out_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    for(int i=0; i<16; i++){
        FFT_out_BU[i][1] = MulMod(FFT_out_BU[i][1], ExpMod(prou_32, i, modular), modular);
        for(int j=0; j<2; j++){
            cout << "FFT_out_BU[" << i << "][" << j << "]" << " = " << FFT_out_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    for(int i=0; i<2; i++){
        for(int j=0; j<16; j++){
            FFT_stage1_input_BU[i][j] = FFT_out_BU[j][i];
            cout << "FFT_stage1_input_BU[" << i << "][" << j << "]" << " = " << FFT_stage1_input_BU[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    for(int i=0; i<2; i++){
        FFT_no_bit_reverse(FFT_stage1_output_BU[i], FFT_stage1_input_BU[i], radix_16, prou_16, modular);
        for(int j=0; j<16; j++){
            cout << "FFT_stage1_output_BU[" << i << "][" << j << "]" << " = " << FFT_stage1_output_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    for(int i=0; i<2; i++){
        for(int j=0; j<16; j++){
            FFT_out_final[i*16+j] = FFT_stage1_output_BU[i][j];
            cout << "FFT_out_final[" << i << "][" << j << "]" << " = " << FFT_out_final[i*16+j] << endl;
        }
    }
    cout << "-----------FFT_out_final_bit_reverse------------" << endl;
    long long FFT_out_final_bit_reverse[m];
    BitOperate BU;
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