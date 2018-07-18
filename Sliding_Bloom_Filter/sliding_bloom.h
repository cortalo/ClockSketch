#ifndef SLIDING_BLOOM_H
#define SLIDING_BLOOM_H

#include "data.h"
#include "basic.h"
#include "bitset.h"
#include "definition.h"

class Sliding_Bloom : public Basic
{
public:
    Sliding_Bloom(uint _STAGE_LEN, uint _HASH_NUM, uint _CYCLE);
    ~Sliding_Bloom();

    void Init(const Data& data, uint time);
    bool Query(const Data& data, uint time);

private:
    const double STEP;
    const uint STAGE_LEN;
    uint Clock_Pos;
    uint Last_Time;
    BitSet* future;
    BitSet* now;
    void Clock_Go(uint num);
};

#endif // SLIDING_BLOOM_H
