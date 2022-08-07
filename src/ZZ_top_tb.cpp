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


void ZZ_top_tb(){

    long long n = 8192;
    long long input_idx[n];
    ZZ data_in[n];
    ZZ data_out[n];
    long long radix = 16;
    ZZ modular = (ZZ)2013265921;
    ZZ phi = find_phi(n, modular);
    //long long phi = 243624;
    cout << "phi = " << phi << endl;

    int BN = radix;
    int MA = (n/BN);

    vector<vector<ZZ> > memory;

    ofstream ofs_mem_data_in, ofs_mem_data_golden;
    ifstream ifs_ans_algo;
    ofs_mem_data_in.open("./tb_data/ZZ_top/mem_data_in.txt");
    ofs_mem_data_golden.open("./tb_data/ZZ_top/mem_data_golden.txt");
    ifs_ans_algo.open("./tb_data/ZZ_NWC_mem/memory_ans.txt");
    
    if(!ofs_mem_data_in.is_open() || !ifs_ans_algo.is_open() ){
        cout << "failed to open file.\n" << endl;
    }else {
        memory.resize(BN);
        for(int i=0; i<BN; i++){
            memory[i].resize(MA);
        }

        for(int i=0; i<n; i++){
            input_idx[i] = i;
            data_in[i] = (ZZ)i;
            mem_init_in_place(memory, data_in[i], input_idx[i], n, radix, modular);
        }
        
        cout << "CCCCCCC" << endl;
        for(int i=0; i<MA; i++){
            for(int j=0; j<BN; j++){
                cout << "memory[" << j << "][" << i << "] = " << memory[j][i] << endl;
                ofs_mem_data_in << std::hex << memory[j][i] << endl;
            }
        }

        long long log_radix = log2(radix);
        ZZ_top(data_out, data_in, n, log_radix, 1, phi, modular, memory);

        cout << "GGGG" << endl;
        for(int i=0; i<BN; i++){
            for(int j=0; j<MA; j++){
                cout << "memory[" << i << "][" << j << "] = " << memory[i][j] << endl;
                ofs_mem_data_golden << std::hex << memory[i][j] << endl;
            }
        }


        //---------check ans----------
        vector<vector<ZZ> > algo_matrix;
        ZZ algo_ans;
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