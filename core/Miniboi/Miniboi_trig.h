#ifndef MINIBOI_TRIG_H
#define MINIBOI_TRIG_H

#include "Miniboi_math.h"

/**
SINE & COSINE TABLES & CALCULATIONS
*/

extern mb14 mbSine[];
extern mb14 mbCosine[];

#define mbSin(n) (mbSine[(n) & 0xff])
#define mbCos(n) (mbCosine[(n) & 0xff])

#endif //MINIBOI_TRIG_H

