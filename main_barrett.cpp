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
    long long n = 8192;
    ZZ modular = find_prime((ZZ)1,27);
    cout << "modular = " << modular << endl;
    long long bit_width = 31;
    long long alpha = bit_width + 1;

    ZZ value = precompute_value(modular, bit_width, alpha);
    cout << "value = " << value << endl;
}