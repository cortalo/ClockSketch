#ifndef DATA_H
#define DATA_H

/*
 * consistent data structure for all Sketch
 */

#include "hash.h"
#include "definition.h"

#define DATA_LEN 4

class Data{
public:
    uchar str[DATA_LEN];
    Data& operator = (const Data& an);
    bool operator < (const Data& an) const;
    bool operator == (const Data& an) const;

    uint Hash(uint num) const;
};

//defined for unordered map
class My_Hash{
public:
    uint operator()(const Data& dat) const;
};

#endif // DATA_H
