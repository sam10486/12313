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
    long long modular = 257;
    long long FFT_data_in[m];

    long long stage0_radix = radix_2;
    long long stage0_BU_pair = m/radix_2; //8
    long long stage0_BU_num = m/2; // 8
    

    long long stage1_radix = radix_2;
    long long stage1_BU_pair = stage0_BU_pair / 2; // 4
    long long stage1_BU_num = stage0_BU_num;    // 8
    long long stage1_group = 2;

    long long stage2_radix = radix_2;
    long long stage2_BU_pair = stage1_BU_pair / 2; // 2
    long long stage2_BU_num = stage1_BU_num; //8
    long long stage2_group = 4;

    long long stage3_radix = radix_2;
    long long stage3_BU_pair = stage2_BU_pair / 2; // 1
    long long stage3_BU_num = stage2_BU_num; //8
    long long stage3_group = 8;

    long long FFT_data_BU[stage0_BU_num][radix_2];
    long long FFT_out_BU[stage0_BU_num][radix_2];

    long long FFT_stage1_input_BU_tmp[m];
    long long FFT_stage1_input_BU[stage1_BU_num][radix_2];
    long long FFT_stage1_output_BU[stage1_BU_num][radix_2];

    long long FFT_stage2_input_BU_tmp[m];
    long long FFT_stage2_input_BU[stage2_BU_num][radix_2];
    long long FFT_stage2_output_BU[stage2_BU_num][radix_2];


    long long FFT_stage3_input_BU_tmp[m];
    long long FFT_stage3_input_BU[stage3_BU_num][radix_2];
    long long FFT_stage3_output_BU[stage3_BU_num][radix_2];

    long long FFT_out_final[m];
    
	long long prou_16 = find_prou(m, modular);
    long long prou_2 = ExpMod(prou_16, 8, modular);
	

    cout << "prou_16 = " << prou_16 << endl;
    cout << "prou_2 = " << prou_2 << endl;

    cout << "-------FFT_data_in--------" << endl;
	for(int i=0; i<m; i++){
		FFT_data_in[i] = i;
		cout << "FFT_data_in " << i << " = " << FFT_data_in[i] << endl;
	}

    cout << "-------------------------------" << endl;
    for(int i=0; i<stage0_BU_num; i++){
        for(int j=0; j<stage0_radix; j++){
            FFT_data_BU[i][j] = FFT_data_in[i+stage0_BU_pair*j];
            cout << "FFT_data_BU[" << i << "][" << j << "]" << " = " << FFT_data_BU[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    for(int i=0; i<stage0_BU_num; i++){
        FFT_no_bit_reverse(FFT_out_BU[i], FFT_data_BU[i], stage0_radix, prou_2, modular);
        for(int j=0; j<stage0_radix; j++){
            cout << "FFT_out_BU[" << i << "][" << j << "]" << " = " << FFT_out_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    BitOperate BU;
    for(int i=0; i<stage0_BU_num; i++){
       for(int j=0; j<stage0_radix; j++){
            long long index_rev = BU.BitReserve(j, 1);
            long long exp_prou16 = i*index_rev;
            FFT_out_BU[i][j] = MulMod(FFT_out_BU[i][j] ,ExpMod(prou_16, exp_prou16, modular), modular);
            cout << "FFT_out_BU[" << i << "][" << j << "]" << " = " << FFT_out_BU[i][j] << endl;
        }
    }
    cout << "----------stage 1 input tmp---------------" << endl;
    for(int i=0; i<stage0_radix; i++){
        for(int j=0; j<stage0_BU_num; j++){
            FFT_stage1_input_BU_tmp[i*stage0_BU_num+j] = FFT_out_BU[j][i];
            cout << "FFT_stage1_input_BU_tmp[" << i*stage0_BU_num+j << "]" << " = " << FFT_stage1_input_BU_tmp[i*stage0_BU_num+j] << endl;
        }
    }
    cout << "------------stage1_input----------------" << endl;
    for(int i=0; i< (stage1_BU_num/stage1_group) ; i++){
        cout << "--------------------------" << endl;
        for(int j=0; j <stage1_radix ; j++){
            for(int k=0; k < stage1_group; k++){
                FFT_stage1_input_BU[i+stage1_BU_pair*k][j] = FFT_stage1_input_BU_tmp[i+stage1_BU_pair*j+(m/stage1_radix)*k];
            }
        }
    }
    cout << "-------------------------------" << endl;
    for(int i=0; i < stage1_BU_num; i++){
        FFT_no_bit_reverse(FFT_stage1_output_BU[i], FFT_stage1_input_BU[i], stage1_radix, prou_2, modular);
        for(int j=0; j<stage1_radix; j++){
            cout << "FFT_stage1_output_BU[" << i << "][" << j << "]" << " = " << FFT_stage1_output_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    for(int i=0; i < (stage1_BU_num/stage1_group); i++){
       for(int j=0; j<stage1_radix; j++){
            long long index_rev = BU.BitReserve(j, 1);
            long long exp_prou16;
            exp_prou16 = stage1_group*i*index_rev;
            for(int k=0; k < stage1_group; k++){
                FFT_stage1_output_BU[i+k*stage1_BU_pair][j] = MulMod(FFT_stage1_output_BU[i+k*stage1_BU_pair][j] ,ExpMod(prou_16, exp_prou16, modular), modular);
            }
        }
    }
    cout << "--------stage 2 input tmp-----------" << endl;
    for(int i=0; i<stage1_radix; i++){
        for(int j=0; j < (stage1_BU_num/stage1_group); j++){
            for(int k=0; k < stage1_group; k++){
                FFT_stage2_input_BU_tmp[i*stage1_BU_pair+j+k*stage1_BU_num] = FFT_stage1_output_BU[j+k*stage1_BU_pair][i];
                cout << "FFT_stage2_input_BU_tmp[" << i*stage1_BU_pair+j+k*stage1_BU_num << "]" << " = " << FFT_stage2_input_BU_tmp[i*stage1_BU_pair+j+k*stage1_BU_num] << endl;
            }
        }
    }
    cout << "------------stage2_input----------------" << endl;
    for(int i=0; i< (stage2_BU_num/stage2_group) ; i++){
        cout << "--------------------------" << endl;
        for(int j=0; j<stage2_radix; j++){
            for(int k=0; k < stage2_group; k++){
                FFT_stage2_input_BU[i+k*stage2_BU_pair][j] = FFT_stage2_input_BU_tmp[i+stage2_BU_pair*j+k*stage2_BU_pair*(stage2_BU_num/stage2_group)];
                cout << "FFT_stage2_input_BU[" << i+k*stage2_BU_pair << "][" << j << "]" << " = " << FFT_stage2_input_BU[i+k*stage2_BU_pair][j] << endl;
            }
        }
    }
    cout << "-------------------------------" << endl;
    for(int i=0; i<stage2_BU_num; i++){
        FFT_no_bit_reverse(FFT_stage2_output_BU[i], FFT_stage2_input_BU[i], stage2_radix, prou_2, modular);
        for(int j=0; j<stage2_radix; j++){
            cout << "FFT_stage2_output_BU[" << i << "][" << j << "]" << " = " << FFT_stage2_output_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    for(int i=0; i<(stage2_BU_num/stage2_group); i++){
       for(int j=0; j<stage2_radix; j++){
            long long index_rev = BU.BitReserve(j, 1);
            long long exp_prou16;
            exp_prou16 = stage2_group*i*index_rev;
            for(int k=0; k < stage2_group; k++){
                FFT_stage2_output_BU[i+k*stage2_BU_pair][j] = MulMod(FFT_stage2_output_BU[i+k*stage2_BU_pair][j] ,ExpMod(prou_16, exp_prou16, modular), modular);
            }
        }
    }

    cout << "--------stage 3 input tmp-----------" << endl;
    for(int i=0; i<stage2_radix; i++){
        for(int j=0; j<stage2_radix; j++){
            for(int k=0; k < stage2_group; k++){
                FFT_stage3_input_BU_tmp[i*2+j+k*stage2_group] = FFT_stage2_output_BU[j+k*stage2_BU_pair][i];
            }
            //FFT_stage3_input_BU_tmp[i*2+j] = FFT_stage2_output_BU[j][i];
            //FFT_stage3_input_BU_tmp[i*2+j+4] = FFT_stage2_output_BU[j+2][i];
            //FFT_stage3_input_BU_tmp[i*2+j+8] = FFT_stage2_output_BU[j+4][i];
            //FFT_stage3_input_BU_tmp[i*2+j+12] = FFT_stage2_output_BU[j+6][i];
            cout << "FFT_stage3_input_BU_tmp[" << i*2+j << "]" << " = " << FFT_stage3_input_BU_tmp[i*2+j] << endl;
            //cout << "FFT_stage2_input_BU_tmp[" << i*2+j+4 << "]" << " = " << FFT_stage3_input_BU_tmp[i*2+j+4] << endl;
            //cout << "FFT_stage2_input_BU_tmp[" << i*2+j+8 << "]" << " = " << FFT_stage3_input_BU_tmp[i*2+j+8] << endl;
            //cout << "FFT_stage2_input_BU_tmp[" << i*2+j+12 << "]" << " = " << FFT_stage3_input_BU_tmp[i*2+j+12] << endl;
        }
    }

    cout << "------------stage3_input----------------" << endl;
    for(int i=0; i< (stage3_BU_num/stage3_group); i++){
        cout << "--------------------------" << endl;
        for(int j=0; j<stage3_radix; j++){
            for(int k=0; k<stage3_group; k++){
                FFT_stage3_input_BU[i+k*stage3_BU_pair][j] = FFT_stage3_input_BU_tmp[i+j+k*stage3_radix];
            }
            /*FFT_stage3_input_BU[i][j] = FFT_stage3_input_BU_tmp[i+j];
            FFT_stage3_input_BU[i+1][j] = FFT_stage3_input_BU_tmp[i+j+2];
            FFT_stage3_input_BU[i+2][j] = FFT_stage3_input_BU_tmp[i+j+4];
            FFT_stage3_input_BU[i+3][j] = FFT_stage3_input_BU_tmp[i+j+6];
            FFT_stage3_input_BU[i+4][j] = FFT_stage3_input_BU_tmp[i+j+8];
            FFT_stage3_input_BU[i+5][j] = FFT_stage3_input_BU_tmp[i+j+10];
            FFT_stage3_input_BU[i+6][j] = FFT_stage3_input_BU_tmp[i+j+12];
            FFT_stage3_input_BU[i+7][j] = FFT_stage3_input_BU_tmp[i+j+14];*/
            //cout << "FFT_stage3_input_BU[" << i << "][" << j << "]" << " = " << FFT_stage3_input_BU[i][j] << endl;
            //cout << "FFT_stage3_input_BU[" << i+1 << "][" << j << "]" << " = " << FFT_stage3_input_BU[i+1][j] << endl;
            //cout << "FFT_stage3_input_BU[" << i+2 << "][" << j << "]" << " = " << FFT_stage3_input_BU[i+2][j] << endl;
            //cout << "FFT_stage3_input_BU[" << i+3 << "][" << j << "]" << " = " << FFT_stage3_input_BU[i+3][j] << endl;
        }
    }
    

    cout << "-------------------------------" << endl;
    for(int i=0; i<stage3_BU_num; i++){
        FFT_no_bit_reverse(FFT_stage3_output_BU[i], FFT_stage3_input_BU[i], stage3_radix, prou_2, modular);
        for(int j=0; j<stage3_radix; j++){
            cout << "FFT_stage3_output_BU[" << i << "][" << j << "]" << " = " << FFT_stage3_output_BU[i][j] << endl;
        }
    }
    cout << "-------------------------------" << endl;
    for(int i=0; i<stage3_BU_num; i++){
        for(int j=0; j<stage3_radix; j++){
            FFT_out_final[i*stage3_radix+j] = FFT_stage3_output_BU[i][j];
            cout << "FFT_out_final[" << i << "][" << j << "]" << " = " << FFT_out_final[i*stage3_radix+j] << endl;
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