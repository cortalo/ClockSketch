#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <string.h>
#include <time.h>
#include <queue>
#include <unordered_map>
#include "sliding_bloom.h"
#include "forget_bloom.h"
#include "dynamic_bloom.h"
#include "data.h"

#define CYCLE 2048000
#define FILE_NUM 1
#define INTERVAL 40960

using namespace std;

struct Node{
    Data data;
    uint time;
};

const string INPUT_FOLDER = "/home/helong/data_subset/";
const string OUTPUT_FOLDER = "/home/helong/zyd/result/";
//const string INPUT_FOLDER = "C:/Project/Data/";
//const string OUTPUT_FOLDER = "C:/Project/";
const string file[] = {"stack"};

void Test_Speed(string INPUT_PATH, string OUTPUT_PATH);
void Test_Time_Speed(string INPUT_PATH, string OUTPUT_PATH);

int main(int argc, char** argv){
    for(int i = 0;i < FILE_NUM;++i){
        string INPUT_PATH = INPUT_FOLDER + file[i]+ ".dat";
        string OUTPUT_PATH = OUTPUT_FOLDER + file[i] + "_time_speed.txt";
        //Test_Speed(INPUT_PATH, OUTPUT_PATH);
        Test_Time_Speed(INPUT_PATH, OUTPUT_PATH);
    }
}

void Test_Time_Speed(string INPUT_PATH, string OUTPUT_PATH){
    cout << INPUT_PATH << endl;
    ofstream output;
    output.open(OUTPUT_PATH.c_str(), ios::app);
    output << "Dataset,Memory(KB),Speed(Mops)" << endl;

    double duration;
    uint num = 0;
    uint Base = 0x48929c85;
    uint time;
    uint start, final;
    Node packet;
    FILE *input;
    Sliding_Bloom* slide;
    Forget_Bloom* forget;

    double Slide_Speed[6] = {0};
    double Forget_Speed[6] = {0};

    for(uint i = 2;i <= 5;++i){
        uint memory = i * INTERVAL;
        cout << memory << endl;

        for(int j = 1;j <= 5;++j){
            slide = new Sliding_Bloom(memory, 10, CYCLE);
            num = 0;
            input = fopen(INPUT_PATH.c_str(),"rb");
            start = clock();
            while(fread(packet.data.str, DATA_LEN, 1, input) > 0){
                fread(&time, DATA_LEN, 1, input);
                num++;
                slide->Init(packet.data, time - Base);
            }
            final = clock();
            fclose(input);
            duration = ((double)(final-start))/CLOCKS_PER_SEC;
            Slide_Speed[i] = (num + 0.0)/ duration / 1000000.0;
            cout << Slide_Speed[i] << endl;
            output << "Sliding," << i * 100 << "," << Slide_Speed[i] << endl;
            delete slide;
        }

        for(int j = 1;j <= 5;++j){
            forget = new Forget_Bloom(2*memory, 8, 10, CYCLE);
            num = 0;
            input = fopen(INPUT_PATH.c_str(),"rb");
            start = clock();
            while(fread(packet.data.str, DATA_LEN, 1, input) > 0){
                fread(&time, DATA_LEN, 1, input);
                num++;
                forget->Init(packet.data, time - Base);
            }
            final = clock();
            fclose(input);
            duration = ((double)(final-start))/CLOCKS_PER_SEC;
            Forget_Speed[i] = (num + 0.0)/ duration / 1000000.0;
            cout << Forget_Speed[i] << endl;
            output << "FBF," << i * 100 << "," << Forget_Speed[i] << endl;
            delete forget;
        }

    }
    output.close();
}

void Test_Speed(string INPUT_PATH, string OUTPUT_PATH){
    cout << INPUT_PATH << endl;
    ofstream output;
    output.open(OUTPUT_PATH.c_str(), ios::app);
    output << "Dataset,Memory(KB),Speed(Mops)" << endl;

    double duration;
    uint num = 0;
    uint time;
    uint start, final;
    Node packet;
    FILE *input;
    Sliding_Bloom* slide;
    Forget_Bloom* forget;

    double Slide_Speed[6] = {0};
    double Forget_Speed[6] = {0};

    for(uint i = 2;i <= 5;++i){
        uint memory = i * INTERVAL;
        cout << memory << endl;

        for(int j = 1;j <= 5;++j){
            slide = new Sliding_Bloom(memory, 10, CYCLE);
            num = 0;
            input = fopen(INPUT_PATH.c_str(),"rb");
            start = clock();
            while(fread(packet.data.str, DATA_LEN, 1, input) > 0){
                fread(&time, DATA_LEN, 1, input);
                slide->Init(packet.data, num++);
            }
            final = clock();
            fclose(input);
            duration = ((double)(final-start))/CLOCKS_PER_SEC;
            Slide_Speed[i] = (num + 0.0)/ duration / 1000000.0;
            cout << Slide_Speed[i] << endl;
            output << "Sliding," << i * 100 << "," << Slide_Speed[i] << endl;
            delete slide;
        }

        for(int j = 1;j <= 5;++j){
            forget = new Forget_Bloom(2*memory, 8, 10, CYCLE);
            num = 0;
            input = fopen(INPUT_PATH.c_str(),"rb");
            start = clock();
            while(fread(packet.data.str, DATA_LEN, 1, input) > 0){
                fread(&time, DATA_LEN, 1, input);
                forget->Init(packet.data, num++);
            }
            final = clock();
            fclose(input);
            duration = ((double)(final-start))/CLOCKS_PER_SEC;
            Forget_Speed[i] = (num + 0.0)/ duration / 1000000.0;
            cout << Forget_Speed[i] << endl;
            output << "FBF," << i * 100 << "," << Forget_Speed[i] << endl;
            delete forget;
        }

    }
    output.close();
}

