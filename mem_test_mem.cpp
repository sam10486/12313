
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

long long mem_in_place_AGU(long long input_idx, long long N, long long radix,
                         long long Read_mode, long long Write_mode);

int main(){

    long long radix_k1 = 4;
    long long radix_k2 = 1;
    long long n = 8192;

    long long k = ( log2(n) - radix_k2) / radix_k1;
    long long parameter_check = radix_k1 * k + radix_k2;
    assert(parameter_check == log2(n));
    int pow_radix_k1 = pow(2, radix_k1);
    BitOperate BR;
    ofstream ofs_AGU_k1_BN, ofs_AGU_k1_MA, ofs_mem_data;

    int BN = pow(2, radix_k1);
    int MA = (n/BN);
    long long mem[BN][MA] = {-1};

    cout << "c78" << endl;
    for(int i=0; i<BN; i++){
        for(int j=0; j<MA; j++){
            mem[i][j] = i*BN+j;
            cout << "mem[" << i << "][" << j << "] = " << mem[i][j] << endl;
            //cout << "i*BN+j = " << i*BN+j << endl;
        }
    }
    cout << "**************" << endl;
    for(int i=0; i<BN; i++){
        for(int j=0; j<MA; j++){
            cout << mem[i][j] << endl;
        }
    }

    cout << "QQ" << endl;
    ofs_AGU_k1_BN.open("/home/ldap-users/siang/Desktop/NWC_verilog/N26094891/sim/data_file/mem/mem_algo_bank.txt");
    ofs_AGU_k1_MA.open("/home/ldap-users/siang/Desktop/NWC_verilog/N26094891/sim/data_file/mem/mem_algo_MA.txt");
    ofs_mem_data.open("/home/ldap-users/siang/Desktop/NWC_verilog/N26094891/sim/data_file/mem/mem_algo_data.txt");
    long long delta = log2(pow_radix_k1);

    if(!ofs_AGU_k1_BN.is_open() || !ofs_AGU_k1_MA.is_open() || !ofs_mem_data.is_open()){
        cout << "failed to open file.\n" << endl;
    }else {
        //k radix 2^k1 NTTs
        for(long long l=0; l<1; l++){
            //cout << endl;
            cout << "----j means group number, i means numbers of BU in one group----" << endl;
            for(int j=0; j<pow(2, radix_k1*l); j++){
                long long j_bar = BR.BitReserve(j, radix_k1*l);
                for(int i=0; i<pow(2, log2(n)- radix_k1*(l+1)); i++){
                    for(int m=0; m<pow(2, radix_k1); m++){
                        long long index = j_bar * pow(2, log2(n) - radix_k1*l) + m * pow(2, log2(n) - radix_k1*(l+1)) + i;
                        long long Bank = mem_in_place_AGU(index, n, pow_radix_k1, 1, 0);
                        long long Addr = index >> delta;
                        //cout << "index = " << index << endl;
                        //ofs_AGU_k1_BN << std::hex << Bank << endl;
                        //ofs_AGU_k1_MA << std::hex << Addr << endl;
                        //ofs_mem_data << std::hex << mem[Bank][Addr] << endl;
                        //cout << "Bank = " << Bank << ", MA = " << Addr << ", data = " << mem[Bank][Addr] << endl;
                    }
                }
                //cout << endl;
            }
        } 
    } 

    cout << "nima" << endl;
    for(int i=0; i<MA;i++){
        for(int j=0; j<BN;j++){
            cout << "Bank = " << j << ", MA = " << i << ", data = " << mem[j][i] << endl;
            ofs_mem_data << std::hex << mem[j][i] << endl;
            ofs_AGU_k1_BN << std::hex << j << endl;
            ofs_AGU_k1_MA << std::hex << i << endl;
        }
    }
}


long long mem_in_place_AGU(long long input_idx, long long N, long long radix,
                         long long Read_mode, long long Write_mode){

    BitOperate BO;
    long long Bank = 0;
    long long delta = log2(radix);
    long long bank_upper_bound = floor((log2(N)) / delta);
    long long bit_width_N = delta * (bank_upper_bound+1);
    vector<long long> Order = BO.DecToBin(input_idx, bit_width_N);
    int Order_tmp_size = (delta * (bank_upper_bound+1)) - (delta * bank_upper_bound);
    vector<long long> Order_tmp(Order_tmp_size);
    for(int i=0; i<=bank_upper_bound; i++){
        int Order_start = delta * i;
        int Order_end = delta * (i+1) - 1;
        for(int j=Order_start; j<=Order_end; j++){
            Order_tmp[j-Order_start] = Order[j];
        }
        long long integer_Order_tmp = BO.VecToInt(Order_tmp, pow(2,Order_tmp_size));
        Bank = AddMod(Bank, integer_Order_tmp, radix);
    }
    return Bank;
}