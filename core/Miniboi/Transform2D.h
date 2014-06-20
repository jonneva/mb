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

        mb14 getCosAngleXY() const;
        mb14 getSinAngleXY() const;

        mb14 getAngleXY() const;
        mb14 getAngle() const;

        void setAngleXY(mb14 angleXY);
        void setAngle(mb14 angleXY);

        void rotateAngleXY(mb14 angle);
        void rotateAngle(mb14 angle);

    protected:
        Vect2D m_location;

    private:
        //float m_cosAngleXY;
        //float m_sinAngleXY;
        mb14 m_cosAngleXY;
        mb14 m_sinAngleXY;

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

    inline mb14 Transform2D::getCosAngleXY() const
    {
        return m_cosAngleXY;
    }

    inline mb14 Transform2D::getSinAngleXY() const
    {
        return m_sinAngleXY;
    }


    inline mb14 Transform2D::getAngleXY() const
    {
        //return atan2(m_sinAngleXY, m_cosAngleXY);
        //float r1 = mb882float(m_sinAngleXY);
        //float r2 = mb882float(m_cosAngleXY);
        float r1 = mb2float(m_sinAngleXY);
        float r2 = mb2float(m_cosAngleXY);
        return float2mb(atan2(r1, r2));
    }


    inline void Transform2D::setAngleXY(mb14 angleXY)
    {
        //m_cosAngleXY = cos(angleXY);
        //m_sinAngleXY = sin(angleXY);
        int angleDeg; int32_t accAngle; // needed to prevent overflow
        //accAngle = angleXY.intValue * 180; // old mb88 version
        accAngle = angleXY * 180; // 32 bits should be enuff
        //accAngle /= PI88;
        accAngle /= PI14;
        angleDeg = accAngle; // the conversion is included in PI14 !
        m_cosAngleXY = fxpcos(angleDeg);
        m_sinAngleXY = fxpsin(angleDeg);
    }

    inline void Transform2D::setAngle(mb14 angleXY)
    {
        setAngleXY(angleXY);
    }


    inline void Transform2D::rotateAngleXY(mb14 angle)
    {
        // this was first float, then mb88, but mb88 is not nearly
        // accurate enough
        //if (!float_equals(mb882float(angle), 0.0f))
            setAngleXY(float2mb(getAngleXY()) + angle);
    }

    inline void Transform2D::rotateAngle(mb14 angleXY)
    {
        rotateAngleXY(angleXY);
    }
}
#endif // Transform2D_h
