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

using namespace std;
using namespace NTL;

int main(){
    ostream_iterator<long long> os(cout, " , ");
    long long degree_R16 = 16;
    long long degree_R32 = 32; 
    vector<long long> a(degree_R16);
    
    long long modular = find_prime(1, 7);
    cout << "modular  = " << modular << endl;
    for(int i=0; i<degree_R16; i++){
        a.at(i) = 1;
    }
    vector<long long> NTT_forward_vec_R16_0 ;
    NTT_forward_vec_R16_0 = NWC_forward(a, degree_R16, modular);
    copy(NTT_forward_vec_R16_0.begin(), NTT_forward_vec_R16_0.end(), os);
    cout << endl;
}