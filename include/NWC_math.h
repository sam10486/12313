#ifndef NWC_math
#define NWC_math

#include <iostream>
#include <vector>
#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

long long AddMod(long long, long long, long long);
long long SubMod(long long in_1, long long in_2, long long q);
long long XGCD(long long a, long long q);
long long InvMod(long long a, long long q);
long long DivMod(long long a,long long b,long long q);
long long MulMod(long long a,long long b,long long q);
long long ExpMod(long long a, long long exp, long long q);
long long find_phi(long long n, long long modular); //// p = 1 mod 2n
bool isPrime(long long n);
long long find_prou(long long n, long long modular);

long long find_n_rou(long long base, long long m, long long modular);
bool check_prou(long long n_rou, long long m, long long modular);
long long find_phi_fast(long long m, long long modular);


vector<long long> phi_array(long long n, long long modular);
vector<long long> phi_array_inv(long long n, long long modular);
long long barrett_reduction(long long a, long long b, long long modular);
long long modular_mul(long long a, long long b, long long modular);
long long precompute_value(long long modular, long long bit_width, long long alpha);
long long find_prime(long long m, long long powerof2); // power of 2: m=1, if not power of 2, such as x^105-1 , m=105
long long FFT(long long *DFT_data, long long *data_in, long long n, long long prou, long long modular);
long long FFT_no_bit_reverse(long long *DFT_data, long long *data_in, long long n, long long prou, long long modular);
long long prou_power(long long data_in, long long power, long long modular);
long long DFT(long long *DFT_data, long long *data_in, long long m, long long prou, long long modular);
vector<long long> NWC(vector<long long> a, long long degree, long long phi, long long modular);
vector<long long> INWC(vector<long long> a, long long degree, long long phi, long long modular);
long long NWC_FFT_no_bit_reverse(   long long *DFT_data, long long *data_in, long long n, long long prou, 
                                    long long phi, long long modular);
long long NWC_forward_DIT(  long long *NWC_ans, long long *a, 
                            long long degree, long long phi, long long modular);
long long power2_NTT(   long long *NTT_data, long long *data_in, long long n, 
                        long long *twiddle_array, long long modular);
long long mixed_radix_NWC(  long long *NWC_data, long long *NWC_data_in, 
                            long long n, long long radix_k1, long long radix_k2, long long phi, 
                            long long modular);
long long radix1_part_NWC(  long long *NWC_data, long long *NWC_data_in, 
                            long long n, long long radix_k1, long long radix_k2, long long phi, 
                            long long modular);
long long radix2_part_NWC(  long long *NWC_data, long long *NWC_data_in, 
                            long long n, long long radix_k1, long long radix_k2, long long phi, 
                            long long modular);  
long long mem_init(vector<vector<vector<long long> > > &memory_init, long long *data_in, long long degree_N, long long radix_r);                         
long long mem_AE(vector<vector<vector<long long> > > &memory_init, long long *data_in, long long degree_N, long long radix_r);
long long mem_AE_test(long long t, long long degree_N, long long radix_r, 
                        long long *BN_array, long long *MA_array, long long *Gray_shift_array, long long *BR_j_array);
long long mixed_radix_NWC_AE(long long *NWC_data, long long *NWC_data_in, 
                            long long n, long long radix_k1, long long radix_k2, long long phi, 
                            long long modular, 
                            vector<vector<vector<long long> > > &memory);
long long mem_init_in_place(vector<vector<long long> > &memory_init, long long data_in, long long input_idx, long long N, long long radix,
                            long long modular);

long long mixed_radix_NWC_in_place(  long long *NWC_data, long long *NWC_data_in, 
                            long long n, long long radix_k1, long long radix_k2, long long phi, 
                            long long modular,
                            vector<vector<long long> > &memory);

long long mem_in_place(vector<vector<long long> > &memory, long long data_in, long long input_idx, long long N, long long radix,
                        long long Read_mode, long long Write_mode);
//----------ZZ-------------
ZZ find_n_rou(ZZ base, long long m, ZZ modular);
bool check_prou(ZZ n_rou, long long m, ZZ modular);
ZZ find_phi(long long m, ZZ modular);
bool isPrime(ZZ n);
ZZ find_prime(ZZ m, long long powerof2);
ZZ precompute_value(ZZ modular, long long bit_width, long long alpha);

long long power2_NTT(   ZZ *NTT_data, ZZ *data_in, long long n, 
                        ZZ *twiddle_array, ZZ modular);

long long mem_init_in_place(vector<vector<ZZ> > &memory_init, ZZ data_in, long long input_idx, long long N, long long radix,
                            ZZ modular);

ZZ mem_in_place(vector<vector<ZZ> > &memory, ZZ data_in, long long input_idx, long long N, long long radix,
                        long long Read_mode, long long Write_mode);

long long mixed_radix_NWC_in_place(  ZZ *NWC_data, ZZ *NWC_data_in, 
                            long long n, long long radix_k1, long long radix_k2, ZZ phi, 
                            ZZ modular,
                            vector<vector<ZZ> > &memory);

void ZZ_top(   ZZ *NWC_data, ZZ *NWC_data_in, 
            long long n, long long radix_k1, long long radix_k2, ZZ phi, 
            ZZ modular,
            vector<vector<ZZ> > &memory);

string ZZtoHex(ZZ zz_tmp);
#endif