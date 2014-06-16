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


inline int16_t float2mb88(float f)
{
	return (int16_t)(f * (1 << 8));
}

struct mb88 {
	int16_t intValue;
	mb88() {}
	// explicit casting different types
	//mb88(uint8_t i) : intValue(i << 8) {}
	mb88(int i) : intValue(i * 256) {}
	//mb88(uint16_t i) : intValue(i << 8) {}
	//mb88(int16_t i) : intValue(i * 256) {}
	mb88(float f) : intValue(float2mb88(f)) {}

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

#endif // MB88_H

