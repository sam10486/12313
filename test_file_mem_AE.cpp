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

int main(){

    long long n = 64;
    long long data_in[n];
    long long data_out[n];
    long long radix_mem = 4;
    long long modular = 12289;
    long long phi = find_phi(n, modular);
    cout << "phi = " << phi << endl;
    BitOperate BR;

    ofstream ofs;
    ofs.open("/home/ldap-users/siang/Desktop/NWC_software/check_AE/memory_ans.txt");

    int BN = 2;
    int MA = (n/BN)/radix_mem;
    vector<vector<vector<long long> > > memory;

    memory.resize(BN);
    for(int i=0; i<BN; i++){
        memory[i].resize(MA);
        for(int j=0; j<MA; j++){    
            memory[i][j].resize(radix_mem);
        }
    }

    /*for(int i=0; i<BN; i++){
        for(int j=0; j<MA; j++){
            for(int k=0; k<radix_mem; k++){
                cout << "memory[" << i << "][" << j << "][" << k << "] = " << memory[i][j][k] << endl;
            }
        }
    }*/

    for(int i=0; i<n; i++){
        data_in[i] = i;
    }

    mem_init(memory, data_in, n, radix_mem);
    cout << "--------------" << endl;
    for(int i=0; i<BN; i++){
        for(int j=0; j<MA; j++){
            for(int k=0; k<radix_mem; k++){
                cout << "memory[" << i << "][" << j << "][" << k << "] = " << memory[i][j][k] << endl;
            }
        }
    }
    cout << "-----------------" << endl;
    //mem_AE(memory, data_in, n, radix_mem);
    

    int num = n/radix_mem;
    long long BN_array[num];
    long long MA_array[num];
    //mem_AE_test(0, n, radix_mem, BN_array, MA_array);

    long long log_radix = log2(radix_mem);
    mixed_radix_NWC_AE(data_out, data_in, n, log_radix, log_radix, phi, modular, memory);
    
    /*cout << "--------------" << endl;
    for(int i=0; i<BN; i++){
        for(int j=0; j<MA; j++){
            for(int k=0; k<radix_mem; k++){
                cout << "memory[" << i << "][" << j << "][" << k << "] = " << memory[i][j][k] << endl;
            }
        }
    }*/
    
    if(!ofs.is_open()){
        cout << "failed to open file.\n" << endl;
    }else {
       for(int i=0; i<BN; i++){
        for(int j=0; j<MA; j++){
            for(int k=0; k<radix_mem; k++){
                ofs << memory[i][j][k] << endl;
            }
        }

    }
        ofs.close();
    }




    /*mixed_radix_NWC(data_out, data_in, n, 2, 2, phi, modular);

    for(int i=0; i<n; i++){
        int idx = BR.BitReserve(i, log2(n));
        cout << data_out[idx] << " ";
    }*/
    cout << endl;
}