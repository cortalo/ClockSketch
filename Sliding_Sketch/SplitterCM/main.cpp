#include <fstream>
#include <iomanip>
#include <map>
#include <math.h>
#include "splitter.h"
#include "data.h"

map<Data, int> mp;

//argv[1]:cycle
//argv[2]:hash_number
//argv[3]:row_length
//argv[4]:file
//argv[5]:input_num_max
//argv[6]:out_file
//argv[7]:out_model
//argv[8]:label_name
int main(int argc, char* argv[])
{
    double tau = 0.05;
    double mu = 1.5;
    int cycle = atoi(argv[1]);
    int hash_number = atoi(argv[2]);
    int row_length = atoi(argv[3]);
    int input_num_max = atoi(argv[5]);
    int out_model = atoi(argv[7]);

    Bucket cm(cycle, tau, mu, hash_number, row_length);
    //Bucket cu(cycle, tau, mu, hash_number, row_length);
    //Bucket co(cycle, tau, mu, hash_number, row_length);
    int input_num = 0;

    Data *dat = new Data[cycle];
    memset(dat, 0, cycle * sizeof(Data));

    FILE* file = fopen(argv[4], "rb");
    Data packet;
    ofstream fout;
    fout.open(argv[6], ios::app);  
    double are_cm = 0;
    double aae_cm = 0;
    double are_cu = 0;
    double aae_cu = 0;
    double are_co = 0;
    double aae_co = 0;
    while(fread(packet.str, DATA_LEN, 1, file) > 0){
        if(input_num >= input_num_max){
            break;
        }
        if(input_num >= cycle){
            mp[dat[input_num % cycle]] -= 1;
        }
        dat[input_num % cycle] = packet;
        if(mp.find(packet) == mp.end()){
            mp[packet] = 1;
        }
        else{
            mp[packet] += 1;
        }
        cm.update(packet.str, DATA_LEN, input_num);
        //cu.cu_update(packet.str, DATA_LEN, input_num);
        //co.count_update(packet.str, DATA_LEN, input_num);

        double dif_cm = fabs(cm.query(packet.str, DATA_LEN) - mp[packet]);
        //double dif_cu = fabs(cu.query(packet.str, DATA_LEN) - mp[packet]);
        //double dif_co = fabs(co.count_query(packet.str, DATA_LEN) - mp[packet]);

        are_cm = are_cm + (dif_cm/mp[packet]);
        aae_cm = aae_cm + dif_cm;

        //are_cu = are_cu + (dif_cu/mp[packet]);
        //aae_cu = aae_cu + dif_cu;

        //are_co = are_co + (dif_co/mp[packet]);
        //aae_co = aae_co + dif_co;

        
        switch(out_model){
        case 1:
            if(input_num % cycle == 0 && input_num > 0){
                fout << argv[8]<<","  << (double)input_num <<"," << (double)cm.q_memory() <<  endl;
                cout << argv[8]<<","  << (double)input_num <<"," << (double)cm.q_memory() <<  endl;
            }
            break;
        case 2:
            if(input_num % cycle == 0 && input_num > 0){
                fout << argv[8]<<","  << (double)input_num <<"," << are_cm/input_num <<  endl;
                cout << argv[8]<<","  << (double)input_num <<"," << are_cm/input_num <<  endl;
            }
            break;
        case 3:
            if(input_num % cycle == 0 && input_num > 0){
                fout << argv[8]<<","  << (double)input_num <<"," << aae_cm/input_num <<  endl;
                cout << argv[8]<<","  << (double)input_num <<"," << aae_cm/input_num <<  endl;
            }
            break;
        
        }

        input_num ++;                       
    }

}