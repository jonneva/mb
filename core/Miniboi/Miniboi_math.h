#ifndef MINIBOI_MATH_H
#define MINIBOI_MATH_H

#ifdef MINIBOI_TRIG
#include "Miniboi_trig.h" //only included if needed = less ram
#endif

/**
MACROS
*/

#define mbMax(a,b) (((a) > (b)) ? (a) : (b))
#define mbMin(a,b) (((a) < (b)) ? (a) : (b))
#define mbAbs(a) (((a)<0) ? -(a) : (a))
#define mbSign(a) (((a)<0) ? -1 : (a)>0 ? 1 : 0)

/**
FIXED POINT MATH
*/

typedef long mb14; // an 18.14 fixed point number

#define int2mb(n) ((n) << 14) // convert int to mb14
#define mb2int(n) ((n) >> 14) // convert mb14 to int
#define mbFract(n) ((n) & 0x3fff) // get low 32-bit fraction

#define mb2float(n) (((double)(n)) / (1 << 14)) // convert to float
#define float2mb(n) ((mb14) (((double)n) * (1 << 14))) // float to mb14

#define mbAdd(n1, n2) ((n1) + (n2)) // add two mb14
#define mbSub(n1, n2) ((n1) - (n2)) // substract mb14

#define mbHalf (1 << 13) // 0.5 in mb14

extern mb14 mbDiv(mb14,mb14); // divide a mb14 by mb14
extern mb14 mbMul(mb14,mb14); // multiply a mb14 by mb14




#endif //MINIBOI_MATH_H
