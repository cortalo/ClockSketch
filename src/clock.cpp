#include "clock.h"

struct Place{
    unsigned int serial;
    unsigned int pos;
};
bool operator < (Place a, Place b){
    return a.serial < b.serial;
}

Recent_Counter::Recent_Counter(int c, int l, int _row_length, int _hash_numberber):Recent_Sketch(c,l,_row_length,_hash_numberber){
    counter = new Unit [l];
    memset(counter, 0, l * sizeof(Unit));
}

Recent_Counter::~Recent_Counter(){
    delete [] counter;
}

void Recent_Counter::CM_Init(const unsigned char* str, int length, unsigned long long int num){
    unsigned int position;
    Clock_Go(num * step);
    for(int i = 0;i < hash_number;++i){
        position = Hash(str, i, length) % row_length + i * row_length;
        counter[position].future += 1;
    }
}

void Recent_Counter::CU_Init(const unsigned char* str, int length, unsigned long long int num){
    int k = clock_pos / row_length;
    unsigned int position = Hash(str, k ,length) % row_length + k * row_length;
    if(position < clock_pos){
        k = (k + 1) % hash_number;
        position = Hash(str, k ,length) % row_length + k * row_length;
    }

    unsigned int height = counter[position].future;
    counter[position].future += 1;

    for(int i = (k + 1) % hash_number;i != k;i = (i + 1) % hash_number){
        position = Hash(str, i ,length) % row_length + i * row_length;
        if(counter[position].future <= height){
            height = counter[position].future;
            counter[position].future += 1;
        }
    }

    Clock_Go((num + 1) * step);
}

unsigned int Recent_Counter::Query(const unsigned char* str, int length){
    unsigned int min_num = 0x7fffffff;

    for(int i = 0;i < hash_number;++i)
        min_num = min(counter[Hash(str, i, length) % row_length + i * row_length].Total(), min_num);

    return min_num;
}

static int Count_Hash[2] = {-1, 1};
void Recent_Counter::CO_Init(const unsigned char *str, int length, unsigned long long num){
    unsigned int position;
    Clock_Go(num * step);
    for(int i = 0;i < hash_number;++i){
        position = Hash(str, i, length) % row_length + i * row_length;
        counter[position].future += Count_Hash[(str[length - 1] + position) & 1];
    }
}


int Recent_Counter::CO_Query(const unsigned char *str, int length){
    int* n = new int[hash_number];
    memset(n, 0, hash_number * sizeof(unsigned int));

    for(int i = 0;i < hash_number;++i)
    {
        unsigned int position = Hash(str, i, length) % row_length + i * row_length;
        //n[i] = counter[position].Total();
        n[i] = (counter[position].future + counter[position].now) * Count_Hash[(str[length - 1] + position) & 1];
    }

    std::sort(n, n + hash_number);

    
    if (((n[hash_number / 2] + n[(hash_number / 2) - 1]) / 2 ) <= 0){
        return 0;
    }

    return (n[hash_number / 2] + n[(hash_number / 2) - 1]) / 2;
}


/*
unsigned int Recent_Counter::CO_Query(const unsigned char *str, int length){
    double * n = new double[hash_number];
    memset(n, 0, hash_number * sizeof(double));

    for(int i = 0;i < hash_number;++i)
    {
        unsigned int position = Hash(str, i, length) % row_length + i * row_length;
        n[i] = counter[position].Total();
        if(clock_pos <= position){
            n[i] = n[i] / ( 1 + double(len - position + clock_pos)/ len);
        }
        else{
            n[i] = n[i] / (1 + double(clock_pos - position) / len);
        }
    }

    std::sort(n, n + hash_number);

    return (n[hash_number >> 1] + n[(hash_number >> 1) - 1]) / 2;
}*/


void Recent_Counter::Clock_Go(unsigned long long int num){
    for(;last_time < num;++last_time){
        counter[clock_pos].now = counter[clock_pos].future;
        counter[clock_pos].future = 0;
        clock_pos = (clock_pos + 1) % len;
    }
}
