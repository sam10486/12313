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
    ifstream  ifs1, ifs2, ifs3;
    ifs1.open("./check_in_place/algo_asn.txt");
    ifs2.open("./check_in_place/memory_ans.txt");
    ifs3.open("../tb_data/mem_out_rtl/mem_out.txt");
    
    vector<long long > algo_asn_array;
    vector<long long > memory_ans_array;
    vector<long long > memory_out_array;

    long long n = 8192;
    long long radix = 16;
    int BN = radix;
    int MA = (n/BN);
    

    if(!ifs1.is_open() || !ifs2.is_open() || !ifs3.is_open()){
        cout << "failed to open file.\n" << endl;
    }else {
        long long algo_asn;
        long long memory_ans;
        long long mem_out;
        while(ifs2 >> memory_ans){
            //cout << memory_ans << endl;
            memory_ans_array.push_back(memory_ans);
        }
        cout << "----------GGGG-------" << endl;
        while(ifs3 >> mem_out){
            cout << mem_out << endl;
            memory_out_array.push_back(mem_out);
        }
    }
    ifs1.close();
    ifs2.close();
    ifs3.close();
    int correct=0;
    int j;


    int flag = 0;
    int equal_idx = 0;
    int cnt = 0;

    for(int i=0; i<BN; i++){
        for(int j=0; j<MA; j++){
            if(memory_ans_array[i*MA+j] == memory_out_array[i*MA+j]){
                cnt++;
            }else{
                cout << "memory_ans_array[" << i*MA+j << "] = " << memory_ans_array[i*MA+j] << 
                        ", and memory_out_array[" << i*MA+j << "] = " << memory_out_array[i*MA+j] << endl;
            }
        }
    }

    if(cnt == n){
        cout << "ALL PASS!!!! " << endl;
        cout << "cnt = " << cnt << endl;
    }else{
        cout << "Something Error.... " << endl;
        cout << "cnt = " << cnt << endl;
    }
}