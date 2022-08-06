#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include <bitset>
#include "NWC_math.h"
#include "BitOperate.h"
#include "math.h"
#include "NWC.h"

using namespace std;

void CF(){
    long long degree_N = 256;
    long long radix_r = 4;
    long long num_stage_p;
    long long bit_width_s;
    long long relocation_group_g;
    BitOperate Int2Vec, Vec2Int, RR;

    num_stage_p = log(degree_N)/log(radix_r);
    relocation_group_g = degree_N/pow(radix_r,2);
    bit_width_s = log(radix_r)/log(2);

    vector<long long> bit_array;

    for(long long t = 0; t<(num_stage_p) ; t++){
        cout << "stage = " << t << endl;
        for(long long i=0; i<(relocation_group_g); i++){
            //cout << "relocation group = " << i << endl;
            for(long long j=0; j<(radix_r); j++){
                //cout << "j = " << j << endl;
                long long BC = j*relocation_group_g + i;
                //cout << "bit_width_s*t = " << bit_width_s*t << endl ;
                long long MA = RR.RR(BC, bit_width_s*t, degree_N, radix_r);
                cout << "(BC, MA) = " << "(" << BC << ", " << MA << ")" << endl;

                Int2Vec.IntToVec(BC,degree_N, bit_array);
                rotate(bit_array.begin(), bit_array.begin()+ bit_width_s*t , bit_array.end());
                long long Data = Vec2Int.VecToInt(bit_array, degree_N);
                cout << "Data_index = ( ";
                long long bit_width = log2(degree_N);
                for(long long k=0; k<radix_r; k++){
                    cout << Data + k*(1<<(bit_width-bit_width_s-bit_width_s*t)) << "  ";
                }				
                cout << ") \n";
            }
        }
    }

}