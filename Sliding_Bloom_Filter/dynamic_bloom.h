#ifndef DYNAMIC_BLOOM_H
#define DYNAMIC_BLOOM_H

#include "data.h"
#include "basic.h"
#include "bloom.h"
#include "definition.h"

class Dynamic_Bloom : public Basic
{
public:
    Dynamic_Bloom(uint _LENGTH, uint _HASH_NUM, uint _CYCLE);
    ~Dynamic_Bloom();

    void Init(const Data &data, uint time);
    bool Query(const Data &data, uint time);

private:
    const uint CYCLE;
    uint Last_Time;
    Bloom* future;
    Bloom* now;
    void Update(uint time);
};

#endif // DYNAMIC_BLOOM_H
