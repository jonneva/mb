#ifndef VECT2D_H
#define VECT2D_H

//#include <iostream>
#include <string>
//#include <vector>
#include <cmath>
//#include <limits>


#define FLT_EPSILON 1.19209290E-07F // decimal constant
#define PI 3.14159265

/* OPERATOR OVERLOADS */
/*
template<typename T>
const T operator+(const T& lhs, const T& rhs)
{
    return T(lhs) += rhs;
}

template<typename T>
const T operator-(const T& lhs, const T& rhs)
{
    return T(lhs) -= rhs;
}

template<typename T>
const T operator/(const T& lhs, float rhs)
{
    if (rhs==0) return 0;
    return T(lhs) /= rhs;
}

template<typename T>
const T operator*(const T& lhs, float rhs)
{
    return T(lhs) *= rhs;
}

template<typename T>
const T operator*(float lhs, const T& rhs)
{
    return T(rhs) *= lhs;
}
*/
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
        float x;
        float y;

        Vect2D();

        Vect2D(float x, float y);

        // assignment operators
        Vect2D& operator += (const Vect2D&);
        Vect2D& operator -= (const Vect2D&);
        Vect2D& operator *= (float);
        Vect2D& operator /= (float);

        // equality operators
        bool operator == (const Vect2D&) const;
        bool operator != (const Vect2D&) const;

        std::string toString() const;

        float length() const;
        void  normalize();
        float dot(const Vect2D&) const;
        void  cross(const Vect2D&, const Vect2D&);

        void add(Transform2D&); // & means passing a reference as variable
        void subtract(Transform2D&); // using a reference, you can manipulate
        void addRotation(Transform2D&); // the original data using a variable
        void subtractRotation(Transform2D&); // in a safer way than a pointer

    private:
        void rotateXY(float,float);

    };  // Vect2D


/* INLINE FUNCTIONS */

    inline float Vect2D::length() const
    {
        return sqrt(x*x + y*y);
    }

    inline void Vect2D::normalize()
    {
        *this /= length();
    }

    inline float Vect2D::dot(const Vect2D& v) const
    {
        return x*v.x + y*v.y;
    }

}

#endif // VECT2D_H

