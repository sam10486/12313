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
#include <fstream>

using namespace std;

void AE(){
    long long degree_N = 32;
    long long radix_r = 4;
    long long num_stage_p;
    long long bit_width_s;
    long long relocation_group_g;
    int tw_degree = 1;
    int log_radix = log(radix_r) / log(2);
    BitOperate Gray_num, unary_xor, RR, IntToVec, VecToInt;

    ofstream radix_r_AE("./tb_data/SPMB_tw/radix_r_AE.txt");

    num_stage_p = log(degree_N)/log(radix_r);
    relocation_group_g = degree_N/pow(radix_r,2);
    bit_width_s = log(radix_r)/log(2);
    long long BC_width = (long long) ceil(log2(degree_N/radix_r));
    vector<long long> bit_array;
    long long bit_width = log2(degree_N);
    for(long long t=0; t<num_stage_p; t++){
        int cnt = 0 ;
        radix_r_AE << "**************stage = " << t << "*************" << endl;
        if(t != 0) tw_degree = tw_degree * radix_r;
        for(long long i=0; i<relocation_group_g; i++){
            //radix_r_AE << "   cnt = " << cnt << endl;
            radix_r_AE << "---------------relocation group i =" << i << "------------------" << endl;
            for(long long j=0; j<radix_r; j++){
                long long BC = j*relocation_group_g + Gray_num.Gray_code(i, relocation_group_g);
                long long RR_out = RR.RR(BC, bit_width_s*t, degree_N, radix_r);
                
                long long BN = unary_xor.unary_xor(RR_out, BC_width);
                long long MA = (long long) floor(RR_out >> 1);

                //-------compute tw degree------
                int length;
                length = BC >> (log_radix*t);

                //------------------------------

                radix_r_AE << "(BC, BN, MA, length, tw_dg) = ";
                radix_r_AE << "(" << BC << " , "<< BN <<" , "<< MA << " , " << length 
                << " , " << tw_degree * length << ")" << endl;

                IntToVec.IntToVec(BC, degree_N, bit_array);
                rotate(bit_array.begin(), bit_array.begin()+ bit_width_s*t , bit_array.end());
                long long Data = VecToInt.VecToInt(bit_array, degree_N);
                radix_r_AE << "Data = " << Data << endl;
                radix_r_AE << (bit_width-bit_width_s-bit_width_s*t) <<" ";	
                radix_r_AE << "------Data_index = ";
                radix_r_AE << "( " ;					
				for(int k = 0; k < radix_r ; k++ ){
					radix_r_AE << Data + k*(1<<(bit_width-bit_width_s-bit_width_s*t)) <<" ";	
				}
                radix_r_AE << "), tf = (";
                for(int k=0; k < radix_r; k++){
                    radix_r_AE << "w^" << length * tw_degree * k << ", ";
                }
                radix_r_AE << ") \n" ;
            }
            cnt++;
        }
    }
}