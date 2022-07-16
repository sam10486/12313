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

    long long m=8192;
    long long radix_16 = 16;
    long long radix_2 = 2;
    long long modular = 65537;
    long long FFT_data_in[m];


    long long FFT_data_BU[4096][radix_2];
    long long FFT_out_BU[4096][radix_2];

    long long FFT_stage1_input_BU_tmp[m];
    long long FFT_stage1_input_BU[512][radix_16];
    long long FFT_stage1_output_BU[512][radix_16];

    long long FFT_stage2_input_BU_tmp[m];
    long long FFT_stage2_input_BU[512][radix_16];
    long long FFT_stage2_output_BU[512][radix_16];

    long long FFT_stage3_input_BU_tmp[m];
    long long FFT_stage3_input_BU[512][radix_16];
    long long FFT_stage3_output_BU[512][radix_16];


    long long FFT_out_final[m];
    
	long long prou_8192 = find_prou(m, modular);
    long long prou_16 = ExpMod(prou_8192, 512, modular);
    long long prou_2 = ExpMod(prou_8192, 4096, modular);
	

    cout << "prou_8192 = " << prou_8192 << endl;
    cout << "prou_16 = " << prou_16 << endl;
    cout << "prou_2 = " << prou_2 << endl;

    cout << "-------FFT_data_in--------" << endl;
	for(int i=0; i<m; i++){
		FFT_data_in[i] = 1;
		cout << "FFT_data_in " << i << " = " << FFT_data_in[i] << endl;
	}

    cout << "-------------------------------" << endl;
    for(int i=0; i<4096; i++){
        for(int j=0; j<2; j++){
            FFT_data_BU[i][j] = FFT_data_in[i+4096*j];
            cout << "FFT_data_BU[" << i << "][" << j << "]" << " = " << FFT_data_BU[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    for(int i=0; i<4096; i++){
        FFT_no_bit_reverse(FFT_out_BU[i], FFT_data_BU[i], radix_2, prou_2, modular);
        for(int j=0; j<2; j++){
            cout << "FFT_out_BU[" << i << "][" << j << "]" << " = " << FFT_out_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    BitOperate BU;
    for(int i=0; i<4096; i++){
       for(int j=0; j<radix_2; j++){
            long long index_rev = BU.BitReserve(j, 1);
            long long exp_prou8192 = i*index_rev;
            FFT_out_BU[i][j] = MulMod(FFT_out_BU[i][j] ,ExpMod(prou_8192, exp_prou8192, modular), modular);
            cout << "FFT_out_BU[" << i << "][" << j << "]" << " = " << FFT_out_BU[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    for(int i=0; i<2; i++){
        for(int j=0; j<4096; j++){
            FFT_stage1_input_BU_tmp[i*4096+j] = FFT_out_BU[j][i];
            cout << "FFT_stage1_input_BU_tmp[" << i*4096+j << "]" << " = " << FFT_stage1_input_BU_tmp[i*4096+j] << endl;
        }
    }
    cout << "------------stage1_input----------------" << endl;
    for(int k=0; k<32; k++){
        for(int i=0; i<16; i++){
        cout << "--------------------------" << endl;
            for(int j=0; j<16; j++){
               FFT_stage1_input_BU[i+16*k][j] = FFT_stage1_input_BU_tmp[i+16*j+256*k];
                cout << "FFT_stage1_input_BU[" << i+16*k << "][" << j << "]" << " = " << FFT_stage1_input_BU[i+16*k][j] << endl;
            }
        }
    }
    
    cout << "-------------------------------" << endl;
    for(int i=0; i<512; i++){
        cout << "--------------------------" << endl;
        FFT_no_bit_reverse(FFT_stage1_output_BU[i], FFT_stage1_input_BU[i], radix_16, prou_16, modular);
        for(int j=0; j<16; j++){
            cout << "FFT_stage1_output_BU_fft[" << i << "][" << j << "]" << " = " << FFT_stage1_output_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    for(int k=0; k<32; k++){
        for(int i=0; i<16; i++){
            cout << "-------------------------------" << endl;
            for(int j=0; j<radix_16; j++){
                long long index_rev = BU.BitReserve(j, 4);
                long long exp_prou8192;
                exp_prou8192 = 2*i*index_rev;
                FFT_stage1_output_BU[i+16*k][j] = MulMod(FFT_stage1_output_BU[i+16*k][j] ,ExpMod(prou_8192, exp_prou8192, modular), modular);
                cout << "FFT_stage1_output_BU[" << i+16*k << "][" << j << "]" << " = " << FFT_stage1_output_BU[i+16*k][j] << endl;
                //FFT_stage1_output_BU[i+16][j] = MulMod(FFT_stage1_output_BU[i+16][j] ,ExpMod(prou_8192, exp_prou8192, modular), modular);
            }
        } 
    }
    
    cout << "-------------stage 2 input-------------" << endl;
    for(int k=0; k<32; k++){
       for(int i=0; i<16; i++){
           cout << "-------------------------------" << endl;
            for(int j=0; j<16; j++){
                FFT_stage2_input_BU[i+16*k][j] = FFT_stage1_output_BU[j+16*k][i];
                //FFT_stage2_input_BU[i+16][j] = FFT_stage1_output_BU[j+16][i];
                //cout << "FFT_stage2_input_BU[" << i << "][" << j << "]" << " = " << FFT_stage2_input_BU[i][j] << endl;
                cout << "FFT_stage2_input_BU[" << i+16*k << "][" << j << "]" << " = " << FFT_stage2_input_BU[i+16*k][j] << endl;
            }
        } 
    }
    
    cout << "-------------------------------" << endl;
    for(int i=0; i<512; i++){
        cout << "--------------------------" << endl;
        FFT_no_bit_reverse(FFT_stage2_output_BU[i], FFT_stage2_input_BU[i], radix_16, prou_16, modular);
        for(int j=0; j<16; j++){
            cout << "FFT_stage2_output_BU[" << i << "][" << j << "]" << " = " << FFT_stage2_output_BU[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    for(int k=0; k<32; k++){
       for(int i=0; i<16; i++){
           cout << "--------------------------" << endl;
            for(int j=0; j<radix_16; j++){
                long long index_rev = BU.BitReserve(j, 4);
                long long exp_prou8192;
                exp_prou8192 = 4*i*index_rev;
                FFT_stage2_output_BU[i+16*k][j] = MulMod(FFT_stage2_output_BU[i+16*k][j] ,ExpMod(prou_8192, exp_prou8192, modular), modular);
                //cout << "FFT_stage2_output_BU[" << i+16*k << "][" << j << "]" << " = " << FFT_stage2_output_BU[i+16*k][j] << endl;
                //FFT_stage1_output_BU[i+16][j] = MulMod(FFT_stage1_output_BU[i+16][j] ,ExpMod(prou_8192, exp_prou8192, modular), modular);
            }
        }   
    }
      
    
    cout << "-------------stage 3 input-------------" << endl;
    for(int k=0; k<32; k++){
       for(int i=0; i<16; i++){
            cout << "-------------------------------" << endl;
            for(int j=0; j<16; j++){
                FFT_stage3_input_BU[i+16*k][j] = FFT_stage2_output_BU[j*16+k][i];
                //FFT_stage3_input_BU[i+16*1][j] = FFT_stage2_output_BU[16*j+1][i];
                //FFT_stage3_input_BU[i+16][j] = FFT_stage2_output_BU[j+16][i];
                //cout << "FFT_stage3_input_BU[" << i << "][" << j << "]" << " = " << FFT_stage3_input_BU[i][j] << endl;
                cout << "FFT_stage3_input_BU[" << i+16*k << "][" << j << "]" << " = " << FFT_stage3_input_BU[i+16*k][j] << endl;
            }
        }  
    }
       
    

    /*cout << "-------------------------------" << endl;
    for(int i=0; i<512; i++){
        cout << "--------------------------" << endl;
        FFT_no_bit_reverse(FFT_stage3_output_BU[i], FFT_stage3_input_BU[i], radix_16, prou_16, modular);
        for(int j=0; j<16; j++){
            cout << "FFT_stage3_output_BU[" << i << "][" << j << "]" << " = " << FFT_stage3_output_BU[i][j] << endl;
        }
    }



    
    cout << "-------------------------------" << endl;
    for(int i=0; i<512; i++){
        for(int j=0; j<16; j++){
            FFT_out_final[i*16+j] = FFT_stage2_output_BU[i][j];
            cout << "FFT_out_final[" << i*16+j << "]" << " = " << FFT_out_final[i*16+j] << endl;
        }
    }
    cout << "-----------FFT_out_final_bit_reverse------------" << endl;
    long long FFT_out_final_bit_reverse[m];
    for(int i=0; i<m;i++){
        long long index_rev = BU.BitReserve(i, 9);
        //cout << "index_rev = " << index_rev << endl;
        FFT_out_final_bit_reverse[index_rev] = FFT_out_final[i];
    }
    for(int i=0; i<m; i++)
        cout << FFT_out_final_bit_reverse[i] << endl;
    
    long long DFT_ans[m];
    DFT(DFT_ans, FFT_data_in, m, prou_8192, modular);
    cout << "-------DFT---------------" << endl;
    for(int i=0; i<m; i++)
        cout << DFT_ans[i] << endl;

    for(int i=0; i<m;i++){
        if(FFT_out_final_bit_reverse[i] == DFT_ans[i]){
            cout << "correct !! " << i << "th: " << FFT_out_final_bit_reverse[i] << " = " 
            << DFT_ans[i] << endl;
        } else{
            cout << "failed !! " << i << "th: " << FFT_out_final_bit_reverse[i] << " != " << DFT_ans[i] << endl;
        }
    }*/


    return 0;
}