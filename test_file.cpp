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
/*
    long long n = 4;
    long long modular = 97;
    long long phi = find_phi(16, modular);
    long long phi_4 = ExpMod(phi, 4, modular);

    cout << "phi = " << phi << endl;
    cout << "phi_4 = " << phi_4 << endl;
    long long W[n];
    W[1] = ExpMod(phi, 8, modular);
    W[2] = ExpMod(phi, 4, modular);
    W[3] = ExpMod(phi, 12, modular);

    cout << "W[1] = " << W[1] << endl;    
    cout << "W[2] = " << W[2] << endl;    
    cout << "W[3] = " << W[3] << endl; 
    long long data_in[n];
    long long NTT_data[n];

    long long NWC_data_in[n];
    long long NWC_data_out[n];
    BitOperate BR;
    for(int i=0; i<n; i++){
        data_in[i] = i;
        //NWC_data_in[i] = BR.BitReserve(i, log2(n));
        NWC_data_in[i] = i;
    }

    cout << "data_in" << endl;
    for(int i=0; i<n; i++){
        cout << data_in[i] << " ";
    }
    cout << endl;
    cout << "NWC_data_in" << endl;
    for(int i=0; i<n; i++){
        cout << NWC_data_in[i] << " ";
    }
    cout << endl;
    power2_NTT(NTT_data, data_in, n, W, modular);
    cout << endl;
    NWC_forward_DIT(NWC_data_out, NWC_data_in, n, phi_4, modular);

    for(int i=0; i<n; i++){
        long long BR_idx = BR.BitReserve(i, log2(n));
        cout << NTT_data[BR_idx] << " ";
    }
    cout << endl;

    for(int i=0; i<n; i++){
        cout << NWC_data_out[i] << " ";
    }
    cout << endl;*/

    ofstream ofs, ofs2;
    ofs.open("/home/ldap-users/siang/Desktop/NWC_software/check_AE/algo_asn.txt");
    ofs2.open("/home/ldap-users/siang/Desktop/NWC_software/check_in_place/algo_asn.txt");

    long long n = 256;
    long long modular = 12289;
    long long phi = find_phi(n, modular);
    long long prou = ExpMod(phi, 2, modular);

    cout << "phi = " << phi << endl;
    long long data_in[n];
    long long mixed_radix_out[n];
    
    long long DFT_in[n];
    long long DFT_data[n];
    for(int i=0; i<n; i++){
        data_in[i] = i;
        DFT_in[i] = i;
        //NWC_data_in[i] = BR.BitReserve(i, log2(n));
        //NWC_data_in[i] = i;
    }

    cout << "data_in" << endl;
    for(int i=0; i<n; i++){
        cout << data_in[i] << " ";
    }
    cout << endl;
    long long radix_mem = 16;
    long long log_radix = log2(radix_mem);
    mixed_radix_NWC(mixed_radix_out, data_in, n, log_radix, log_radix, phi, modular);

    cout << "output" << endl;
    BitOperate BR;
    for(int i=0; i<n;i++){
        int idx = BR.BitReserve(i, log2(n));
        cout << mixed_radix_out[idx] << " ";
    }
    cout << endl;

    NWC_forward_DIT(DFT_data, DFT_in, n, phi, modular);

    cout << "NWC_forward_DIT" << endl;
    for(int i=0; i<n;i++){
        cout << DFT_data[i] << " ";
    }
    cout << endl;

    int err = 0;
    for(int i=0; i<n; i++){
        int idx = BR.BitReserve(i, log2(n));
        if(mixed_radix_out[idx] != DFT_data[i]){
            err++;
        }
    }


    if(err != 0)
        cout << "mix radix error!, error = " << err << endl;
    else 
        cout << "****PASS****" << endl;
    
    
    if(!ofs.is_open() || !ofs2.is_open()){
        cout << "failed to open file.\n" << endl;
    }else {
        for(int i=0; i<n;i++){
            ofs << DFT_data[i] << endl;
            ofs2 << DFT_data[i] << endl;
        }
        ofs.close();
        ofs2.close();
    }
    
    
    
    return 0;
}