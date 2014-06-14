#ifndef Transform2D_h
#define Transform2D_h

#include "Vect2D.h"

namespace Miniboi2D
{

    class Transform2D
    {
    public:
        Transform2D() { m_location = Vect2D(0.0f, 0.0f); setAngle(0.0f); };
        Transform2D(float x, float y) { m_location = Vect2D(x, y); setAngle(0.0f); }

        Vect2D& getLocation();
        void setLocation(const Vect2D& v);

        float getCosAngleXY() const;
        float getSinAngleXY() const;

        float getAngleXY() const;
        float getAngle() const;

        void setAngleXY(float angleXY);
        void setAngle(float angleXY);

        void rotateAngleXY(float angle);
        void rotateAngle(float angle);

    protected:
        Vect2D m_location;

    private:
        float m_cosAngleXY;
        float m_sinAngleXY;

    }; // Transform2D

/* INLINE FUNCTIONS */

    inline Vect2D& Transform2D::getLocation()
    {
        return m_location;
    }

    inline void Transform2D::setLocation(const Vect2D& v)
    {
        m_location = v;
    }

    inline float Transform2D::getCosAngleXY() const
    {
        return m_cosAngleXY;
    }

    inline float Transform2D::getSinAngleXY() const
    {
        return m_sinAngleXY;
    }


    inline float Transform2D::getAngleXY() const
    {
        return atan2(m_sinAngleXY, m_cosAngleXY);
    }


    inline void Transform2D::setAngleXY(float angleXY)
    {
        m_cosAngleXY = cos(angleXY);
        m_sinAngleXY = sin(angleXY);
    }

    inline void Transform2D::setAngle(float angleXY)
    {
        setAngleXY(angleXY);
    }


    inline void Transform2D::rotateAngleXY(float angle)
    {
        if (!float_equals(angle, 0.0f))
            setAngleXY(getAngleXY() + angle);
    }

    inline void Transform2D::rotateAngle(float angleXY)
    {
        rotateAngleXY(angleXY);
    }
}
#endif // Transform2D_h
