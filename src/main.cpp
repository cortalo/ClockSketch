#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <unordered_map>
#include "clock.h"
#include "data.h"



using namespace std;
unordered_map<Data, int, My_Hash> mp;


//argv[1]:cycle
//argv[2]:hash_number
//argv[3]:row_length
//argv[4]:file
//argv[5]:input_num_max
//argv[6]:out_file
//argv[7]:out_model
//argv[8]:label_name

void Read_File(int argc, char* argv[]){
    int cycle = atoi(argv[1]);
    int hash_number = atoi(argv[2]);
    int row_length = atoi(argv[3]);
    int input_num_max = atoi(argv[5]);
    int out_model = atoi(argv[7]);
    Recent_Counter CM_Counter(cycle, hash_number * row_length, row_length, hash_number);
    Recent_Counter CU_Counter(cycle, hash_number * row_length, row_length, hash_number);
    Recent_Counter CO_Counter(2 * cycle / 3, hash_number * row_length, row_length, hash_number);

    Data *dat = new Data[cycle + 1];

    unsigned long long int num = 0;
    double CM_dif = 0, CU_dif = 0, CO_dif = 0;
    double CM_ae = 0,  CU_ae = 0,  CO_ae = 0;
    double CM_re = 0,  CU_re = 0,  CO_re = 0;

    FILE* file = fopen(argv[4],"rb");
    Data packet;

    ofstream fout;
    fout.open(argv[6], ios::app);


    while(fread(packet.str, DATA_LEN, 1, file) > 0)
    {

        if(num > input_num_max){
            break;
        }

        unsigned int pos = num % cycle;
        if(num >= cycle){
            mp[dat[pos]] -= 1;
        }


        dat[pos] = packet;

        CM_Counter.CM_Init(packet.str, DATA_LEN, num);
        CU_Counter.CU_Init(packet.str, DATA_LEN, num);
        CO_Counter.CO_Init(packet.str, DATA_LEN, num);

        if(mp.find(packet) == mp.end())
            mp[packet] = 1;
        else
            mp[packet] += 1;

        int CM_guess = CM_Counter.Query(packet.str, DATA_LEN);
        int CU_guess = CU_Counter.Query(packet.str, DATA_LEN);
        int CO_guess = CO_Counter.CO_Query(packet.str, DATA_LEN);

        double real = mp[packet];

        int CM_sub = CM_guess - real;
        int CU_sub = CU_guess - real;
        int CO_sub = CO_guess - real;

        CM_dif += abs(CM_sub);
        CU_dif += abs(CU_sub);
        CO_dif += abs(CO_sub);

        CM_re = CM_re + (double)abs(CM_sub) / real;
        CU_re = CU_re + (double)abs(CU_sub) / real;
        CO_re = CO_re + (double)abs(CO_sub) / real;

        CM_ae += abs(CM_sub);
        CU_ae += abs(CU_sub);
        CO_ae += abs(CO_sub);

        switch(out_model){
        case 1:
            if(num % cycle == 0 && num > 0){
                fout << argv[8] << "," << num << "," << CM_re / num << endl;
            }
            break;
        case 2:
            if(num % cycle == 0 && num > 0){
                fout << argv[8] << "," << num << "," << CU_re / num << endl;
            }
            break;
        case 3:
            if(num % cycle == 0 && num > 0){
                fout << argv[8] << "," << num << "," << CO_re / num << endl;
            }
            break;
        case 4:
            if(num % cycle == 0 && num > 0){
                fout << argv[8] << "," << num << "," << CM_ae / num << endl;
            }
            break;
        case 5:
            if(num % cycle == 0 && num > 0){
                fout << argv[8] << "," << num << "," << CU_ae / num << endl;
            }
            break;
        case 6:
            if(num % cycle == 0 && num > 0){
                fout << argv[8] << "," << num << "," << CO_ae / num << endl;
            }
            break;
        }

        num++;

        
    }

}

int main(int argc, char* argv[]){
    Read_File(argc, argv);
}
