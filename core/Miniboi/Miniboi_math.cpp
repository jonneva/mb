#include "Miniboi_math.h"

// Divide an mb14 value by another mb14
mb14 mbDiv(mb14 n1 , mb14 n2) {
    long long int n64;
    n64 = (long long int)n1<<14;
    n64 = n64 / n2;
    return (mb14) n64;
}

// Multiply an mb14 value by another mb14
mb14 mbMul(mb14 n1, mb14 n2) {
    long long int n64;
    n64 = (long long int) n1;
    n64 *= (long long int) n2;
    n64 >>= 14;
    return (mb14) n64;
}

