#include "Vect2D.h"
#include "Poly2D.h"
#include "Transform2D.h"

using namespace Miniboi2D;
using namespace std;

Poly2D::Poly2D()
{
    m_numVertices = 0;
}


Poly2D::Poly2D(const Vect2D& v0, const Vect2D& v1, const Vect2D& v2)
{
    m_numVertices = 3;
    m_vec2DArray.push_back(v0);
    m_vec2DArray.push_back(v1);
    m_vec2DArray.push_back(v2);
}


Poly2D::Poly2D(const Vect2D& v0, const Vect2D& v1, const Vect2D& v2, const Vect2D& v3)
{
    m_numVertices = 4;
    m_vec2DArray.push_back(v0);
    m_vec2DArray.push_back(v1);
    m_vec2DArray.push_back(v2);
    m_vec2DArray.push_back(v3);
}


Poly2D::Poly2D(const Vec2DArray& v)
{
    m_numVertices = (int)v.size();
    m_vec2DArray = v;
}


// transform each vertex by adding the vector
Poly2D& Poly2D::operator += (const Vect2D& v)
{
    for (int i=0; i!=m_numVertices; i++)
    {
        m_vec2DArray[i] += v;
    }
    return *this;
}


// transform each vertex by subtracting the vector
Poly2D& Poly2D::operator -= (const Vect2D& v)
{
    for (int i = 0; i != m_numVertices; i++)
    {
        m_vec2DArray[i] -= v;
    }
    return *this;
}


void Poly2D::add(Transform2D& xform)
{
    // rotate
    addRotation(xform);

    // translate
    *this += xform.getLocation();
}


void Poly2D::subtract(Transform2D& xform)
{
    // translate
    *this -= xform.getLocation();

    // rotate
    subtractRotation(xform);
}


void Poly2D::addRotation(Transform2D& xform) {
    for (int i=0; i!=m_numVertices; i++) {
        m_vec2DArray[i].addRotation(xform);
    }
}


void Poly2D::subtractRotation(Transform2D& xform) {
    for (int i=0; i!=m_numVertices; i++) {
        m_vec2DArray[i].subtractRotation(xform);
    }
}



// Increase the size of the vertex array by length
void Poly2D::ensureCapacity(int length)
{
    if ((int)m_vec2DArray.size() < length)
    {
        m_vec2DArray.resize(length);
    }
}

void Poly2D::insertVertex(int index, float x, float y)
{
    Vect2D newVertex(m_vec2DArray[m_vec2DArray.size()-1]);

    newVertex.x = x;
    newVertex.y = y;

    for (int i = (int)m_vec2DArray.size()-1; i > index; i--) {
        m_vec2DArray[i] = m_vec2DArray[i-1];
    }
    m_vec2DArray[index] = newVertex;
    m_numVertices++;
}


void Poly2D::deleteVertex(int index)
{
    Vect2D deleted(m_vec2DArray[index]);

    for (int i = index; i < (int)m_vec2DArray.size()-1; i++) {
        m_vec2DArray[i] = m_vec2DArray[i+1];
    }
    m_vec2DArray[m_vec2DArray.size()-1] = deleted;
    m_numVertices--;
}


