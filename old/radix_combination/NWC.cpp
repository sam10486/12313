#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "NWC_math.h"
#include "BitOperate.h"
#include "math.h"
#include "NWC.h"

using namespace std;

vector<long long> NWC_forward(vector<long long> a, long long degree, long long modular){
    long long t = degree;
    vector<long long> phi_array_rev = phi_array(degree, modular);
    /*ostream_iterator<long long> os(cout, " , ");
    copy(phi_array_rev.begin(), phi_array_rev.end(), os);
    cout << endl;*/

    for(long long m = 1; m < degree; m = m << 1){
        t = t / 2;
        for(long long i = 0; i < m; i++){
            long long j1 = 2*i*t;
            long long j2 = j1 + t - 1;
            long long S = phi_array_rev.at(m+i);
            for(long long j = j1; j <= j2; j++){
                long long U = a.at(j);
                long long V = MulMod(a.at(j+t), S, modular);
                a.at(j) = AddMod(U, V, modular);
                a.at(j+t) = SubMod(U, V, modular);
            }
        }
    }
    return a;
}

ostream_iterator<long long> os(cout, " | ");

vector<long long> NWC_backward(vector<long long> a, long long degree, long long modular){
    long long t = 1;
    vector<long long> phi_array_inv_rev = phi_array_inv(degree, modular);
    for(long long m=degree; m>1; m = m >> 1){
        long long j1 = 0;
        long long h = m / 2;
        for(long long i = 0; i < h; i++){
            long long j2 = j1 + t - 1;
            long long S = phi_array_inv_rev.at(h+i);
            for(long long j = j1; j <= j2; j++){
                long long U = a.at(j);
                long long V = a.at(j+t);
                a.at(j) = AddMod(U, V, modular);
                a.at(j+t) = MulMod(SubMod(U,V,modular), S, modular);
            }
            j1 = j1 + 2*t;
        }
        t = 2*t;
    }
    long long degree_inv = InvMod(degree, modular);
    for(long long j = 0; j < degree; j++){
        a.at(j) = MulMod(a.at(j), degree_inv, modular);
    }
    return a;
}



vector<long long> NWC_backward_merge_scale(vector<long long> a, long long degree, long long modular){
    long long t = 1;
    vector<long long> phi_array_inv_rev = phi_array_inv(degree, modular);
    for(long long m=degree; m>1; m = m >> 1){
        long long j1 = 0;
        long long h = m / 2;
        for(long long i = 0; i < h; i++){
            long long j2 = j1 + t - 1;
            long long S = phi_array_inv_rev.at(h+i);
            for(long long j = j1; j <= j2; j++){
                long long U = a.at(j);
                long long V = a.at(j+t);
                a.at(j) = DivMod(AddMod(U, V, modular), 2, modular);
                a.at(j+t) = MulMod(DivMod(SubMod(U,V,modular), 2, modular), S, modular);
            }
            j1 = j1 + 2*t;
        }
        t = 2*t;
    }
    return a;
}
