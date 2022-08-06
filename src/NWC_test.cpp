#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include "NWC_math.h"
#include "BitOperate.h"
#include "math.h"
#include "NWC.h"

using namespace std;

void NWC_test(){
    int in_1 = 2;
    int in_2 = 5;
    int modular = 12289;
    long long ans =0;
    int n = 2048;
    BitOperate rev;
    ostream_iterator<long long> os(cout, " , ");
    
    std::default_random_engine generator( time(NULL) );
    std::uniform_real_distribution<float> unif(0, modular);

    /*vector<long long> phi_array_rev = phi_array(n, modular);
    copy(phi_array_rev.begin(), phi_array_rev.end(), os);
    cout << endl;
    vector<long long> phi_array_inv_rev = phi_array_inv(n, modular);
    copy(phi_array_inv_rev.begin(), phi_array_inv_rev.end(), os);
    cout << endl;*/
    
    cout << "-------------------------input data--------------------" << endl;
    vector<long long> a(n);
    vector<long long> b(n);
    for(int i=0; i<n; i++){
        long long x = unif(generator);
        long long y = unif(generator);
        a.at(i) = i;
        b.at(i) = i;
    }    

    copy(a.begin(), a.end(), os);
    cout << endl;
    copy(b.begin(), b.end(), os);
    cout << endl;
    cout << "-------------------DFT ans -----------------------" << endl;
    vector<long long> NTT_forward_vec ;
    NTT_forward_vec = NWC_forward(a, n, modular);
    /*copy(NTT_forward_vec.begin(), NTT_forward_vec.end(), os);
    cout << endl;*/
    cout << "---------------IDFT ans-------------------" << endl;
    vector<long long> NTT_backward_vec ;
    long long rev_index;

    NTT_backward_vec = NWC_backward(NTT_forward_vec, n, modular);
    /*copy(NTT_backward_vec.begin(), NTT_backward_vec.end(), os);
    cout << endl;*/

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

    //----------------check element wise product---------------
    vector<long long> NTT_forward_in1 , NTT_forward_in2;
    vector<long long> NTT_forward_product(n);
    NTT_forward_in1 = NWC_forward(a, n, modular);
    NTT_forward_in2 = NWC_forward(b, n, modular);
    for(long long int i=0; i<n; i++){
        NTT_forward_product.at(i) = MulMod(NTT_forward_in1.at(i), NTT_forward_in2.at(i), modular);
    }
    vector<long long> NTT_backward_product ;
    NTT_backward_product = NWC_backward(NTT_forward_product, n, modular);
    copy(NTT_backward_product.begin(), NTT_backward_product.end(), os);
    cout << endl;
}

