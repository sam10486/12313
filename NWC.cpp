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

vector<long long> NWC_forward_DIT(vector<long long> a, long long degree, long long phi, long long modular){
    BitOperate BR;
    vector<long long> A_BR(degree);
    // vector s a bit reverse
    long long bit_width = log2(degree);
    for(long long i=0; i<degree; i++){
        long long idx_BR = BR.BitReserve(i, bit_width);
        A_BR.at(i) = a.at(idx_BR);
    }

    /*for(int i=0; i<degree; i++){
        cout << "A_BR = " << A_BR.at(i) << endl;
    }*/

    for(long long s=1; s<=log2(degree); s++){
        cout << "s = " << s << endl;
        long long m = ExpMod(2, s, modular);
        //cout << "m = " << m << endl;
        for(long long j=0; j<=(m/2)-1; j++){
            long long twiddle_dg = ((2*j+1) * degree)/m;
            long long twiddle = ExpMod(phi, twiddle_dg, modular);
            //cout << "j = " << j << endl;
            cout << "twiddle_dg = " << twiddle_dg << endl;
            for(long long k=0; k<=(degree/m)-1; k++){
                //cout << "k = " << k << endl;
                long long u = A_BR.at(k*m+j);
                long long t = MulMod(twiddle, A_BR.at(k*m+j+(m/2)), modular);
                A_BR.at(k*m+j) = AddMod(u, t, modular);
                A_BR.at(k*m+j+(m/2)) = SubMod(u, t, modular);
                /*cout << "u = " << u << endl;
                cout << "t = " << t << endl;
                cout << "A_BR.at " << (k*m+j) << "= " << A_BR.at(k*m+j) << endl;
                cout << "A_BR.at " << (k*m+j+(m/2)) << " = " << A_BR.at(k*m+j+(m/2)) << endl;
                cout << "------------------" << endl;*/
            }
        }
    }

    return A_BR;
}

