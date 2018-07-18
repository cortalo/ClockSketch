#ifndef BLOOM_H
#define BLOOM_H

#include "data.h"
#include "basic.h"
#include "bitset.h"

class Bloom : public Basic{
public:
    Bloom(uint _LENGTH, uint _HASH_NUM);
    ~Bloom();

    void Init(const Data& data);
    bool Query(const Data& data);

private:
    BitSet* bitset;
};

#endif // BLOOM_H
