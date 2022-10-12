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

void parameter_for_bluestein(){

    long long n = 65536;
    ZZ modular = find_prime((ZZ)1,25);
    cout << "modular = " << modular << endl;

    ZZ prou ;
    prou = find_prou(n, modular);
    cout << "prou = " << prou << endl;
}