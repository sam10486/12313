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
//----------ZZ-------------
ZZ find_n_rou(ZZ base, long long m, ZZ modular);
bool check_prou(ZZ n_rou, long long m, ZZ modular);
ZZ find_phi(long long m, ZZ modular);
bool isPrime(ZZ n);
ZZ find_prime(ZZ m, long long powerof2);
ZZ precompute_value(ZZ modular, long long bit_width, long long alpha);
#endif