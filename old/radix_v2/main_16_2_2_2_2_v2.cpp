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


    long long stage0_BU_input[8][radix_2];
    long long stage0_BU_output[8][radix_2];

    long long stage1_BU_input[8][radix_2];
    long long stage1_BU_output[8][radix_2];
    long long stage1_BU_group_output[4][4];

    long long stage2_BU_input[8][radix_2];
    long long stage2_BU_output[8][radix_2];

    long long stage3_BU_input[8][radix_2];
    long long stage3_BU_output[8][radix_2];

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

    cout << "-----------stage 0 input---------------" << endl;
    for(int i=0; i<8; i++){
        for(int j=0; j<2; j++){
            stage0_BU_input[i][j] = FFT_data_in[i+8*j];
            cout << "stage0_BU_input[" << i << "][" << j << "]" << " = " << stage0_BU_input[i][j] << endl;
        }
    }
    cout << "-----------------------" << endl;
    for(int i=0; i<8; i++){
        cout << "-----------------------" << endl;
        FFT_no_bit_reverse(stage0_BU_output[i], stage0_BU_input[i], radix_2, prou_2, modular);
        for(int j=0; j<2; j++){
            cout << "stage0_BU_output[" << i << "][" << j << "]" << " = " << stage0_BU_output[i][j] << endl;
        }
    }
   
    cout << "----------------------------" << endl;
    BitOperate BU;
    for(int i=0; i<8; i++){
        cout << "-----------------------" << endl;
        for(int j=0; j<2; j++){
            long long index_rev = BU.BitReserve(j, 1);
            long long exp_prou16 = i*index_rev;
            stage0_BU_output[i][j] = MulMod(stage0_BU_output[i][j] ,ExpMod(prou_16, exp_prou16, modular), modular);
             cout << "stage0_BU_output[" << i << "][" << j << "]" << " = " << stage0_BU_output[i][j] << endl;
        }
    }

    cout << "------------stage 1 input ---------" << endl;
    for(int k=0; k<4; k++){
        for(int i=0; i<2; i++){
            cout << "-----------------------" << endl;
            for(int j=0; j<2; j++){
                stage1_BU_input[i+2*k][j] = stage0_BU_output[j+2*k][i];
                cout << "stage1_BU_input[" << i+2*k << "][" << j << "]" << " = " << stage1_BU_input[i+2*k][j] << endl;
            }
        }  
    }

    cout << "-------------------------------" << endl;
    for(int k=0; k<4; k++){
        for(int i=0; i<2; i++){
            cout << "-----------------------" << endl;
            FFT_no_bit_reverse(stage1_BU_output[i+2*k], stage1_BU_input[i+2*k], radix_2, prou_2, modular);
            for(int j=0; j<2; j++){
                cout << "stage1_BU_output[" << i+2*k << "][" << j << "]" << " = " << stage1_BU_output[i+2*k][j] << endl;
            }
        }  
    }

    cout << "----------------------------" << endl;
    for(int k=0; k<4; k++){       
        for(int i=0; i<2; i++){
            for(int j=0; j<2; j++){
                stage1_BU_group_output[k][2*i+j] = stage1_BU_output[i+2*k][j];
            }
        }
    }  

    cout << "-------------------------------" << endl;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            long long index_rev = BU.BitReserve(j, 2);
            long long exp_prou16 = i*index_rev;
            stage1_BU_group_output[i][j] = MulMod(stage1_BU_group_output[i][j] ,ExpMod(prou_16, exp_prou16, modular), modular);
        } 
    }

    cout << "------------stage 2 input ---------" << endl;
    for(int i=0; i<4; i++){
        cout << "-----------------------" << endl;
        for(int j=0; j<4; j++){
           stage2_BU_input[i][j] = stage1_BU_group_output[j][i];
           cout << "stage2_BU_input[" << i << "][" << j << "]" << " = " << stage2_BU_input[i][j] << endl;
        }
    }
    

    
    return 0;
}