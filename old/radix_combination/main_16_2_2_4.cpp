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
    long long radix_2 = 2;
    long long radix_4 = 4;
    long long modular = 257;
    long long FFT_data_in[m];


    long long FFT_data_BU[8][radix_2];
    long long FFT_out_BU[8][radix_2];

    long long FFT_stage1_input_BU_tmp[m];
    long long FFT_stage1_input_BU[8][radix_2];
    long long FFT_stage1_output_BU[8][radix_2];

    long long FFT_stage2_input_BU_tmp[m];
    long long FFT_stage2_input_BU[4][radix_4];
    long long FFT_stage2_output_BU[4][radix_4];


    long long FFT_out_final[m];
    
	long long prou_16 = find_prou(m, modular);
    long long prou_4 = ExpMod(prou_16, 4, modular);
    long long prou_2 = ExpMod(prou_16, 8, modular);
	

    cout << "prou_16 = " << prou_16 << endl;
    cout << "prou_4 = " << prou_4 << endl;
    cout << "prou_2 = " << prou_2 << endl;

    cout << "-------FFT_data_in--------" << endl;
	for(int i=0; i<m; i++){
		FFT_data_in[i] = i;
		cout << "FFT_data_in " << i << " = " << FFT_data_in[i] << endl;
	}

    cout << "-------------------------------" << endl;
    for(int i=0; i<8; i++){
        for(int j=0; j<2; j++){
            FFT_data_BU[i][j] = FFT_data_in[i+8*j];
            cout << "FFT_data_BU[" << i << "][" << j << "]" << " = " << FFT_data_BU[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    for(int i=0; i<8; i++){
        FFT_no_bit_reverse(FFT_out_BU[i], FFT_data_BU[i], radix_2, prou_2, modular);
        for(int j=0; j<2; j++){
            cout << "FFT_out_BU[" << i << "][" << j << "]" << " = " << FFT_out_BU[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    BitOperate BU;
    for(int i=0; i<8; i++){
       for(int j=0; j<radix_2; j++){
            long long index_rev = BU.BitReserve(j, 1);
            long long exp_prou16 = i*index_rev;
            FFT_out_BU[i][j] = MulMod(FFT_out_BU[i][j] ,ExpMod(prou_16, exp_prou16, modular), modular);
            cout << "FFT_out_BU[" << i << "][" << j << "]" << " = " << FFT_out_BU[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    for(int i=0; i<2; i++){
        for(int j=0; j<8; j++){
            FFT_stage1_input_BU_tmp[i*8+j] = FFT_out_BU[j][i];
            cout << "FFT_stage1_input_BU_tmp[" << i*8+j << "]" << " = " << FFT_stage1_input_BU_tmp[i*8+j] << endl;
        }
    }

    cout << "------------stage1_input----------------" << endl;
    for(int i=0; i<4; i++){
        cout << "--------------------------" << endl;
        for(int j=0; j<2; j++){
            FFT_stage1_input_BU[i][j] = FFT_stage1_input_BU_tmp[i+4*j];
            FFT_stage1_input_BU[i+4][j] = FFT_stage1_input_BU_tmp[i+4*j+8];
            //cout << "FFT_stage1_input_BU[" << i << "][" << j << "]" << " = " << FFT_stage1_input_BU[i][j] << endl;
            //cout << "FFT_stage1_input_BU[" << i+4 << "][" << j << "]" << " = " << FFT_stage1_input_BU[i+4][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    for(int i=0; i<8; i++){
        FFT_no_bit_reverse(FFT_stage1_output_BU[i], FFT_stage1_input_BU[i], radix_2, prou_2, modular);
        for(int j=0; j<2; j++){
            cout << "FFT_stage1_output_BU[" << i << "][" << j << "]" << " = " << FFT_stage1_output_BU[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    for(int i=0; i<8; i++){
       for(int j=0; j<radix_2; j++){
            long long index_rev = BU.BitReserve(j, 1);
            long long exp_prou16;
            if(i<4){
                exp_prou16 = 2*i*index_rev;
                FFT_stage1_output_BU[i][j] = MulMod(FFT_stage1_output_BU[i][j] ,ExpMod(prou_16, exp_prou16, modular), modular);
                //cout << "FFT_stage1_output_BU[" << i << "][" << j << "]" << " = " << FFT_stage1_output_BU[i][j] << endl;
            }else{
                exp_prou16 = 2*(i-4)*index_rev;
                FFT_stage1_output_BU[i][j] = MulMod(FFT_stage1_output_BU[i][j] ,ExpMod(prou_16, exp_prou16, modular), modular);
                //cout << "FFT_stage1_output_BU[" << i+2 << "][" << j << "]" << " = " << FFT_stage1_output_BU[i+2][j] << endl;
            }
        }
    }

    cout << "----------stage2 input------------" << endl;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(i<2){
                FFT_stage2_input_BU[i][j] = FFT_stage1_output_BU[j][i];
                cout << "FFT_stage2_input_BU[" << i << "][" << j << "]" << " = " << FFT_stage2_input_BU[i][j] << endl;
            }else{
                FFT_stage2_input_BU[i][j] = FFT_stage1_output_BU[j+4][i-2];
                cout << "FFT_stage2_input_BU[" << i << "][" << j << "]" << " = " << FFT_stage2_input_BU[i][j] << endl;
            }    
        }
    }
    cout << "-------------------------------" << endl;
    for(int i=0; i<4; i++){
        FFT_no_bit_reverse(FFT_stage2_output_BU[i], FFT_stage2_input_BU[i], radix_4, prou_4, modular);
        for(int j=0; j<4; j++){
            cout << "FFT_stage2_output_BU[" << i << "][" << j << "]" << " = " << FFT_stage2_output_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            FFT_out_final[i*4+j] = FFT_stage2_output_BU[i][j];
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