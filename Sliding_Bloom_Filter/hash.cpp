#include "hash.h"

#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}\

uint Hash::BOBHash32(const uchar* str, uint len, uint num){
    //register ub4 a,b,c,len;
    uint a,b,c;
    /* Set up the internal state */
    a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
    c = prime[num];         /* the previous hash value */

    /*---------------------------------------- handle most of the key */
    while (len >= 12)
    {
        a += (str[0] +((uint)str[1]<<8) +((uint)str[2]<<16) +((uint)str[3]<<24));
        b += (str[4] +((uint)str[5]<<8) +((uint)str[6]<<16) +((uint)str[7]<<24));
        c += (str[8] +((uint)str[9]<<8) +((uint)str[10]<<16)+((uint)str[11]<<24));
        mix(a,b,c);
        str += 12; len -= 12;
    }

    /*------------------------------------- handle the last 11 bytes */
    c += len;
    switch(len)              /* all the case statements fall through */
    {
        case 11: c+=((uint)str[10]<<24);
        case 10: c+=((uint)str[9]<<16);
        case 9 : c+=((uint)str[8]<<8);
        /* the first byte of c is reserved for the length */
        case 8 : b+=((uint)str[7]<<24);
        case 7 : b+=((uint)str[6]<<16);
        case 6 : b+=((uint)str[5]<<8);
        case 5 : b+=str[4];
        case 4 : a+=((uint)str[3]<<24);
        case 3 : a+=((uint)str[2]<<16);
        case 2 : a+=((uint)str[1]<<8);
        case 1 : a+=str[0];
        /* case 0: nothing left to add */
    }
    mix(a,b,c);
    /*-------------------------------------------- report the result */
    return c;
}
