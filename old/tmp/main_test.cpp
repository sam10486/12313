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
    long long m = 512;
    long long modular = 12289;

    long long FFT_data_in[m];

    long long stage0_data_in[256][radix_2];
    long long stage0_data_out_tmp[256][radix_2];
    
    long long stage1_data_group[2][256];
    long long stage1_data_in_radix16[2][16][radix_16];
    long long stage1_data_out_radix16[2][16][radix_16];
    long long stage1_data_out_tmp[2][16][radix_16];

    long long stage2_data_in_radix16[2][16][radix_16];
    long long stage2_data_out_radix16[2][16][radix_16];
    long long stage2_data_out_tmp[2][256];

    long long FFT_out_final[m];

    long long prou_512 = find_prou(m, modular);
    long long prou_256 = ExpMod(prou_512, 2, modular);
    long long prou_16 = ExpMod(prou_512, 32, modular);
    long long prou_2 = ExpMod(prou_512, 256, modular);

    cout << "prou_256 = " << prou_256 << endl;

    for(int i=0; i<m; i++){
        FFT_data_in[i] = i;
        cout << "FFT_data_in" << i << " = " << FFT_data_in[i] << endl;
    }

    cout << "----------stage0_data_in-----------" << endl;
    for(int i=0; i<256; i++){
        for(int j=0; j<2; j++){
            stage0_data_in[i][j] = FFT_data_in[i+256*j];
            cout << "stage0_data_in[" << i << "][" << j << "]" << " = " << stage0_data_in[i][j] << endl;
        }
    }

    cout << "----------stage0_fft------------" << endl;
    for(int i=0; i<256; i++){
        for(int j=0; j<2; j++){
            FFT(stage0_data_out_tmp[i], stage0_data_in[i], radix_2, prou_2, modular);
            cout << "stage0_data_out_tmp[" << i << "][" << j << "]" << " = " << stage0_data_out_tmp[i][j] << endl;
        }
    }

    cout << "-------------------------------" << endl;
    for(int i=0; i<256; i++){
       for(int j=0; j<2; j++){
            long long exp_prou512 = i*j;
            stage0_data_out_tmp[i][j] = MulMod(stage0_data_out_tmp[i][j] ,ExpMod(prou_512, exp_prou512, modular), modular);
            cout << "stage0_data_out_tmp_twiddle[" << i << "][" << j << "]" << " = " << stage0_data_out_tmp[i][j] << endl;
        }
    }

    cout << "----------stage1_data_in-----------" << endl;
    for(int i=0; i<2; i++){
        cout << "--------------------------------" << endl;
        for(int j=0; j<256; j++){
           stage1_data_group[i][j] = stage0_data_out_tmp[j][i];
           cout << "stage1_data_group[" << i << "][" << j << "]" << " = " << stage1_data_group[i][j] << endl;
        }
    }

    cout << "----------stage1_radix16 input-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<16; i++){
            for(int j=0; j<16; j++){
                stage1_data_in_radix16[k][i][j] = stage1_data_group[k][i+j*16];
                //stage1_data_in_radix16[i+16][j] = stage1_data_group[1][i+j*16];
                cout << "stage1_data_in_radix16[" << k << "][" << i << "][" << j << "]" << " = " << stage1_data_in_radix16[k][i][j] << endl;
                //cout << "stage1_data_in_radix16[" << i+16 << "][" << j << "]" << " = " << stage1_data_in_radix16[i+16][j] << endl;
            }
        }
    }
    
    cout << "----------stage1_radix16 fft-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<16; i++){
            for(int j=0; j<16; j++){
                FFT(stage1_data_out_radix16[k][i], stage1_data_in_radix16[k][i], radix_16, prou_16, modular);
                cout << "stage1_data_out_radix16[" << k << "][" << i << "][" << j << "]" << " = " << stage1_data_out_radix16[k][i][j] << endl;
            }
        }
    }

    cout << "----------stage1_radix16 fft twiidle-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<16; i++){
            for(int j=0; j<16; j++){
                long long exp_prou256 = i*j;
                stage1_data_out_tmp[k][i][j] = MulMod(stage1_data_out_radix16[k][i][j] ,ExpMod(prou_256, exp_prou256, modular), modular);
                cout << "stage1_data_out_tmp[" << k << "][" << i << "][" << j << "]" << " = " << stage1_data_out_tmp[k][i][j] << endl;
            }
        }
    }

    cout << "----------stage2_data_in-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<16; i++){
            for(int j=0; j<16; j++){
                stage2_data_in_radix16[k][i][j] = stage1_data_out_tmp[k][j][i];
                cout << "stage2_data_in_radix16[" << k << "][" << i << "][" << j << "]" << " = " << stage2_data_in_radix16[k][i][j] << endl;
            }
        }
    }

    cout << "----------stage2_radix16 fft-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<16; i++){
            for(int j=0; j<16; j++){
                FFT_no_bit_reverse(stage2_data_out_radix16[k][i], stage2_data_in_radix16[k][i], radix_16, prou_16, modular);
                cout << "stage2_data_out_radix16[" << k << "][" << i << "][" << j << "]" << " = " << stage2_data_out_radix16[k][i][j] << endl;
            }
        }
    }

    cout << "----------stage2_output-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<16; i++){
            for(int j=0; j<16; j++){
                stage2_data_out_tmp[k][i*16+j] = stage2_data_out_radix16[k][i][j];
                cout << "stage2_data_out_tmp[" << k << "][" << i*16+j << "]" << " = " << stage2_data_out_tmp[k][i*16+j] << endl;
            }
        }
    }


    cout << "----------fft_complete-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<16; i++){
            for(int j=0; j<16; j++){
                FFT_out_final[k*256+i*16+j] = stage2_data_out_tmp[k][i*16+j];
                cout << "FFT_out_final[" << k*256+i*16+j << "]" << " = " << FFT_out_final[k*256+i*16+j] << endl;
            }
        }
    }

    cout << "----------fft_output bit rev-----------" << endl;
    long long FFT_out_final_bit_reverse[m];
    for(int i=0; i<m;i++){
        BitOperate BU;
        long long index_rev = BU.BitReserve(i, 9);
        FFT_out_final_bit_reverse[index_rev] = FFT_out_final[i];
        cout << FFT_out_final_bit_reverse[index_rev] << endl;
    }

    long long DFT_ans[m];
    DFT(DFT_ans, FFT_data_in, m, prou_512, modular);
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
    }


    return 0;
}