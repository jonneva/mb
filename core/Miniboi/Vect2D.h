#ifndef VECT2D_H
#define VECT2D_H

//#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "mb88.h"
//#include <limits>

#define FLT_EPSILON 1.19209290E-07F // decimal constant
#define PI 3.14159265

namespace Miniboi2D
{
    class Transform2D; // Forward declaration

    inline bool float_equals(float lhs, float rhs)
    {
        return fabs(lhs - rhs) < FLT_EPSILON ? true : false;
    }

    inline float DegToRad(float degree)
    {
        return degree * (PI / 180.0f);
    }

    inline float RadToDeg(float radian)
    {
        return radian * (180.0f / PI);
    }

    class Vect2D
    {
    public:
        mb88 x;
        mb88 y;

        Vect2D();
        Vect2D(mb88 x, mb88 y);

        // assignment operators
        Vect2D& operator += (const Vect2D&);
        Vect2D& operator -= (const Vect2D&);
        Vect2D& operator *= (mb88);
        Vect2D& operator /= (mb88);

        // equality operators
        bool operator == (const Vect2D&) const;
        bool operator != (const Vect2D&) const;

        std::string toString() const;

        mb88 length() const;
        void  normalize();
        mb88 dot(const Vect2D&) const;
        void  cross(const Vect2D&, const Vect2D&);

        void add(Transform2D&); // & means passing a reference as variable
        void subtract(Transform2D&); // using a reference, you can manipulate
        void addRotation(Transform2D&); // the original data using a variable
        void subtractRotation(Transform2D&); // in a safer way than a pointer
        void scale(mb88);

    private:
        void rotateXY(mb14,mb14);

    };  // Vect2D


/* INLINE FUNCTIONS */

    inline mb88 Vect2D::length() const
    {
        // return sqrt(x*x + y*y); // the original formula
        // i need to make these 32-bit to be able to hold the bigger
        // values
        mb88 l;
        uint32_t x32 = x.intValue;
        uint32_t y32 = y.intValue;
        x32 *= x32;
        y32 *= y32;
        x32 += y32;
        x32 = sqrt(x32);
        l.intValue = (uint16_t)x32;
        return l;
    }

    inline void Vect2D::normalize()
    {
        *this /= length();
    }

    inline mb88 Vect2D::dot(const Vect2D& v) const
    {
        return x*v.x + y*v.y;
    }

}

#endif // VECT2D_H

