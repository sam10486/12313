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
    long long m=8;
	long long radix=4;
	long long modular = 257;
	long long FFT_data_in[m];
	long long FFT_tmp[m];
    long long radix_2=2;
	
	long long FFT_data_in_up[radix];
	long long FFT_data_in_down[radix];
	long long prou = find_prou(radix, modular);
	long long prou_8 = find_prou(m, modular);
	long long prou_2 = find_prou(radix_2, modular);

	long long FFT_out_up[radix];
	long long FFT_out_down[radix];

    cout << "prou = " << prou << endl;
    cout << "prou_8 = " << prou_8 << endl;

	
    cout << "-------FFT_data_in--------" << endl;
	for(int i=0; i<m; i++){
		FFT_data_in[i] = i;
		cout << "FFT_data_in " << i << " = " << FFT_data_in[i] << endl;
	}
    cout << "-------FFT_data_in_up--------" << endl;
	for(int i=0; i<radix; i++){
		FFT_data_in_up[i] = FFT_data_in[i*2];
		cout << "FFT_data_in_up " << i << " = " << FFT_data_in_up[i] << endl;
	}
	cout << "-------FFT_data_in_down--------" << endl;
	for(int i=0; i<radix; i++){
		FFT_data_in_down[i] = FFT_data_in[2*i+1];
		cout << "FFT_data_in_down " << i << " = " << FFT_data_in_down[i] << endl;
	}
	


	long long FFT_out_up_no_bit_reverse[radix];
	long long FFT_out_down_no_bit_reverse[radix];

    FFT_no_bit_reverse(FFT_out_up_no_bit_reverse, FFT_data_in_up, radix, prou, modular);
    FFT_no_bit_reverse(FFT_out_down_no_bit_reverse, FFT_data_in_down, radix, prou, modular);


	
    cout << "-------FFT_out_up_no_bit_reverse--------" << endl;
	for(int i=0; i<radix; i++){
		cout << FFT_out_up_no_bit_reverse[i] << endl;
	}
	cout << "-------FFT_out_down_no_bit_reverse--------" << endl;
	for(int i=0; i<radix; i++){
		cout << FFT_out_down_no_bit_reverse[i] << endl;
	}


    long long FFT_out_BU0_stage0_out[radix];
    long long FFT_out_BU1_stage0_out[radix];
    BitOperate BU;
    for(int i=0; i<radix; i++){
        long long index_rev = BU.BitReserve(i, 2);
        long long exp_prou8 = 0*index_rev;
        FFT_out_BU0_stage0_out[i] = MulMod(FFT_out_up_no_bit_reverse[i] ,ExpMod(prou_8, exp_prou8, modular), modular);
    }
    for(int i=0; i<radix; i++){
        long long index_rev = BU.BitReserve(i, 2);
        long long exp_prou8 = 1*index_rev;
        FFT_out_BU1_stage0_out[i] = MulMod(FFT_out_down_no_bit_reverse[i] ,ExpMod(prou_8, exp_prou8, modular), modular);
    }

	cout << "-------------------------" << endl;


    for(int i=0;i<radix;i++){
        FFT_tmp[2*i] = FFT_out_BU0_stage0_out[i];
        FFT_tmp[2*i+1] = FFT_out_BU1_stage0_out[i];
    }

    cout << "-----------FFT_tmp------------" << endl;
    for(int i=0; i<m; i++)
        cout << FFT_tmp[i] << endl;

    long long FFT_stage1_input_BU0[radix_2];
    long long FFT_stage1_input_BU1[radix_2];
    long long FFT_stage1_input_BU2[radix_2];
    long long FFT_stage1_input_BU3[radix_2];

    cout << "-----------FFT_stage1_input_BU0------------" << endl;
    for(int i=0; i<radix_2;i++){
        FFT_stage1_input_BU0[i] = FFT_tmp[i];
        cout << "FFT_stage1_input_BU0 " << i << " = " << FFT_stage1_input_BU0[i] << endl;
    }
    cout << "-----------FFT_stage1_input_BU0------------" << endl;
    for(int i=0; i<radix_2;i++){
        FFT_stage1_input_BU1[i] = FFT_tmp[i+2];
        cout << "FFT_stage1_input_BU1 " << i << " = " << FFT_stage1_input_BU1[i] << endl;
    }
    cout << "-----------FFT_stage1_input_BU0------------" << endl;
    for(int i=0; i<radix_2;i++){
        FFT_stage1_input_BU2[i] = FFT_tmp[i+4];
        cout << "FFT_stage1_input_BU2 " << i << " = " << FFT_stage1_input_BU2[i] << endl;
    }
    cout << "-----------FFT_stage1_input_BU0------------" << endl;
    for(int i=0; i<radix_2;i++){
        FFT_stage1_input_BU3[i] = FFT_tmp[i+6];
        cout << "FFT_stage1_input_BU3 " << i << " = " << FFT_stage1_input_BU3[i] << endl;
    }

    long long FFT_stage1_out_BU0[radix_2];
    long long FFT_stage1_out_BU1[radix_2];
    long long FFT_stage1_out_BU2[radix_2];
    long long FFT_stage1_out_BU3[radix_2];
    FFT(FFT_stage1_out_BU0, FFT_stage1_input_BU0, radix_2, prou_2, modular);
    FFT(FFT_stage1_out_BU1, FFT_stage1_input_BU1, radix_2, prou_2, modular);
    FFT(FFT_stage1_out_BU2, FFT_stage1_input_BU2, radix_2, prou_2, modular);
    FFT(FFT_stage1_out_BU3, FFT_stage1_input_BU3, radix_2, prou_2, modular);


    long long FFT_out_final[m];
    for(int i=0; i<radix_2;i++){
        FFT_out_final[i] = FFT_stage1_out_BU0[i];
        FFT_out_final[i+2] = FFT_stage1_out_BU1[i];
        FFT_out_final[i+4] = FFT_stage1_out_BU2[i];
        FFT_out_final[i+6] = FFT_stage1_out_BU3[i];
    }
    long long FFT_out_final_bit_reverse[m];
    for(int i=0; i<m;i++){
        long long index_rev = BU.BitReserve(i, 3);
        FFT_out_final_bit_reverse[index_rev] = FFT_out_final[i];
    }

    cout << "-----------FFT_out_final------------" << endl;
    for(int i=0; i<m; i++)
        cout << FFT_out_final[i] << endl;

    cout << "-----------FFT_out_final_bit_reverse------------" << endl;
    for(int i=0; i<m; i++)
        cout << FFT_out_final_bit_reverse[i] << endl;

    long long DFT_ans[m];
    DFT(DFT_ans, FFT_data_in, m, prou_8, modular);
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