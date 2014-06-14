#ifndef solidpolygon3d_h
#define solidpolygon3d_h

#include "Vector3D.h"
#include "Polygon3D.h"


namespace Miniboi3D
{
    class SolidPolygon3D : public Polygon3D
    {
    public:
        SolidPolygon3D() : Polygon3D() {}
        SolidPolygon3D(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2) : Polygon3D(v0, v1, v2) {}
        SolidPolygon3D(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2, const Vector3D& v3) : Polygon3D(v0, v1, v2, v3) {}
        SolidPolygon3D(const Vec3DArray& v) : Polygon3D(v) {}

        unsigned int getFillColor() const { return m_color; }
        void setFillColor(unsigned int color) { m_color = color; }
        unsigned int getLineColor() const { return m_linecolor; }
        void setLineColor(unsigned int color) { m_linecolor = color; }

    protected:
    private:
         int8_t m_color;
         int8_t m_linecolor;
    };

}

#endif // solidpolygon3d_h
