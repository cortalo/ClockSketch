#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <map>
#include <fstream>
#include <iomanip>
#include "BOBHash32.h"
#include "params.h"
#include "ssummary.h"
#include "heavykeeper.h"
#include "spacesaving.h"
#include "LossyCounting.h"
#include "CSS.h"
using namespace std;
map <string ,int> B;
map <string, int> map_hk;
struct node {string x;int y;} p[10000005];

char a[105];
string Read(int read_length, ifstream& fin)
{
    fin.read(a, read_length);
    a[read_length]='\0';
    string tmp=a;
    return tmp;
}
int cmp(node i,node j) {return i.y>j.y;}

//argv[1]:MEM  KB
//argv[2]:K  heavy_hitter
//argv[3]:m, number of flows
//argv[4]:cycle
//argv[5]:read_length
//argv[6]:input_file_name
//argv[7]:field_num
//argv[8]:out_model
//argv[9]:str_name
//argv[10]:output_file

int main(int argc, char* argv[])
{
    ifstream fin(argv[6],ios::in|ios::binary);
    ofstream fout(argv[10], ios::app);

    int MEM,K;

    MEM = atoi(argv[1]);
    K = atoi(argv[2]);
    int cycle = atoi(argv[4]);
    int read_length = atoi(argv[5]);
    int out_model = atoi(argv[8]);
    string* cyc_dat = new string[cycle];

    int m = atoi(argv[3]);
    // preparing heavykeeper
    int hk_M;
    int field_num = atoi(argv[7]);
    int single_size = 8 + (4 * (field_num + 1));
    //for (hk_M=1; single_size*hk_M*HK_d+432*K<=MEM*1024*8; hk_M++); if (hk_M%2==0) hk_M--;
    hk_M = MEM * 1024 / (single_size * HK_d);
    heavykeeper *hk; 
    hk=new heavykeeper(hk_M,cycle,field_num); hk->clear();

    double average_cr = 0;
    double average_rr = 0;
    int average_heavy = 0;
    int out_num = 0;

    // Inserting
    for (int i=1; i<=m; i++)
	{
	    //if (i%(m/10)==0) cout<<"Insert "<<i<<endl;
		string s=Read(read_length, fin);
        
        if(i >= cycle){
            B[cyc_dat[(i%cycle)]] --;
        }
        cyc_dat[(i%cycle)] = s;
		B[s]++;
		hk->Insert(s, i);


        if(i%cycle == 0){
            out_num ++;

            //cout<<"preparing true flow"<<endl;
            // preparing true flow
            int cnt=0;


            cnt = 0;
            double recall = 0;
            double recall_ = 0;
            double real = 0;
            double real_ = 0;
            for(int qi = 0; qi < HK_d; qi++){
                for(int qj = 0; qj < hk_M; qj++){
                    if(map_hk.find(hk->HK[qi][qj].FP) == map_hk.end()){
                        map_hk[hk->HK[qi][qj].FP] = 1;
                        if(hk->num_query(hk->HK[qi][qj].FP) > K){
                            real = real + 1;
                            if(B[hk->HK[qi][qj].FP] > K){
                                real_ = real_ + 1;
                            }
                        }
                    }
                }
            }

            
            //cout << "precision rate:" << real_/real << endl;
            average_cr = average_cr + (real_/real);
            
            for(map <string ,int>::iterator sit = B.begin();sit != B.end(); sit++){
                if(sit->second > K){
                    recall = recall + 1;
                    if(hk->num_query(sit->first) > K){
                        recall_ = recall_ + 1;
                    }
                }
            }

            //cout << "recall rate:" << recall_/recall << endl;
            average_rr = average_rr + (recall_/recall);
            average_heavy = average_heavy + recall;

            switch(out_model){
            case 1:
                fout << argv[9] << "," << argv[1] << "," <<  real_/real << endl; // percision rate
                break;
            case 2:
                fout << argv[9] << "," << argv[1] << "," << recall_/recall << endl; // recall rate
                break;
            }

            map_hk.clear();

            

        }
	}

    /*
    switch(out_model){
    case 1:
        fout << argv[9] <<"," << argv[1] <<"," << average_cr / out_num << endl; // percision rate
        break;
    case 2:
        fout << argv[9] << "," << argv[1] << "," << average_rr / out_num << endl; // recall rate
        break;

    }*/

    cout << "total average heavy hitter:" << average_heavy / out_num << endl;
    cout << "average precision rate:" << average_cr / out_num << endl;
    cout <<"average recall rate:" << average_rr / out_num << endl;
    

	
    return 0;
}
