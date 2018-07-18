#ifndef FORGET_BLOOM_H
#define FORGET_BLOOM_H

#include "data.h"
#include "basic.h"
#include "bloom.h"
#include "definition.h"

class Forget_Bloom : public Basic{
public:
    Forget_Bloom(uint _LENGTH, uint _HASH_NUM, uint _BLOOM_NUM, uint _CYCLE);
    ~Forget_Bloom();

    void Init(const Data &data, uint time);
    bool Query(const Data &data, uint time);

private:
    const uint STEP;
    const uint BLOOM_NUM;
    uint Last_Time;
    uint future;
    Bloom **bloom;
    void Update(uint time);
};

#endif // FORGET_BLOOM_H
