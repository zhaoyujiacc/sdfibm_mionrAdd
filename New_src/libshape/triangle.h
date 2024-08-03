#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "ishape.h"
#include <algorithm> // For std::max
#include <cmath>     // For std::sqrt

namespace sdfibm {

class Triangle : public IShape, _shapecreator<Triangle>
{
private:
    scalar m_base;
    scalar m_height;

public:
    Triangle(const dictionary& para)
    {
        m_base = Foam::readScalar(para.lookup("base"));
        m_height = Foam::readScalar(para.lookup("height"));
        m_com = para.lookupOrDefault("com", vector::zero);

        // Set inherited variables
        m_volume = 0.5 * m_base * m_height; // Area of the triangle, treated as volume for consistency
        m_volumeINV = 1.0 / m_volume;

        scalar tmp = m_volume * (m_height*m_height + m_base*m_base/12);
        m_moi[0] = tmp; m_moi[4] = tmp; m_moi[8] = tmp;
        m_moiINV = Foam::inv(m_moi);
        m_radiusB = std::max(m_height, m_base / 2.0);
    }

    SHAPETYPENAME("Triangle")
    virtual std::string description() const override
    {
        return "isosceles triangle (x-z plane), base parallel to y-axis, height parallel to x-axis, base = " + std::to_string(m_base) + ", height = " + std::to_string(m_height);
    }

    virtual inline bool isInside(const vector& p) const override
    {
        vector localP = p - m_com; // Transform point to local triangle space
        
        // Given the triangle's symmetry and orientation, calculate if the point is within the triangle
        scalar absY = std::abs(localP.y());
        scalar triangleHeightAtY = (m_height / (m_base / 2.0)) * (m_base / 2.0 - absY);
        
        return absY <= (m_base / 2.0) && localP.x() >= 0 && localP.x() <= triangleHeightAtY;
    }

    virtual inline scalar signedDistance(const vector& p) const override
    {
        // This simple implementation only checks inside/outside
        return isInside(p) ? -1.0 : 1.0;
    }
};

}

#endif

