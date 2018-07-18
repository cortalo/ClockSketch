#include <fstream>
#include <iomanip>
#include <map>
#include <math.h>
#include "sketch.h"
#include "data.h"

map<Data, int> mp;

//argv[1]:cycle  argv[2]:update_k
//argv[3]:hash_number  argv[4]:row_length
//argv[5]:file    argv[6]:input_num_max
//argv[7]:out_file
//argv[8]:out_model
//argv[9]:label_name
//argv[10]:memory_str
int main(int argc, char* argv[])
{
	int cycle = atoi(argv[1]);
    Data *dat = new Data[cycle];
	int update_k = atoi(argv[2]);
	int hash_number =  atoi(argv[3]);
	int row_length =  atoi(argv[4]);
	int out_model =  atoi(argv[8]);

	
    ECM ecm(cycle, update_k, hash_number, row_length);
	
	ECU ecu(cycle,update_k,hash_number,row_length);

    ECO eco(cycle,update_k,hash_number,row_length);

	unsigned long long int input_num_max =  atoi(argv[6]);
    unsigned long long int input_num = 0;
    FILE* file = fopen(argv[5], "rb");
    Data packet;
    ofstream fout;
    fout.open(argv[7], ios::app);  
    //fout << fixed << setprecision(1);
    double are_ecm = 0,are_ecu=0,are_eco=0;
    double aae_ecm = 0,aae_ecu=0,aae_eco=0;
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
        ecm.update(packet.str, DATA_LEN, input_num);
		ecu.update(packet.str, DATA_LEN, input_num);
        eco.update(packet.str, DATA_LEN, input_num);
        double dif_ecm = fabs(ecm.query(packet.str, DATA_LEN, input_num) - mp[packet]);
        double dif_ecu= fabs(ecu.query(packet.str, DATA_LEN, input_num) - mp[packet]);
        double dif_eco= fabs(eco.query(packet.str, DATA_LEN, input_num) - mp[packet]);
        
        are_ecm = are_ecm + (dif_ecm/mp[packet]);
        aae_ecm = aae_ecm + dif_ecm;
		are_ecu = are_ecu + (dif_ecu / mp[packet]);
		aae_ecu = aae_ecu + dif_ecu;
        are_eco = are_eco + (dif_eco / mp[packet]);
		aae_eco = aae_eco + dif_eco;
        
        switch(out_model){
        case 1:
            if(input_num % cycle == 0){
                fout << argv[9]<<","  << argv[10] <<"," << (double)ecm.q_memory(input_num) <<  endl;
                cout << argv[9]<<","  << (double)input_num <<"," << (double)ecm.q_memory(input_num) <<  endl;
                cout << mp.size() << endl;
            }
            break;
        case 2:
            if(input_num % cycle == 0 && input_num / cycle >= 1){
                fout << argv[9] << "," << (double)input_num  << "," << are_ecm/input_num << endl;
                cout << argv[9] << "," << (double)input_num  << "," << are_ecm/input_num << endl;
                cout << mp.size() << endl;
            }
			break;
        case 3:
            if(input_num % cycle == 0 && input_num / cycle >= 1){
                fout << argv[9] << "," << (double)input_num  << "," << aae_ecm/input_num << endl;
                cout << argv[9] << "," << (double)input_num  << "," << aae_ecm/input_num << endl;
                cout << mp.size() << endl;
            }
			break;
		case 4:
			if (input_num % cycle == 0) {
				fout << argv[9] << "," << (double)input_num << "," << (double)ecu.q_memory(input_num) << endl;
			}
			break;
		case 5:
			if (input_num % cycle == 0 && input_num / cycle >= 1) {
				fout << argv[9] << "," << (double)input_num << "," << are_ecu / input_num << endl;
			}
			break;
		case 6:
			if (input_num % cycle == 0 && input_num / cycle >= 1) {
				fout << argv[9] << "," << (double)input_num << "," << aae_ecu / input_num << endl;
			}
			break;
        case 7:
			if (input_num % cycle == 0) {
				fout << argv[9] << "," << (double)input_num << "," << (double)eco.q_memory(input_num) << endl;
			}
			break;
		case 8:
			if (input_num % cycle == 0 && input_num / cycle >= 1) {
				fout << argv[9] << "," << (double)input_num << "," << are_eco / input_num << endl;
			}
			break;
		case 9:
			if (input_num % cycle == 0 && input_num / cycle >= 1) {
				fout << argv[9] << "," << (double)input_num << "," << aae_eco / input_num << endl;
			}
			break;
        }
        

        input_num ++;                       
    }
}