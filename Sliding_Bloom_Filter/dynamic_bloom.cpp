#include "dynamic_bloom.h"

Dynamic_Bloom::Dynamic_Bloom(uint _LENGTH, uint _HASH_NUM, uint _CYCLE):
    Basic(_LENGTH, _HASH_NUM), CYCLE(_CYCLE){
    future = new Bloom(LENGTH, HASH_NUM);
    now = new Bloom(LENGTH, HASH_NUM);
    Last_Time = 0;
}

Dynamic_Bloom::~Dynamic_Bloom(){
    delete future;
    delete now;
}

void Dynamic_Bloom::Init(const Data &data, uint time){
    Update(time / CYCLE);
    future->Init(data);
}

bool Dynamic_Bloom::Query(const Data &data, uint time){
    Update(time / CYCLE);
    if(future->Query(data))
        return true;
    else{
        if(now->Query(data)){
            future->Init(data);
            return true;
        }
    }
    return false;
}

void Dynamic_Bloom::Update(uint time){
    for(;Last_Time < time;++Last_Time){
        delete now;
        now = future;
        future = new Bloom(LENGTH, HASH_NUM);
    }
}
