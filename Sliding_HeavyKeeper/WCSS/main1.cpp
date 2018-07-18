#define _CRT_SECURE_NO_DEPRECATE
#include <fstream>
#include<iostream>
#include <iomanip>
#include <map>
#include <math.h>
#include "wcss.h"
#include "data.h"
using namespace std;

map<Data, int> mp,mp1,mp2;

//argv[1]:cycle
//argv[2]:hash_number
//argv[3]:row_length
//argv[4]:input_num_max
//argv[5]:k
//argv[6]::thre
//argv[7]:in_file
//argv[8]:out_file
int main(int argc,char ** argv)
{

    //setlocale(LC_ALL,"Chinese-simplified");
	int jindu = 0;
	int cycle = atoi(argv[1]);
	int hash_number = atoi(argv[2]);
	int row_length = atoi(argv[3]);
	int input_num_max = atoi(argv[4]);
	int k = atoi(argv[5]);
	int thre = atoi(argv[6]);
	const char * in_file = argv[7];
	const char * out_file = argv[8];
/*
	int cycle = 10000;
	int hash_number = 10;
	int row_length = 10000;
	int input_num_max = 100001;
	int input_num = 0;
	int k = 5000;
	int thre = 70;
*/
	for (hash_number;hash_number < 60;hash_number += 10)
	{
		mp.clear();
		int input_num = 0;
		Data *dat = new Data[cycle];
		memset(dat, 0, cycle * sizeof(Data));

        ifstream fin;
		fin.open(in_file);
		Data packet;
		ofstream fout;
		fout.open(out_file, ios::app);
		wcss cm(0, cycle, hash_number, row_length, k);
		double real = 0, estiall = 0, estireal = 0;
		int element;
        int timestamp;
		if(jindu==0)
		fout << "Dataset,Memory,Precision" << endl;
		while (fin>>element>>timestamp) {
            sprintf((char*)packet.str,"%d",element);
           // cout<<packet.str<<endl;
			if (input_num >= input_num_max) {
				break;
			}
			if (input_num >= cycle) {
				mp[dat[input_num % cycle]] -= 1;
			}
			dat[input_num % cycle] = packet;
			if (mp.find(packet) == mp.end()) {
				mp[packet] = 1;
			}
			else {
				mp[packet] += 1;
			}

			cm.add((char *)packet.str, DATA_LEN);
			if (input_num%cycle == 0)
			{
				jindu++;
				cout << jindu << endl;
			}

			if (input_num % cycle == 0 && input_num > 0) {
				real = 0;
				estiall = 0;
				estireal = 0;
				mp1.clear();
				mp2.clear();
				for (int i = 0;i < cycle;i++)
				{
					if (mp[dat[i]] >= thre&&mp1.find(dat[i]) == mp1.end())
					{
						real++;
						//cout << mp[dat[i]] << ' ';
						mp1[dat[i]] = mp[dat[i]];
						//cout << cm.query((char *)dat[i].str, DATA_LEN) << endl;
						if (cm.query((char *)dat[i].str, DATA_LEN) > thre)
						{
							estireal++;
						}
					}
					if (cm.query((char *)dat[i].str, DATA_LEN) >= thre&&mp2.find(dat[i]) == mp2.end())
					{
						estiall++;
						mp2[dat[i]] = cm.query((char *)dat[i].str, DATA_LEN);
					}
				}
				cout << real << ' ' << estireal << ' ' << estiall << endl;
				//fout <<" zhaohui"<<  ":" <<estireal/real<< ' ';
				//fout << "zhunque" << ":" <<estireal/estiall<< endl;
				fout << "WCSS,"<<hash_number*10<<',' << estireal / estiall << endl;
			}

			input_num++;
		}
        fin.close();
	fout.close();
	}
	//fout << endl;
	return 0;
}
