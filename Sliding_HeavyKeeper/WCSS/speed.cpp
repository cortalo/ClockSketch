#define _CRT_SECURE_NO_DEPRECATE
#include <fstream>
#include<iostream>
#include <iomanip>
#include <map>
#include <math.h>
#include <time.h>
#include "wcss.h"
#include "data.h"
using namespace std;

map<Data, int> mp,mp1,mp2;

//argv[1]:cycle
//argv[2]:hash_number
//argv[3]:mymemory
//argv[4]:input_num_max
//argv[5]:k
//argv[6]::thre
//argv[7]:in_file
//argv[8]:out_file
int main(int argc,char ** argv)
{

	int cycle = atoi(argv[1]);
	int hash_number = atoi(argv[2]);
	//int row_length = atoi(argv[3]);
	double mymemory = atof(argv[3]);

	int input_num_max = atoi(argv[4]);
	int k = atoi(argv[5]);
	int thre = atoi(argv[6]);
	const char * in_file = argv[7];
	const char * out_file = argv[8];

	int row_length = (mymemory * 1024 - 4 * k)/hash_number/4;

	/*
	int cycle = 10000;
	int hash_number = 100;
	int row_length = 10000;
	int input_num_max = 100001;
	int input_num = 0;
	int k = 5000;
	int thre = 70;
	*/

	int input_num = 0;
	ofstream fout;
    fout.open(out_file, ios::app);
	
	FILE* file = fopen(in_file, "rb");
	Data packet;
	wcss cm(0, cycle, hash_number, row_length, k);
	double real = 0, estiall = 0, estireal = 0;

	double first = clock();
	while (fread(packet.str, DATA_LEN, 1, file) > 0) {
		if (input_num >= input_num_max) {
			break;
		}
		cm.add((char *)packet.str, DATA_LEN);
		input_num++;
	}
	
	double second = clock();
    double duration = ((double)(second-first))/CLOCKS_PER_SEC;
 fout << "WCSS" << "," << mymemory << "," << (input_num_max + 0.0)/ duration / 1000000 << endl;
   cout << "WCSS" << "," << mymemory << "," << (input_num_max + 0.0)/ duration / 1000000 << endl;

	fclose(file);
	fout.close();

	return 0;
}
