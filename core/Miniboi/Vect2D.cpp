#include "Vect2D.h"
#include "Miniboi_math.h"
#include "Miniboi.h"
//#include "mb88.h"

#include <limits>
#include <cmath>
#include <cassert>
#include <sstream>
#include <algorithm>
#include "Transform2D.h"
//#include "viewwindow.h"
//#include "polygon3D.h"

using namespace std;

// Constructors

namespace Miniboi2D {

Vect2D::Vect2D() { x = y = 0.0f; }

Vect2D::Vect2D(mb88 x1, mb88 y1) : x(x1), y(y1) {}

// Vector addition
Vect2D& Vect2D::operator += (const Vect2D& v)
{
    x += v.x;
    y += v.y;

    return *this;
}

Vect2D& Vect2D::operator -= (const Vect2D& v)
{
    x -= v.x;
    y -= v.y;

    return *this; // returns the original reference
}

Vect2D& Vect2D::operator *= (mb88 k)
{
    x *= k;
    y *= k;

    return *this;
}

Vect2D& Vect2D::operator /= (mb88 k)
{
    assert(k > FLT_EPSILON);     // check for divide-by-zero

    x /= k;
    y /= k;

    return *this;
}

bool Vect2D::operator==(const Vect2D& v) const
{
    return (fabs(x - v.x) < FLT_EPSILON
        &&  fabs(y - v.y) < FLT_EPSILON)
        ? true : false;
}


bool Vect2D::operator!=(const Vect2D& v) const
{
    return (fabs(x - v.x) > FLT_EPSILON
        ||  fabs(y - v.y) > FLT_EPSILON)
        ? true : false;
}

string Vect2D::toString() const
{
    ostringstream ostr;

    ostr << fixed << "(" << mb882float(x) << ", " << mb882float(y) << ")" << endl;
    return ostr.str();
}


/**
    Rotate this vector in XY plane
*/
void Vect2D::rotateXY(mb14 cosAngle, mb14 sinAngle) {
    mb14 newX = x.intValue; // a 16-bit value
    mb14 newY = y.intValue;
    x = mb14to88((newX*cosAngle - newY*sinAngle)>>8);
    y = mb14to88((newX*sinAngle + newY*cosAngle)>>8);
}


/**
    Adds the specified transform to this vector. This vector
    is first rotated, then translated.
*/
void Vect2D::add(Transform2D& xform) {

    // rotate
    addRotation(xform);

    // translate
    *this += xform.getLocation();
}


/**
    Subtracts the specified transform to this vector. This
    vector translated, then rotated.
*/
void Vect2D::subtract(Transform2D& xform) {

    // translate
    *this -= xform.getLocation();

    // rotate
    subtractRotation(xform);
}


/**
    Rotates this vector with the angle of the specified
    transform.
*/
void Vect2D::addRotation(Transform2D& xform) {
    rotateXY(xform.getCosAngleXY(), xform.getSinAngleXY());
}


/**
    Rotates this vector with the opposite angle of the
    specified transform.
*/
void Vect2D::subtractRotation(Transform2D& xform) {
    // note that sin(-x) == -sin(x) and cos(-x) == cos(x)
    rotateXY(xform.getCosAngleXY(), -xform.getSinAngleXY());
}

/**
    Scales the vector
*/
void Vect2D::scale(mb88 s) {
    x *= s;
    y *= s;
}

}

