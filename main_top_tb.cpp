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

void top(   long long *NWC_data, long long *NWC_data_in, 
            long long n, long long radix_k1, long long radix_k2, long long phi, 
            long long modular,
            vector<vector<long long> > &memory);

int main(){

    long long n = 8192;
    long long input_idx[n];
    long long data_in[n];
    long long data_out[n];
    long long radix = 16;
    long long modular = 65537;
    long long phi = find_phi(n, modular);
    cout << "phi = " << phi << endl;

    int BN = radix;
    int MA = (n/BN);

    vector<vector<long long> > memory;

    ofstream ofs_mem_data_in, ofs_mem_data_golden;
    ofs_mem_data_in.open("../tb_data/top/mem_data_in.txt");
    ofs_mem_data_golden.open("../tb_data/top/mem_data_golden.txt");
    
    if(!ofs_mem_data_in.is_open()){
        cout << "failed to open file.\n" << endl;
    }else {
        memory.resize(BN);
        for(int i=0; i<BN; i++){
            memory[i].resize(MA);
        }

        for(int i=0; i<n; i++){
            input_idx[i] = i;
            data_in[i] = i;
            //cout << "data_in[" << i << "] = " << data_in[i] << endl;
            mem_init_in_place(memory, data_in[i], input_idx[i], n, radix, modular);
            //cout << "------------" << endl;
        }
        
        cout << "CCCCCCC" << endl;
        for(int i=0; i<MA; i++){
            for(int j=0; j<BN; j++){
                cout << "memory[" << j << "][" << i << "] = " << memory[j][i] << endl;
                ofs_mem_data_in << std::hex << memory[j][i] << endl;
            }
        }

        long long log_radix = log2(radix);
        top(data_out, data_in, n, log_radix, 1, phi, modular, memory);

        cout << "GGGG" << endl;
        for(int i=0; i<MA; i++){
            for(int j=0; j<BN; j++){
                cout << "memory[" << j << "][" << i << "] = " << memory[j][i] << endl;
                ofs_mem_data_golden << std::hex << memory[j][i] << endl;
            }
        }
    }
}

void top(   long long *NWC_data, long long *NWC_data_in, 
            long long n, long long radix_k1, long long radix_k2, long long phi, 
            long long modular,
            vector<vector<long long> > &memory){
    


    long long k = ( log2(n) - radix_k2) / radix_k1;
    //cout << "k = " << k << endl;
    long long parameter_check = radix_k1 * k + radix_k2;
    assert(parameter_check == log2(n));

    int pow_radix_k1 = pow(2, radix_k1);
    long long W[pow_radix_k1 - 1] = {0};
    long long tmp[pow_radix_k1] = {0} ;
    long long tmp_array[pow_radix_k1] = {0};
    BitOperate BR;

    ofstream ofs_TF_based, ofs_TF_const;
    ofs_TF_based.open("../tb_data/top/TF_based_in.txt");
    ofs_TF_const.open("../tb_data/top/TF_const_in.txt");

    if(!ofs_TF_based.is_open() || !ofs_TF_const.is_open()){
        cout << "failed to open file.\n" << endl;
    }else {
        //k radix 2^k1 NTTs
        for(long long l=0; l<k; l++){
            cout << "<<<<<<<l>>>>>>>> = " << l << endl << "m_bar = ";
            for(long long m=1; m<pow(2, radix_k1); m++){
                long long m_bar = BR.BitReserve( (pow(2, radix_k1*l)-1) * pow(2, floor(log2(m))) + m, log2(n) ); 
                W[m] = ExpMod(phi, m_bar, modular);
                ofs_TF_based << std::hex << W[m] << endl;
                //cout << m_bar << endl;
                cout << "W[" << m << "] = " << W[m] << endl;
            }
            cout << "----j means group number, i means numbers of BU in one group----" << endl;
            for(int j=0; j<pow(2, radix_k1*l); j++){
                long long j_bar = BR.BitReserve(j, radix_k1*l);
                for(int i=0; i<pow(2, log2(n)- radix_k1*(l+1)); i++){
                    for(int m=0; m<pow(2, radix_k1); m++){
                        long long index = j_bar * pow(2, log2(n) - radix_k1*l) + m * pow(2, log2(n) - radix_k1*(l+1)) + i;
                        long long mem_Read_Data = mem_in_place(memory, 0, index, n, pow_radix_k1, 1, 0);
                        tmp[m] = mem_Read_Data;
                        //cout << index << ", ";
                    }

                    power2_NTT(tmp_array, tmp, pow(2, radix_k1), W, modular);

                    /*for(int i=0; i<pow(2, radix_k1); i++){
                        cout << "tmp_array[" << i << "] = " << tmp_array[i] << endl;
                    }*/
                
                    for(int m=0; m<pow(2, radix_k1); m++){
                        long long index = j_bar * pow(2, log2(n) - radix_k1*l) + m * pow(2, log2(n) - radix_k1*(l+1)) + i;
                        mem_in_place(memory, tmp_array[m], index, n, pow_radix_k1, 0, 1);
                    }
                }
                for(int m=1; m<pow(2, radix_k1); m++){
                    long long Wc_degree = log2(n) - radix_k1 * l - floor(log2(m));
                    if(Wc_degree != log2(n))
                        Wc_degree = pow(2, Wc_degree);
                    else 
                        Wc_degree = 0;
                    long long Wc = ExpMod(phi, Wc_degree, modular);
                    W[m] = MulMod(W[m], Wc, modular);
                }
            }
        }
        for(int i=0; i<=log2(n); i++){
            long long Wc;
            if(i == log2(n)){
                cout << "Wc = " << 1 << endl;
                ofs_TF_const << std::hex << 1 << endl;
            }else{
                cout << "Wc = " << ExpMod(phi, pow(2, i), modular) << endl;
                ofs_TF_const << std::hex << ExpMod(phi, pow(2, i), modular) << endl;
            }
        }
    }
}