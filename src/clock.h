#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include <string.h>
#include <algorithm>
#include "hash_class.h"

#define STBF_NUM 700

class Recent_Sketch{
public :
    unsigned int clock_pos;
    unsigned int len;
    unsigned int step;
    unsigned long long int last_time;
    int row_length;
    int hash_number;

    Recent_Sketch(unsigned int c, unsigned int l, int _row_length, int _hash_number):len(l),step(l/c),row_length(_row_length),hash_number(_hash_number){
        clock_pos = 0;
        last_time = 0;
    }
};

class Recent_Counter: public Recent_Sketch{
public:
    struct Unit{
        int future;
        int now;
        unsigned int Total(){
            return abs(future) + abs(now);
        }
    };
    Unit* counter;

    Recent_Counter(int c, int l, int _row_length, int _hash_number);
    ~Recent_Counter();
    void Clock_Go(unsigned long long int num);
    void CM_Init(const unsigned char* str, int length, unsigned long long int num);
    void CO_Init(const unsigned char* str, int length, unsigned long long int num);
    void CU_Init(const unsigned char* str, int length, unsigned long long int num);
    int CO_Query(const unsigned char* str, int length);
    unsigned int Query(const unsigned char* str, int length);
};

#endif // CLOCK_H
