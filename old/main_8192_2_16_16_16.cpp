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
//k: means group, i:means BU num in a group, j: means radix in a BU

int main(){
    long long radix_2 = 2;
    long long radix_16 = 16;
    long long radix_256 = 256;
    long long m = 8192;
    long long modular = 65537;

    long long FFT_data_in[m];
    long long stage0_data_in[4096][radix_2];
    long long stage0_data_out_tmp[4096][radix_2];

    long long stage1_data_group[2][4096];
    long long stage1_data_in_radix16[2][256][radix_16];
    long long stage1_data_out_radix16[2][256][radix_16];
    long long stage1_data_out_tmp[2][256][radix_16];

    long long stage2_data_in_radix256[2][16][radix_256];
    long long stage2_data_in_radix16_in_BU256[2][16][16][radix_16];
    long long stage2_data_in_radix16_out_BU256[2][16][16][radix_16];
    long long stage2_data_out_tmp[2][16][16][radix_16];

    long long stage3_data_in_radix16_in_BU256[2][16][16][radix_16];
    long long stage3_data_in_radix16_out_BU256[2][16][16][radix_16];
    long long stage3_data_out_tmp[2][16][256];
    long long stage3_data_out_tmp_2[2][4096];
    long long stage3_data_out_tmp_3[m];

    long long prou_8192 = find_prou(m, modular);
    long long prou_4096 = ExpMod(prou_8192, 2, modular);
    long long prou_256 = ExpMod(prou_8192, 32, modular);
    long long prou_16 = ExpMod(prou_8192, 512, modular);
    long long prou_2 = ExpMod(prou_8192, 4096, modular);

    for(int i=0; i<m; i++){
        FFT_data_in[i] = i;
        cout << "FFT_data_in" << i << " = " << FFT_data_in[i] << endl;
    }


    cout << "----------stage0_data_in-----------" << endl;
    for(int i=0; i<4096; i++){
        for(int j=0; j<2; j++){
            stage0_data_in[i][j] = FFT_data_in[i+4096*j];
            cout << "stage0_data_in[" << i << "][" << j << "]" << " = " << stage0_data_in[i][j] << endl;
        }
    }

    cout << "----------stage0_fft------------" << endl;
    for(int i=0; i<4096; i++){
        for(int j=0; j<2; j++){
            FFT_no_bit_reverse(stage0_data_out_tmp[i], stage0_data_in[i], radix_2, prou_2, modular);
            cout << "stage0_data_out_tmp[" << i << "][" << j << "]" << " = " << stage0_data_out_tmp[i][j] << endl;
        }
    }

    cout << "-----------stage0_8192 fft twiidle--------------" << endl;
    for(int i=0; i<4096; i++){
       for(int j=0; j<2; j++){
            BitOperate BU;
            long long index_rev = BU.BitReserve(j, 1);
            long long exp_prou8192 = i*index_rev;
            stage0_data_out_tmp[i][j] = MulMod(stage0_data_out_tmp[i][j] ,ExpMod(prou_8192, exp_prou8192, modular), modular);
            cout << "stage0_data_out_tmp_twiddle[" << i << "][" << j << "]" << " = " << stage0_data_out_tmp[i][j] << endl;
        }
    }

    cout << "----------stage1_data_in-----------" << endl;
    for(int k=0; k<2; k++){
        cout << "--------------------------------" << endl;
        for(int j=0; j<4096; j++){
           stage1_data_group[k][j] = stage0_data_out_tmp[j][k];
           cout << "stage1_data_group[" << k << "][" << j << "]" << " = " << stage1_data_group[k][j] << endl;
        }
    }

    cout << "----------stage1_radix16 input-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<256; i++){
            for(int j=0; j<16; j++){
                stage1_data_in_radix16[k][i][j] = stage1_data_group[k][i+j*256];
                cout << "stage1_data_in_radix16[" << k << "][" << i << "][" << j << "]" << " = " << stage1_data_in_radix16[k][i][j] << endl;      
            }
        }
    }

    cout << "----------stage1_radix16 fft-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<256; i++){
            for(int j=0; j<16; j++){
                FFT_no_bit_reverse(stage1_data_out_radix16[k][i], stage1_data_in_radix16[k][i], radix_16, prou_16, modular);
                cout << "stage1_data_out_radix16[" << k << "][" << i << "][" << j << "]" << " = " << stage1_data_out_radix16[k][i][j] << endl;
            }
        }
    }

    cout << "----------stage1_4096 fft twiidle-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<256; i++){
            for(int j=0; j<16; j++){
                BitOperate BU;
                long long index_rev = BU.BitReserve(j, 4);
                long long exp_prou4096 = i*index_rev;
                stage1_data_out_tmp[k][i][j] = MulMod(stage1_data_out_radix16[k][i][j] ,ExpMod(prou_4096, exp_prou4096, modular), modular);
                cout << "stage1_data_out_tmp[" << k << "][" << i << "][" << j << "]" << " = " << stage1_data_out_tmp[k][i][j] << endl;
            }
        }
    }
    cout << "----------stage2_data_in-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<16; i++){
            for(int j=0; j<256; j++){
                stage2_data_in_radix256[k][i][j] = stage1_data_out_tmp[k][j][i];
                cout << "stage2_data_in_radix256[" << k << "][" << i << "][" << j << "]" << " = " << stage2_data_in_radix256[k][i][j] << endl;
            }
        }
    }

    cout << "----------stage2_data_in for radix 16 in 256 BU-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<16; i++){
            for(int u=0; u<16; u++){
                for(int v=0; v<16; v++){
                    // k: 0th group, i: 0th 256 BU, u: 0th 16 BU, v: radix16 output
                    stage2_data_in_radix16_in_BU256[k][i][u][v] = stage2_data_in_radix256[k][i][u+v*16];
                    cout << "stage1_data_in_radix256[" << k << "][" << i << "][" << u << "][" << i << "]"  << " = " << stage2_data_in_radix16_in_BU256[k][i][u][v] << endl;
                }
            }
        }
    }


    cout << "----------stage2_data_in for radix 16 in 256 BU fft-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<16; i++){
            for(int u=0; u<16; u++){
                for(int v=0; v<16; v++){
                    // k: 0th group, i: 0th 256 BU, u: 0th 16 BU, v: radix16 output
                    FFT_no_bit_reverse(stage2_data_in_radix16_out_BU256[k][i][u], stage2_data_in_radix16_in_BU256[k][i][u], radix_16, prou_16, modular);
                    cout << "stage2_data_in_radix16_out_BU256[" << k << "][" << i << "][" << u << "][" << i << "]"  << " = " << stage2_data_in_radix16_out_BU256[k][i][u][v] << endl;
                }
            }
        }
    }

    cout << "----------stage2_256 fft twiidle-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<16; i++){
            for(int u=0; u<16; u++){
                for(int v=0; v<16; v++){
                    // k: 0th group, i: 0th 256 BU, u: 0th 16 BU, v: radix16 output
                    BitOperate BU;
                    long long index_rev = BU.BitReserve(v, 4);
                    long long exp_prou256 = u*index_rev;
                    stage2_data_out_tmp[k][i][u][v] = MulMod(stage2_data_in_radix16_out_BU256[k][i][u][v] ,ExpMod(prou_256, exp_prou256, modular), modular);
                    cout << "stage2_data_out_tmp[" << k << "][" << i << "][" << u << "][" << i << "]"  << " = " << stage2_data_out_tmp[k][i][u][v] << endl;
                }
            }
        }
    }

    cout << "---------------stage3_data_in--------------------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<16; i++){
            for(int u=0; u<16; u++){
                for(int v=0; v<16; v++){
                    // k: 0th group, i: 0th 256 BU, u: 0th 16 BU, v: radix16 output
                    stage3_data_in_radix16_in_BU256[k][i][u][v] = stage2_data_out_tmp[k][i][v][u];
                }
            }
        }
    }

    cout << "----------stage3_data_in for radix 16 in 256 BU fft-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<16; i++){
            for(int u=0; u<16; u++){
                for(int v=0; v<16; v++){
                    // k: 0th group, i: 0th 256 BU, u: 0th 16 BU, v: radix16 output
                    FFT_no_bit_reverse(stage3_data_in_radix16_out_BU256[k][i][u], stage3_data_in_radix16_in_BU256[k][i][u], radix_16, prou_16, modular);
                    cout << "stage3_data_in_radix16_out_BU256[" << k << "][" << i << "][" << u << "][" << i << "]"  << " = " << stage3_data_in_radix16_out_BU256[k][i][u][v] << endl;
                }
            }
        }
    }

    cout << "----------stage3_output-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<16; i++){
            for(int u=0; u<16; u++){
                for(int v=0; v<16; v++){
                    // k: 0th group, i: 0th 256 BU, u: 0th 16 BU, v: radix16 output
                    stage3_data_out_tmp[k][i][u*16+v] = stage3_data_in_radix16_out_BU256[k][i][u][v];
                    cout << "stage3_data_out_tmp[" << k << "][" << i << "][" << u*16+v << "]"  << " = " << stage3_data_out_tmp[k][i][u*16+v] << endl;
                }
            }
        }
    }

    cout << "----------stage3_output_2-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<16; i++){
            for(int u=0; u<256; u++){
                // k: 0th group, i: 0th 256 BU, u: 0th 16 BU, v: radix16 output
                stage3_data_out_tmp_2[k][i*256+u] = stage3_data_out_tmp[k][i][u];
                cout << "stage3_data_out_tmp_2[" << k << "][" << i*256+u << "]"  << " = " << stage3_data_out_tmp_2[k][i*256+u] << endl;
            }
        }
    }
    cout << "----------stage3_output_3_fft_complete-----------" << endl;
    for(int k=0; k<2; k++){
        for(int i=0; i<4096; i++){
            // k: 0th group, i: 0th 256 BU, u: 0th 16 BU, v: radix16 output
            stage3_data_out_tmp_3[k*4096+i] = stage3_data_out_tmp_2[k][i];
            cout << "stage3_data_out_tmp_3[" << k << "]" << " = " << stage3_data_out_tmp_3[k*4096+i] << endl;
        }
    }
    cout << "----------fft_output bit rev-----------" << endl;
    long long FFT_out_final_bit_reverse[m];
    for(int i=0; i<m;i++){
        BitOperate BU;
        long long index_rev = BU.BitReserve(i, 13);
        FFT_out_final_bit_reverse[index_rev] = stage3_data_out_tmp_3[i];
        cout << FFT_out_final_bit_reverse[index_rev] << endl;
    }

    long long DFT_ans[m];
    int err=0;
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
            err++;
        }
    }
    if(err == 0){
        cout << "All Passed!" << endl;
    }else{
       cout << "Failed! err = " << err << endl;
    }
    return 0;
}