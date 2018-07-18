#ifndef BITSET_H
#define BITSET_H

/*
 * My bitset
 */

#include <string.h>
#include "definition.h"

class BitSet
{
public:
    BitSet(uint _LENGTH);
    ~BitSet();
    void Set(uint index);
    void Clear(uint index);
    bool Get(uint index);

private:
    uchar* bitset;
};

#endif // BITSET_H
