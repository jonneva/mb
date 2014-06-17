#ifndef MB88_H
#define MB88_H

#include <string>
#include <cmath>



inline int16_t mb88mul(int16_t a, int16_t b)
{
    return (int16_t)(((int32_t)a * b) >> 8);
}

inline int16_t mb88div(int16_t a, int16_t b)
{
	return (int16_t)((((int32_t)a) << 8) / b);
}




#define PI88 804
#define FLT_EPSILON float2mb88(1.19209290E-07F)



struct mb88 {
	int16_t intValue;
	mb88() {}
	// explicit casting different types
	//mb88(uint8_t i) : intValue(i << 8) {}
	mb88(int i) : intValue(i * 256) {}
	//mb88(uint16_t i) : intValue(i << 8) {}
	//mb88(int16_t i) : intValue(i * 256) {}
	mb88(float f) : intValue((int32_t)(f*256)) {}

    mb88& operator = (mb88 r) { intValue = r.intValue; return *this; }
	mb88& operator += (mb88 r) { intValue += r.intValue; return *this; }
	mb88& operator -= (mb88 r) { intValue -= r.intValue; return *this; }
	mb88& operator *= (mb88 r) { intValue = mb88mul(intValue, r.intValue); return *this; }
	mb88& operator /= (mb88 r) { intValue = mb88div(intValue, r.intValue); return *this; }

	mb88& operator *= (int16_t r) { intValue *= r; return *this; }
	mb88& operator /= (int16_t r) { intValue /= r; return *this; }

	mb88 operator - () const { mb88 x; x.intValue = -intValue; return x; }
	mb88 operator + (mb88 r) const { mb88 x = *this; x += r; return x;}
	mb88 operator - (mb88 r) const { mb88 x = *this; x -= r; return x;}
	mb88 operator * (mb88 r) const { mb88 x = *this; x *= r; return x;}
	mb88 operator / (mb88 r) const { mb88 x = *this; x /= r; return x;}

	bool operator == (mb88 r) const { return intValue == r.intValue; }
	bool operator != (mb88 r) const { return !(*this == r); }
	bool operator <  (mb88 r) const { return intValue < r.intValue; }
	bool operator >  (mb88 r) const { return intValue > r.intValue; }
	bool operator <= (mb88 r) const { return intValue <= r.intValue; }
	bool operator >= (mb88 r) const { return intValue >= r.intValue; }

	mb88 operator + (int16_t r) const { mb88 x = *this; x += r; return x;}
	mb88 operator - (int16_t r) const { mb88 x = *this; x -= r; return x;}
	mb88 operator * (int16_t r) const { mb88 x = *this; x *= r; return x;}
	mb88 operator / (int16_t r) const { mb88 x = *this; x /= r; return x;}
};

// Direct operations with integers !!!!!

inline mb88 float2mb88(float f)
{
    // return (int32_t)(f * (1 << p));
	int32_t temp = f*256;
	mb88 t88;
	t88.intValue = (int16_t) temp;
	//return (int16_t)(f * (1 << 8));
	return t88;
}

inline mb88 operator + (int8_t a, mb88 b)
{ return b + mb88(a); }
inline mb88 operator + (int16_t a, mb88 b)
{ return b + a * 256; } // Discards high 8 bits !


inline mb88 operator - (int8_t a, mb88 b)
{ return -b + a; }
inline mb88 operator - (int16_t a, mb88 b)
{ return -b + a * 256; }


inline mb88 operator * (int8_t a, mb88 b)
{ return b * a; }
inline mb88 operator * (int16_t a, mb88 b)
{ return b * a; }


inline mb88 operator / (int8_t a, mb88 b)
{ mb88 r(a); r /= b; return r; }
inline mb88 operator / (int16_t a, mb88 b)
{ mb88 r(a); r /= b; return r; }



// math functions
// no default implementation
/*
template <int p>
inline fixed_point<p> sin(fixed_point<p> a);

template <int p>
inline fixed_point<p> cos(fixed_point<p> a);

template <int p>
inline fixed_point<p> sqrt(fixed_point<p> a);

template <int p>
inline fixed_point<p> rsqrt(fixed_point<p> a);

template <int p>
inline fixed_point<p> inv(fixed_point<p> a);

template <int p>
inline fixed_point<p> abs(fixed_point<p> a)
{
	fixed_point<p> r;
	r.intValue = a.intValue > 0 ? a.intValue : -a.intValue;
	return r;
}
*/

inline int8_t mb882int (mb88 a)
{
    return a.intValue >>8;
}

inline uint8_t mb88fract (mb88 a)
{
    return a.intValue & 0xFF;
}

inline float mb882float(mb88 f)
{
	return (float) f.intValue / (1 << 8);
}

inline mb88 sqrt(mb88 a)
{
    int32_t a0,x;
    int i;
    /* intvalue is incremented by 256 */
    /* then divided by 2
    for example n = 16 is 4096 in mb88 notation
    then add 256, gives 4352
    then divided by 2 to s = 2176
    then an iterative loop
        s = (s + 4096/s)/2; six times
    THIS IS THE BABYLONIAN METHOD !
    */
    a0 = (int32_t) a.intValue << 8; // for more accuracy
    x = a0 + a.intValue;
    x >>= 1; // x is initial guess
    //x = (a.intValue + (1<<16)) >> 1; // original 16 bit version
    /* 10 iterations to converge */
    for (i = 0; i < 10; i++)
		x = (x + (a0/x)) >> 1;
    //a.intValue = s*16; // to get mb88 answer
    //x /= 16;
    a.intValue = (int16_t)x; // to get mb88 answer
    return a;
}

inline mb88 fabs(mb88 a)
{
	mb88 r;
	r.intValue = a.intValue > 0 ? a.intValue : -a.intValue;
	return r;
}

static const int16_t sin_tab[] = {
0x0000,   /* 0 rad, 0 deg */
0x023C,   /* 0.01745 rad, 1 deg */
0x0478,   /* 0.03490 rad, 2 deg */
0x06B3,   /* 0.05235 rad, 3 deg */
0x08EE,   /* 0.06981 rad, 4 deg */
0x0B28,   /* 0.08726 rad, 5 deg */
0x0D61,   /* 0.10471 rad, 6 deg */
0x0F99,   /* 0.12217 rad, 7 deg */
0x11D0,   /* 0.13962 rad, 8 deg */
0x1406,   /* 0.15707 rad, 9 deg */
0x163A,   /* 0.17453 rad, 10 deg */
0x186C,   /* 0.19198 rad, 11 deg */
0x1A9D,   /* 0.20943 rad, 12 deg */
0x1CCB,   /* 0.22689 rad, 13 deg */
0x1EF7,   /* 0.24434 rad, 14 deg */
0x2121,   /* 0.26179 rad, 15 deg */
0x2348,   /* 0.27925 rad, 16 deg */
0x256C,   /* 0.29670 rad, 17 deg */
0x278E,   /* 0.31415 rad, 18 deg */
0x29AC,   /* 0.33161 rad, 19 deg */
0x2BC7,   /* 0.34906 rad, 20 deg */
0x2DDF,   /* 0.36651 rad, 21 deg */
0x2FF3,   /* 0.38397 rad, 22 deg */
0x3203,   /* 0.40142 rad, 23 deg */
0x3410,   /* 0.41887 rad, 24 deg */
0x3618,   /* 0.43633 rad, 25 deg */
0x381C,   /* 0.45378 rad, 26 deg */
0x3A1C,   /* 0.47123 rad, 27 deg */
0x3C17,   /* 0.48869 rad, 28 deg */
0x3E0E,   /* 0.50614 rad, 29 deg */
0x4000,   /* 0.52359 rad, 30 deg */
0x41EC,   /* 0.54105 rad, 31 deg */
0x43D4,   /* 0.55850 rad, 32 deg */
0x45B6,   /* 0.57595 rad, 33 deg */
0x4793,   /* 0.59341 rad, 34 deg */
0x496A,   /* 0.61086 rad, 35 deg */
0x4B3C,   /* 0.62831 rad, 36 deg */
0x4D08,   /* 0.64577 rad, 37 deg */
0x4ECD,   /* 0.66322 rad, 38 deg */
0x508D,   /* 0.68067 rad, 39 deg */
0x5246,   /* 0.69813 rad, 40 deg */
0x53F9,   /* 0.71558 rad, 41 deg */
0x55A5,   /* 0.73303 rad, 42 deg */
0x574B,   /* 0.75049 rad, 43 deg */
0x58EA,   /* 0.76794 rad, 44 deg */
0x5A82,   /* 0.78539 rad, 45 deg */
0x5C13,   /* 0.80285 rad, 46 deg */
0x5D9C,   /* 0.82030 rad, 47 deg */
0x5F1F,   /* 0.83775 rad, 48 deg */
0x609A,   /* 0.85521 rad, 49 deg */
0x620D,   /* 0.87266 rad, 50 deg */
0x6379,   /* 0.89011 rad, 51 deg */
0x64DD,   /* 0.90757 rad, 52 deg */
0x6639,   /* 0.92502 rad, 53 deg */
0x678D,   /* 0.94247 rad, 54 deg */
0x68D9,   /* 0.95993 rad, 55 deg */
0x6A1D,   /* 0.97738 rad, 56 deg */
0x6B59,   /* 0.99483 rad, 57 deg */
0x6C8C,   /* 1.01229 rad, 58 deg */
0x6DB7,   /* 1.02974 rad, 59 deg */
0x6ED9,   /* 1.04719 rad, 60 deg */
0x6FF3,   /* 1.06465 rad, 61 deg */
0x7104,   /* 1.08210 rad, 62 deg */
0x720C,   /* 1.09955 rad, 63 deg */
0x730B,   /* 1.11701 rad, 64 deg */
0x7401,   /* 1.13446 rad, 65 deg */
0x74EE,   /* 1.15191 rad, 66 deg */
0x75D2,   /* 1.16937 rad, 67 deg */
0x76AD,   /* 1.18682 rad, 68 deg */
0x777F,   /* 1.20427 rad, 69 deg */
0x7847,   /* 1.22173 rad, 70 deg */
0x7906,   /* 1.23918 rad, 71 deg */
0x79BB,   /* 1.25663 rad, 72 deg */
0x7A67,   /* 1.27409 rad, 73 deg */
0x7B0A,   /* 1.29154 rad, 74 deg */
0x7BA2,   /* 1.30899 rad, 75 deg */
0x7C32,   /* 1.32645 rad, 76 deg */
0x7CB7,   /* 1.34390 rad, 77 deg */
0x7D33,   /* 1.36135 rad, 78 deg */
0x7DA5,   /* 1.37881 rad, 79 deg */
0x7E0D,   /* 1.39626 rad, 80 deg */
0x7E6C,   /* 1.41371 rad, 81 deg */
0x7EC0,   /* 1.43116 rad, 82 deg */
0x7F0B,   /* 1.44862 rad, 83 deg */
0x7F4B,   /* 1.46607 rad, 84 deg */
0x7F82,   /* 1.48352 rad, 85 deg */
0x7FAF,   /* 1.50098 rad, 86 deg */
0x7FD2,   /* 1.51843 rad, 87 deg */
0x7FEB,   /* 1.53588 rad, 88 deg */
0x7FFA,   /* 1.55334 rad, 89 deg */
0x7FFF    /* 1.57079 rad, 90 deg */
};

/** func  quad   return       func  quad   return
    sin   1      table[n]     cos   1      table[l-n]
    sin   2      table[l-n]   cos   2      -table[n]
    sin   3      -table[n]    cos   3      -table[l-n]
    sin   4      -table[l-n]  cos   4      table[n]
    l = table length
*/

inline mb88 fxpsin (int angle) {
    // angle comes in as an signed degree integer
    mb88 a;int16_t index;
    if (angle >= 360) angle %= 360; // put angle in 0-360 range
    index = angle % 90; // put index in 1st quadrant
    // if angle = 90, index becomes 0
    if (angle == 90) return 1;
    if (angle == 180) return 0;
    if (angle == 270) return -1;

    if (angle < 90) {
        a.intValue = sin_tab[index] >> 7;
        return a;
    } else if (angle < 180) {
        a.intValue = sin_tab[90-index] >> 7 ; // returns 1 if angle =90
        return a;
    } else if (angle < 270) {
        a.intValue = -sin_tab[index] >> 7 ; // returns 0 if angle = 180
        return a;
    }
    a.intValue = -sin_tab[89-index] >> 7 ; // returns -1 if angle = 270
    return a;
}

inline mb88 fxpcos (mb88 angle) {

}




#endif // MB88_H
