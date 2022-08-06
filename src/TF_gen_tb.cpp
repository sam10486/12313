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
#include <fstream>
#include "assert.h"

using namespace std;
using namespace NTL;

void TF_gen_tb(){
    long long radix_k1 = 4;
    long long radix_k2 = 1;
    long long n = 8192;
    long long modular = 65537;
    long long phi = find_phi(n, modular);
    cout << "phi = " << phi << endl;

    long long k = ( log2(n) - radix_k2) / radix_k1;
    long long parameter_check = radix_k1 * k + radix_k2;
    assert(parameter_check == log2(n));

    int pow_radix_k1 = pow(2, radix_k1);
    long long W[pow_radix_k1 - 1] = {0};
    long long tmp[pow_radix_k1] = {0} ;
    long long tmp_array[pow_radix_k1] = {0};
    BitOperate BR;
    ofstream ofs_TF_gen_k1, ofs_TF_W_initial, ofs_TF_Wc_const;

    ofs_TF_gen_k1.open("../tb_data/TF/TF_gen_golden.txt");
    ofs_TF_W_initial.open("../tb_data/TF/W_init_golden.txt");
    ofs_TF_Wc_const.open("../tb_data/TF/Wc_const_golden.txt");

    if(!ofs_TF_gen_k1.is_open() || !ofs_TF_W_initial.is_open() || !ofs_TF_Wc_const.is_open() ){
        cout << "failed to open file.\n" << endl;
    }else {
        //k radix 2^k1 NTTs
        for(long long l=0; l<k; l++){
            cout << "<<<<<<<l>>>>>>>> = " << l << endl << "m_bar = ";
            for(long long m=1; m<pow(2, radix_k1); m++){
                long long m_bar = BR.BitReserve( (pow(2, radix_k1*l)-1) * pow(2, floor(log2(m))) + m, log2(n) ); 
                W[m] = ExpMod(phi, m_bar, modular);
                //cout << "W[" << m << "] = " << W[m] << endl;
                ofs_TF_W_initial << std::hex << W[m] << endl;
            }
            cout << "----j means group number, i means numbers of BU in one group----" << endl;
            for(int j=0; j<pow(2, radix_k1*l); j++){
                for(int m=1; m<pow(2, radix_k1); m++){
                    long long Wc_degree = log2(n) - radix_k1 * l - floor(log2(m));
                    if(Wc_degree != log2(n))
                        Wc_degree = pow(2, Wc_degree);
                    else 
                        Wc_degree = 0;

                    cout << "W[" << m << "] = " << W[m] << endl; 
                    ofs_TF_gen_k1 << std::hex << W[m] << endl;

                    long long Wc = ExpMod(phi, Wc_degree, modular);
                    W[m] = MulMod(W[m], Wc, modular);
                    //cout << "Wc_degree = " << Wc_degree << endl;
                    //cout << "W[" << m <<  "]_x_const = " << W[m] << endl;
                    //cout << "Wc = " << Wc << endl;
                }
                cout << "--------------" << endl;
            }
        }

        for(int i=0; i<=log2(n); i++){
            long long Wc;
            if(i == log2(n)){
                cout << "Wc = " << 1 << endl;
                ofs_TF_Wc_const << std::hex << 1 << endl;
            }else{
                cout << "Wc = " << ExpMod(phi, pow(2, i), modular) << endl;
                ofs_TF_Wc_const << std::hex << ExpMod(phi, pow(2, i), modular) << endl;
            }

        }
    } 
}

