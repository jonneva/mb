#ifndef Polygon2D_h
#define Polygon2D_h

#include "Vect2D.h"

namespace Miniboi2D
{

    typedef std::vector<Vect2D> Vec2DArray;

    class Poly2D
    {
    public:
        Poly2D();
        Poly2D(const Vect2D&, const Vect2D&, const Vect2D&);
        Poly2D(const Vect2D&, const Vect2D&, const Vect2D&, const Vect2D&);
        Poly2D(const Vec2DArray&);

        Vect2D& operator[](const size_t idx) { return m_vec2DArray[idx]; }
        const Vect2D& operator[](const size_t idx) const { return m_vec2DArray[idx]; }

        // assignment operators
        Poly2D& operator += (const Vect2D&);
        Poly2D& operator -= (const Vect2D&);

        int getNumVertices() const { return m_numVertices; }
        void add(Transform2D&);
        void subtract(Transform2D&);
        void addRotation(Transform2D&);
        void subtractRotation(Transform2D&);
        void ensureCapacity(int length);
        int8_t fillColor, lineColor;

    protected:
        void insertVertex(int index, float x, float y);
        void deleteVertex(int index);

    private:
        Vec2DArray m_vec2DArray ;   // A polygon is a vector(an array really) of Vector3D
        int m_numVertices;       // The number of vertices in the polygon
    };  // Poly2D
} // Miniboi2D

#endif // Poly2D_h
