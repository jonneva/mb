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




#define PI float2mb88(3.14159265f)
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
    mb88 s;
    int i;
    s = (a.intValue + (1<<8)) >> 1;
    /* 6 iterations to converge */
    for (i = 0; i < 6; i++)
		s = (s.intValue + mb88div(a.intValue, s.intValue)) >> 1;
    return s;
}

inline mb88 fabs(mb88 a)
{
	mb88 r;
	r.intValue = a.intValue > 0 ? a.intValue : -a.intValue;
	return r;
}

static const int16_t sin_tab[] = {
0x0000,   /* 0 rad, 0 deg */
0x0324,   /* 0.02454 rad, 1.40625 deg */
0x0648,   /* 0.04908 rad, 2.8125 deg */
0x096A,   /* 0.07363 rad, 4.21875 deg */
0x0C8C,   /* 0.09817 rad, 5.625 deg */
0x0FAB,   /* 0.12271 rad, 7.03125 deg */
0x12C8,   /* 0.14726 rad, 8.4375 deg */
0x15E2,   /* 0.17180 rad, 9.84375 deg */
0x18F9,   /* 0.19634 rad, 11.25 deg */
0x1C0B,   /* 0.22089 rad, 12.6562 deg */
0x1F1A,   /* 0.24543 rad, 14.0625 deg */
0x2223,   /* 0.26998 rad, 15.4687 deg */
0x2528,   /* 0.29452 rad, 16.875 deg */
0x2826,   /* 0.31906 rad, 18.2812 deg */
0x2B1F,   /* 0.34361 rad, 19.6875 deg */
0x2E11,   /* 0.36815 rad, 21.0937 deg */
0x30FB,   /* 0.39269 rad, 22.5 deg */
0x33DF,   /* 0.41724 rad, 23.9062 deg */
0x36BA,   /* 0.44178 rad, 25.3125 deg */
0x398C,   /* 0.46633 rad, 26.7187 deg */
0x3C56,   /* 0.49087 rad, 28.125 deg */
0x3F17,   /* 0.51541 rad, 29.5312 deg */
0x41CE,   /* 0.53996 rad, 30.9375 deg */
0x447A,   /* 0.56450 rad, 32.3437 deg */
0x471C,   /* 0.58904 rad, 33.75 deg */
0x49B4,   /* 0.61359 rad, 35.1562 deg */
0x4C3F,   /* 0.63813 rad, 36.5625 deg */
0x4EBF,   /* 0.66267 rad, 37.9687 deg */
0x5133,   /* 0.68722 rad, 39.375 deg */
0x539B,   /* 0.71176 rad, 40.7812 deg */
0x55F5,   /* 0.73631 rad, 42.1875 deg */
0x5842,   /* 0.76085 rad, 43.5937 deg */
0x5A82,   /* 0.78539 rad, 45 deg */
0x5CB3,   /* 0.80994 rad, 46.4062 deg */
0x5ED7,   /* 0.83448 rad, 47.8125 deg */
0x60EB,   /* 0.85902 rad, 49.2187 deg */
0x62F1,   /* 0.88357 rad, 50.625 deg */
0x64E8,   /* 0.90811 rad, 52.0312 deg */
0x66CF,   /* 0.93266 rad, 53.4375 deg */
0x68A6,   /* 0.95720 rad, 54.8437 deg */
0x6A6D,   /* 0.98174 rad, 56.25 deg */
0x6C23,   /* 1.00629 rad, 57.6562 deg */
0x6DC9,   /* 1.03083 rad, 59.0625 deg */
0x6F5E,   /* 1.05537 rad, 60.4687 deg */
0x70E2,   /* 1.07992 rad, 61.875 deg */
0x7254,   /* 1.10446 rad, 63.2812 deg */
0x73B5,   /* 1.12900 rad, 64.6875 deg */
0x7504,   /* 1.15355 rad, 66.0937 deg */
0x7641,   /* 1.17809 rad, 67.5 deg */
0x776B,   /* 1.20264 rad, 68.9062 deg */
0x7884,   /* 1.22718 rad, 70.3125 deg */
0x7989,   /* 1.25172 rad, 71.7187 deg */
0x7A7C,   /* 1.27627 rad, 73.125 deg */
0x7B5C,   /* 1.30081 rad, 74.5312 deg */
0x7C29,   /* 1.32535 rad, 75.9375 deg */
0x7CE3,   /* 1.34990 rad, 77.3437 deg */
0x7D89,   /* 1.37444 rad, 78.75 deg */
0x7E1D,   /* 1.39899 rad, 80.1562 deg */
0x7E9C,   /* 1.42353 rad, 81.5625 deg */
0x7F09,   /* 1.44807 rad, 82.9687 deg */
0x7F61,   /* 1.47262 rad, 84.375 deg */
0x7FA6,   /* 1.49716 rad, 85.7812 deg */
0x7FD8,   /* 1.52170 rad, 87.1875 deg */
0x7FF5,   /* 1.54625 rad, 88.5937 deg */
0x7FFF    /* 1.57079 rad, 90 deg */
};

/** func  quad   return       func  quad   return
    sin   1      table[n]     cos   1      table[l-n]
    sin   2      table[l-n]   cos   2      -table[n]
    sin   3      -table[n]    cos   3      -table[l-n]
    sin   4      -table[l-n]  cos   4      table[n]
    l = table length
*/

inline mb88 fxpsin (mb88 cosangle) {
    //if (cosangle < )
}

inline mb88 fxpcos (mb88 cosangle) {

}




#endif // MB88_H

