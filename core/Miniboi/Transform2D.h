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

        mb88 getCosAngleXY() const;
        mb88 getSinAngleXY() const;

        mb88 getAngleXY() const;
        mb88 getAngle() const;

        void setAngleXY(mb88 angleXY);
        void setAngle(mb88 angleXY);

        void rotateAngleXY(mb88 angle);
        void rotateAngle(mb88 angle);

    protected:
        Vect2D m_location;

    private:
        //float m_cosAngleXY;
        //float m_sinAngleXY;
        mb88 m_cosAngleXY;
        mb88 m_sinAngleXY;

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

    inline mb88 Transform2D::getCosAngleXY() const
    {
        return m_cosAngleXY;
    }

    inline mb88 Transform2D::getSinAngleXY() const
    {
        return m_sinAngleXY;
    }


    inline mb88 Transform2D::getAngleXY() const
    {
        //return atan2(m_sinAngleXY, m_cosAngleXY);
        float r1 = mb882float(m_sinAngleXY);
        float r2 = mb882float(m_cosAngleXY);
        return float2mb88(atan2(r1, r2));
    }


    inline void Transform2D::setAngleXY(mb88 angleXY)
    {
        //m_cosAngleXY = cos(angleXY);
        //m_sinAngleXY = sin(angleXY);
        m_cosAngleXY = fxpcos(angleXY);
        m_sinAngleXY = fxpsin(angleXY);
    }

    inline void Transform2D::setAngle(mb88 angleXY)
    {
        setAngleXY(angleXY);
    }


    inline void Transform2D::rotateAngleXY(mb88 angle)
    {
        if (!float_equals(mb882float(angle), 0.0f))
            setAngleXY(getAngleXY() + angle);
    }

    inline void Transform2D::rotateAngle(mb88 angleXY)
    {
        rotateAngleXY(angleXY);
    }
}
#endif // Transform2D_h
