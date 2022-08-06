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
    long long prime = 786433;
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
        for(int i=0; i<n; i++){
            long long x = unif(generator);
            ofs_input_pattern << std::hex << i << endl;
            a.at(i) = i;
        }
        
        long long phi = find_phi(n, prime);
        for(int i=0; i<n;i++){
            ofs_twiddle << std::hex << ExpMod(phi, i, prime) << endl;
        }

        vector<long long> NTT_forward_vec ;
        
        NTT_forward_vec = NWC_forward(a, n, prime); 
        for(int i=0; i<n; i++){
            ofs_output_pattern << std::hex << NTT_forward_vec.at(i) << endl;
        }
        copy(NTT_forward_vec.begin(), NTT_forward_vec.end(), os);
        cout << endl;


        vector<long long> NTT_backward_vec ;
        NTT_backward_vec = NWC_backward(NTT_forward_vec, n, prime);
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