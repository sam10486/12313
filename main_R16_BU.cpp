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
    ofstream ofs_twiddle, ofs_input_pattern, ofs_output_pattern;
    ostream_iterator<long long> os(cout, " , ");
    
    ofs_twiddle.open("../tb_data/R16/phi_factor.txt");
    ofs_input_pattern.open("../tb_data/R16/input_pattern.txt");
    ofs_output_pattern.open("../tb_data/R16/output_pattern.txt");

    long long n=16;
    //long long prime = find_prime(1, 30);
    long long prime = 65537;
    //long long twiddle = find_phi(n, prime);
    long long twiddle = 15;
    cout << "twiddle = " << twiddle << endl;
    cout << "prime  = " << prime << endl;

    std::default_random_engine generator( time(NULL) );
    std::uniform_real_distribution<float> unif(0, prime);

    if(!ofs_twiddle.is_open() || !ofs_input_pattern.is_open() || !ofs_output_pattern.is_open()){
        cout << "failed to open file.\n" << endl;
    }else {
        vector<long long> a(n);
        /*for(int i=0; i<n; i++){
            long long x = unif(generator);
            ofs_input_pattern << std::hex << i << endl;
            a.at(i) = i;
        }*/

        a.at(0) = 0;
        a.at(1) = 512;
        a.at(2) = 1024;
        a.at(3) = 1536;
        a.at(4) = 2048;
        a.at(5) = 2560;
        a.at(6) = 3072;
        a.at(7) = 3584;
        a.at(8) = 4096;
        a.at(9) = 4608;
        a.at(10) = 5120;
        a.at(11) = 5632;
        a.at(12) = 6144;
        a.at(13) = 6656;
        a.at(14) = 7168;
        a.at(15) = 7680;
        
        long long phi = find_phi(16, prime);
        for(int i=0; i<n;i++){
            ofs_twiddle << std::hex << ExpMod(phi, i, prime) << endl;
        }

        vector<long long> NTT_forward_vec ;
        
        NTT_forward_vec = NWC(a, n, 15, prime); 
        for(int i=0; i<n; i++){
            ofs_output_pattern << std::hex << NTT_forward_vec.at(i) << endl;
        }
        copy(NTT_forward_vec.begin(), NTT_forward_vec.end(), os);
        cout << endl;


        vector<long long> NTT_backward_vec ;
        NTT_backward_vec = INWC(NTT_forward_vec, n, 15, prime);
        int flag;
        for(int i=0; i<n; i++){
            if(NTT_backward_vec.at(i) != a.at(i)){
                cout << "NWC error!" << endl;
                flag = 1;
                break;
            }else{
                flag = 0;
            }
        }
        if(flag == 0)
            cout << "NWC successful!" << endl;

        
        copy(NTT_backward_vec.begin(), NTT_backward_vec.end(), os);
        cout << endl;
    }
}