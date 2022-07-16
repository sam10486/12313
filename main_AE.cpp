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

int main(){
    long long degree_N = 16;
    long long radix_r = 2;
    long long num_stage_p;
    long long bit_width_s;
    long long relocation_group_g;
    BitOperate Gray_num, unary_xor, RR, IntToVec, VecToInt;

    num_stage_p = log(degree_N)/log(radix_r);
    relocation_group_g = degree_N/pow(radix_r,2);
    bit_width_s = log(radix_r)/log(2);
    long long BC_width = (long long) ceil(log2(degree_N/radix_r));
    vector<long long> bit_array;
    long long bit_width = log2(degree_N);

    for(long long t=0; t<num_stage_p; t++){
        cout << "stage = " << t << endl;
        for(long long i=0; i<relocation_group_g; i++){
            for(long long j=0; j<radix_r; j++){
                long long BC = j*relocation_group_g + Gray_num.Gray_code(i, relocation_group_g);
                long long RR_out = RR.RR(BC, bit_width_s*t, degree_N, radix_r);
                
                long long BN = unary_xor.unary_xor(RR_out, BC_width);
                long long MA = (long long) floor(RR_out >> 1);
                cout << "(BC, BN, MA) = ";
                cout << "(" << BC << " , "<< BN <<" , "<< MA << ")" << endl;	
            
                IntToVec.IntToVec(BC, degree_N, bit_array);
                rotate(bit_array.begin(), bit_array.begin()+ bit_width_s*t , bit_array.end());
                long long Data = VecToInt.VecToInt(bit_array, degree_N);
                //cout << "Data = " << Data << endl;
                cout << "Data_index = ";
                cout << "( " ;					
				for(int k = 0; k < radix_r ; k++ ){
					cout << Data + k*(1<<(bit_width-bit_width_s-bit_width_s*t)) <<" ";	
				}
                cout << ") \n" ;
            }
        }
    }
}