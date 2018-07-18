#include <iostream>
#include <fstream>
#include <memory.h>
#include <iomanip>
#include <time.h>
#include <unordered_map>
#include <stdlib.h>
#include "summary.h"
#include "data.h"

//#define TOP_SIZE 170000
//#define NUM 100000


using namespace std;
unordered_map<Data, int, My_Hash> mp;

//argv[1]:cycle
//argv[2]:input_file
//argv[3]:hitter_k
//argv[4]:input_max
//argv[5]:para_lambda
//argv[6]:output_file
//argv[7]:out_model
//argv[8]:label_name
//argv[9]:memory_para1
//argv[10]:memory_para2

void Read_File(int argc, char* argv[]){

    int cycle = atoi(argv[1]);
    int hitter_k = atoi(argv[3]);
    int input_max = atoi(argv[4]);
    int para_lambda = atoi(argv[5]);
    Data* dat = new Data[cycle + 1];
    memset(dat, 0, (cycle + 1) * sizeof(Data));
    ofstream fout(argv[6], ios::app);
    int out_model = atoi(argv[7]);

    int memory_para1 = atoi(argv[9]);
    int memory_para2 = atoi(argv[10]);

    
    Summary summary(memory_para1, memory_para2, cycle, para_lambda);


    unsigned long long int num = 1;
    double SU_dif = 0;

    FILE* file = fopen(argv[2],"rb");
    Data packet;
    int cycle_num = 0;
    double average_recall = 0;
    double average_prec = 0;
    int average_heavy = 0;

    while(fread(packet.str, DATA_LEN, 1, file) > 0)
    {
        if(num >= input_max){
            break;
        }
        unsigned int pos = num % cycle;
        if(num >= cycle){
            mp[dat[pos]] -= 1;
        }

        dat[pos] = packet;

        summary.Init(packet, num);

        if(mp.find(packet) == mp.end())
            mp[packet] = 1;
        else
            mp[packet] += 1;

        double real = mp[packet];

        if(num % cycle == 0){
            int heavy = 0;
            int yes = 0, no = 0;
            unordered_map<Data, int, My_Hash>::iterator it;
            for(it = mp.begin();it != mp.end();++it){
                bool init = false;
                if(it->second > hitter_k){
                    heavy += 1;
                    init = true;
                }
                if(summary.Query(it->first, num) > hitter_k){
                    if(init) yes += 1;
                    else no += 1;
                }
            }
            /*
            cout << summary.memory << endl;
            cout << heavy << " " << yes << " " << no << endl;
            */
            switch(out_model){
            case 1:
                fout << argv[8] << "," << memory_para1 / 1000 << "," << summary.memory << endl; // memory
                break;
            case 2:
                fout << argv[8] << "," << memory_para1 / 1000 << "," << (yes + 0.0)/(yes+no) << endl; //precison rate
                break;
            case 3:
                fout << argv[8] <<"," << memory_para1 / 1000 << "," << (yes + 0.0)/heavy << endl; // recall rate
                break;
            }
            average_recall = average_recall + (yes + 0.0)/heavy;
            average_prec = average_prec + (yes + 0.0)/(yes+no);
            average_heavy = average_heavy + heavy;
            cycle_num ++;
        }

        num++;
    }
    cout << "total average heavy hitter:" << average_heavy / cycle_num << endl;
    cout << "average precision rate:" << average_recall / cycle_num << endl;
    cout <<"average recall rate:" << average_prec / cycle_num << endl;
}

int main(int argc, char* argv[]){
    Read_File(argc, argv);
}
