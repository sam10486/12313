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

void top_tb(){

    long long n = 8192;
    long long input_idx[n];
    long long data_in[n];
    long long data_out[n];
    long long radix = 16;
    long long modular = 167772161;
    long long phi = find_phi_fast(n, modular);
    //long long phi = 243624;
    cout << "phi = " << phi << endl;

    int BN = radix;
    int MA = (n/BN);

    vector<vector<long long> > memory;

    ofstream ofs_mem_data_in, ofs_mem_data_golden;
    ifstream ifs_ans_algo;
    ofs_mem_data_in.open("./tb_data/top/mem_data_in.txt");
    ofs_mem_data_golden.open("./tb_data/top/mem_data_golden.txt");
    ifs_ans_algo.open("./tb_data/check_in_place/memory_ans.txt");
    
    if(!ofs_mem_data_in.is_open() || !ifs_ans_algo.is_open() ){
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
        for(int i=0; i<BN; i++){
            for(int j=0; j<MA; j++){
                cout << "memory[" << i << "][" << j << "] = " << memory[i][j] << endl;
                ofs_mem_data_golden << std::hex << memory[i][j] << endl;
            }
        }


        //---------check ans----------
         vector<vector<long long> > algo_matrix;
        long long algo_ans;
        algo_matrix.resize(BN);
        for(int i=0; i<BN; i++){
            algo_matrix[i].resize(MA);
        }
        for(int i=0; i<BN; i++){
            for(int j=0; j<MA; j++){
                ifs_ans_algo >> algo_ans;
                algo_matrix[i][j] = algo_ans;
                //cout << "algo_ans = " << algo_ans << endl;
            }
        }
        int err = 0;
        for(int i=0; i<BN; i++){
            for(int j=0; j<MA; j++){
                //cout << "algo_matrix[" << i << "][" << j << "] = " << algo_matrix[i][j] << endl;
                if( memory[i][j] == algo_matrix[i][j] ){
                    cout << "num " << i*MA+j << " is PASS!" << endl;
                }else{
                    cout << "ERROR!, memory[" << i << "][" << j << "] is " << memory[i][j] 
                         << ", and algo_matrix[" << i << "][" << j << "] is " << algo_matrix[i][j] << endl;
                    err++;
                }
            }
        }
        if(err == 0){
            cout << "ALL PASS!!" << endl;
        }else {
            cout << "ERROR...." << endl;
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
    ofs_TF_based.open("./tb_data/top/TF_based_in.txt");
    ofs_TF_const.open("./tb_data/top/TF_const_in.txt");

    if(!ofs_TF_based.is_open() || !ofs_TF_const.is_open()){
        cout << "failed to open file.\n" << endl;
    }else {
        //k radix 2^k1 NTTs
        for(long long l=0; l<k; l++){
            cout << "<<<<<<<l>>>>>>>> = " << l << endl << "m_bar = ";
            for(long long m=1; m<pow(2, radix_k1); m++){
                long long m_bar = BR.BitReserve( (pow(2, radix_k1*l)-1) * pow(2, floor(log2(m))) + m, log2(n) ); 
                W[m] = ExpMod(phi, m_bar, modular);
                //ofs_TF_based << std::hex << W[m] << endl;
                //cout << "W[" << m << "] = " << W[m] << endl;
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
                //cout << "Wc = " << 1 << endl;
                ofs_TF_const << std::hex << 1 << endl;
            }else{
                //cout << "Wc = " << ExpMod(phi, pow(2, i), modular) << endl;
                ofs_TF_const << std::hex << ExpMod(phi, pow(2, i), modular) << endl;
            }
        }

        //---------------k2 stage------------------
        cout << "---*******************-------" << endl;
        int pow_radix_k2 = pow(2, radix_k2);
        long long W_k2[pow_radix_k2 - 1] = {0} ;
        long long tmp_k2[pow_radix_k2] = {0} ;
        long long tmp_array_k2[pow_radix_k2] = {0};
        // radix_2^k2 NTT
        for(int m=1; m<pow(2, radix_k2); m++){
            long long idx = (pow(2, radix_k1*k) - 1) * pow(2, floor(log2(m))) + m;
            long long m_bar = BR.BitReserve(idx, log2(n));
            W_k2[m] = ExpMod(phi, m_bar, modular);
        }
        for(int j=0; j<pow(2, radix_k1 * k); j++){
            long long j_bar = BR.BitReserve(j, radix_k1 * k);
            for(int m=0; m<pow(2, radix_k2); m++){
                int idx = j_bar * pow(2, radix_k2) + m;
                long long mem_Read_Data_k2_stage = mem_in_place(memory, 0, idx, n, pow_radix_k1, 1, 0);
                tmp_k2[m] = mem_Read_Data_k2_stage;
            }
            cout << "j = " << j << endl;
            power2_NTT(tmp_array_k2, tmp_k2, pow(2,radix_k2), W_k2, modular);
            for(int m=0; m<pow(2, radix_k2); m++){
                int idx = j_bar * pow(2, radix_k2) + m;
                mem_in_place(memory, tmp_array_k2[m], idx, n, pow_radix_k1, 0, 1);
            }
            for(int m=1; m<pow(2, radix_k2); m++){
                long long Wc_degree_k2 = radix_k2 - floor(log2(m));
                Wc_degree_k2 = pow(2, Wc_degree_k2);
                long long Wc_k2 = ExpMod(phi, Wc_degree_k2, modular);
                W_k2[m] = MulMod(W_k2[m], Wc_k2, modular);
            }
        }











        //---------generate TF---------
        for(long long l=0; l<k; l++){
            cout << "<<<<<<<l>>>>>>>> = " << l << endl << "m_bar = ";
            for(long long m=1; m<pow(2, radix_k1); m++){
                long long m_bar = BR.BitReserve( (pow(2, radix_k1*l)-1) * pow(2, floor(log2(m))) + m, log2(n) ); 
                W[m] = ExpMod(phi, m_bar, modular);
            }
            cout << "----j means group number, i means numbers of BU in one group----" << endl;
            for(int j=0; j<pow(2, radix_k1*l); j++){          

                switch(l){
                    case 0: {
                        if(j < 1){
                            for(long long m=1; m<pow(2, radix_k1); m++){
                                ofs_TF_based << std::hex << W[m] << endl;
                                //cout << "W[" << m << "] = " << W[m] << endl;
                            }
                        }
                    }
                    break;
                    case 1: {
                        if(j < 1){
                            for(long long m=1; m<pow(2, radix_k1); m++){
                                ofs_TF_based << std::hex << W[m] << endl;
                                //cout << "W[" << m << "] = " << W[m] << endl;
                            }
                        }
                    }
                    break;
                    case 2: {
                        if(j < 2) {
                            for(long long m=1; m<pow(2, radix_k1); m++){
                                ofs_TF_based << std::hex << W[m] << endl;
                                //cout << "W[" << m << "] = " << W[m] << endl;
                            }
                        }
                    }
                    break;
                    default: break;
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
        //------generate TF part k2-----------
        cout << "--------k2 TF---------" << endl;
        for(int m=1; m<pow(2, radix_k2); m++){
            long long idx = (pow(2, radix_k1*k) - 1) * pow(2, floor(log2(m))) + m;
            long long m_bar = BR.BitReserve(idx, log2(n));
            W_k2[m] = ExpMod(phi, m_bar, modular);
        }
        for(int j=0; j<pow(2, radix_k1 * k); j++){
            if(j==0){
                cout << "j = " << j << endl;
                for(int m=1; m<pow(2, radix_k2); m++){
                    ofs_TF_based << std::hex << W_k2[m] << endl;
                    cout << "W_k2[" << m << "] = " << W_k2[m] << endl;
                }
                for(int i=pow(2, radix_k2); i<pow(2, radix_k1); i++){
                    ofs_TF_based << std::hex << 0 << endl;
                }
            }
            for(int m=1; m<pow(2, radix_k2); m++){
                long long Wc_degree_k2 = radix_k2 - floor(log2(m));
                Wc_degree_k2 = pow(2, Wc_degree_k2);
                long long Wc_k2 = ExpMod(phi, Wc_degree_k2, modular);
                W_k2[m] = MulMod(W_k2[m], Wc_k2, modular);
            }
        }







    }
}