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

void BU(){
    ofstream ofs, ofs_up, ofs_down, ofs_pattern_up, ofs_pattern_down;
 
    long long prime = find_prime(1, 9);
    cout << "prime  = " << prime << endl;
    ofs.open("/home/ldap-users/siang/Desktop/NWC_verilog/N26094891/sim/data_file/BU/twiddle_out.txt");
    ofs_up.open("/home/ldap-users/siang/Desktop/NWC_verilog/N26094891/sim/data_file/BU/fft_up.txt");
    ofs_down.open("/home/ldap-users/siang/Desktop/NWC_verilog/N26094891/sim/data_file/BU/fft_down.txt");
    ofs_pattern_up.open("/home/ldap-users/siang/Desktop/NWC_verilog/N26094891/sim/data_file/BU/pattern_up.txt");
    ofs_pattern_down.open("/home/ldap-users/siang/Desktop/NWC_verilog/N26094891/sim/data_file/BU/pattern_down.txt");
    if(!ofs.is_open() || !ofs_up.is_open() || !ofs_down.is_open() || !ofs_pattern_up.is_open() || !ofs_pattern_down.is_open()){
        cout << "failed to open file.\n" << endl;
    }else {
        for(int i=10; i<16;i++){
            for(int j=0; j<16; j++){
                ofs_pattern_up << std::hex << i << endl;
                ofs_pattern_down << std::hex << j << endl; 
            }
        }
        long long degree = 64;
        long long twiddle = find_prou(degree, prime);
 
        int k=0;
        for(int i=10; i < 16; i++){
            for(int j=0; j<16;j++){
                long long fft_up = AddMod(i,j, prime);
                long long pow_twiddle = ExpMod(twiddle, k, prime);
                k = k+1;
                long long fft_down = MulMod(SubMod(i,j, prime), pow_twiddle, prime);
                ofs << std::hex << pow_twiddle << endl;
                ofs_up << std::hex << fft_up << endl;
                ofs_down << std::hex << fft_down << endl; 
            }
        }
        ofs.close();
        ofs_up.close();
        ofs_down.close();
    }


    ofstream    ofs_pattern_up_NWC, ofs_pattern_down_NWC, 
                ofs_twiddle_NWC, ofs_golden_up, ofs_golden_down;

    ofs_pattern_up_NWC.open("/home/ldap-users/siang/Desktop/NWC_verilog/N26094891/sim/data_file/BU_NWC/pattern_up_NWC.txt");
    ofs_pattern_down_NWC.open("/home/ldap-users/siang/Desktop/NWC_verilog/N26094891/sim/data_file/BU_NWC/pattern_down_NWC.txt");
    ofs_twiddle_NWC.open("/home/ldap-users/siang/Desktop/NWC_verilog/N26094891/sim/data_file/BU_NWC/twiddle_NWC.txt");
    ofs_golden_up.open("/home/ldap-users/siang/Desktop/NWC_verilog/N26094891/sim/data_file/BU_NWC/golden_up.txt");
    ofs_golden_down.open("/home/ldap-users/siang/Desktop/NWC_verilog/N26094891/sim/data_file/BU_NWC/golden_down.txt");
    
    if( !ofs_pattern_up_NWC.is_open() || !ofs_pattern_down_NWC.is_open() || 
        !ofs_twiddle_NWC.is_open() || !ofs_golden_up.is_open() || !ofs_golden_down.is_open())   {
        cout << "failed to open file.\n" << endl;
    }else {

        long long num = 16;
        for(int i=8; i<num; i++){
            for(int j=8; j<num; j++){
                ofs_pattern_up_NWC << std::hex << i << endl;
                ofs_pattern_down_NWC << std::hex << j << endl;
            }       
        }
        

        long long degree = 64;
        long long twiddle = find_prou(degree, prime);
        /*vector<long long> twiddle_array;
        for(int i=0; i<num; i++){
            twiddle_array.at(i) = ExpMod(twiddle, i, prime);
            ofs_twiddle_NWC << twiddle_array.at(i) << endl;
        }*/
        
        for(int i = 8; i<num; i++){
            int k=0;
            for(int j=8; j<num; j++){
                long long pow_twiddle_NWC = ExpMod(twiddle, k, prime);
                k = k+1;
                long long pattern_down_NWC_twiddle = MulMod(j, pow_twiddle_NWC, prime);
                long long golden_up = AddMod(i, pattern_down_NWC_twiddle, prime);
                long long golden_down = SubMod(i, pattern_down_NWC_twiddle, prime);
                ofs_twiddle_NWC << std::hex << pow_twiddle_NWC << endl;
                ofs_golden_up << std::hex << golden_up << endl;
                ofs_golden_down << std::hex << golden_down << endl;
            }
        }
        
        ofs_pattern_up_NWC.close();
        ofs_pattern_down_NWC.close();
        ofs_twiddle_NWC.close();
        ofs_golden_up.close();
        ofs_golden_down.close();
    }
}