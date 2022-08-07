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

using namespace std;
using namespace NTL;

void ZZ_NWC_in_place(){
    long long n = 8192;
    long long input_idx[n];
    ZZ data_in[n];
    ZZ data_out[n];
    long long radix = 16;
    ZZ modular = (ZZ)2013265921;
    ZZ phi = find_phi(n, modular);
    //ZZ phi = (ZZ)260512;
    cout << "phi = " << phi << endl;
    BitOperate BR;

    int BN = radix;
    int MA = (n/BN);

    vector<vector<ZZ> > memory;

    ofstream ofs;
    ofs.open("./tb_data/ZZ_NWC_mem/memory_ans.txt");

    memory.resize(BN);
    for(int i=0; i<BN; i++){
        memory[i].resize(MA);
    }

    
    for(int i=0; i<n; i++){
        input_idx[i] = i;
        data_in[i] = (ZZ)i;
        //cout << "data_in[" << i << "] = " << data_in[i] << endl;
        mem_init_in_place(memory, data_in[i], input_idx[i], n, radix, modular);
        //cout << "------------" << endl;
    }
    cout << "----123123------" << endl;
    
    long long log_radix = log2(radix);
    mixed_radix_NWC_in_place(data_out, data_in, n, log_radix, 1, phi, modular, memory);

    cout << "---GGGGGGGGGGG------" << endl;
    for(int i=0; i<BN; i++){
        cout << "------------" << endl;
        for(int j=0; j<MA; j++){
            cout << "memory[" << i << "][" << j << "] = " << memory[i][j] << endl;
        }
    }


    if(!ofs.is_open()){
        cout << "failed to open file.\n" << endl;
    }else {
        for(int i=0; i<BN; i++){
            for(int j=0; j<MA; j++){
                 ofs << std::hex << memory[i][j] << endl;
            }
        }
        ofs.close();
    }

}