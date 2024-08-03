#ifndef CIRCLE_TRIANGLETAIL_H
#define CIRCLE_TRIANGLETAIL_H

#include "ishape.h"
namespace sdfibm{

class Circle_TriangleTail : public IShape, _shapecreator<Circle_TriangleTail>
{
private:
    scalar m_radius;
    scalar m_base;
    scalar m_height;
    scalar m_radiusSQR;

public:
    Circle_TriangleTail(const dictionary& para)
    {
        m_radius = Foam::readScalar(para.lookup("radius"));
        m_base = Foam::readScalar(para.lookup("base"));
        m_height = Foam::readScalar(para.lookup("height"));
        m_com = para.lookupOrDefault("com", vector::zero);

        m_radiusSQR = m_radius * m_radius;

        // Assuming the volume and moment of inertia calculations are placeholders
        // These would need to be updated based on the actual shape
        m_volume = M_PI * m_radiusSQR + 0.5 * m_base * m_height;
        m_volumeINV = 1.0 / m_volume;
        scalar tmp = 0.5 * m_volume * m_radiusSQR; // Simplified and not accurate for combined shape
        m_moi[0] = tmp; m_moi[4] = tmp; m_moi[8] = tmp;
        m_moiINV = Foam::inv(m_moi);
        m_radiusB = 2 * std::max(m_radius, std::sqrt((m_base / 2.0) * (m_base / 2.0) + m_height * m_height));
    }

    inline scalar getRadius() const { return m_radius; }
    inline scalar getVolume() const { return m_volume; }

    SHAPETYPENAME("Circle_TriangleTail")
    
    virtual std::string description() const override
    {
        return "Circle_TriangleTail (x-y plane), [r, base, height] = " + std::to_string(m_radius) + ", " + std::to_string(m_base) + ", " + std::to_string(m_height);
    }

        virtual inline bool isInside(const vector& p) const override
    {
        vector p2d = m_com + p; 
        p2d.z() = 0.0;

        bool b1 = sdf::triangle_bool(sdf::offset(sdf::rot90(p2d), vector(0.0, -0.5, 0.0)), m_base, m_height);
        bool b2 = sdf::circle_bool_fast(vector(p2d.x(), p2d.y(), 0.0), m_radiusSQR);

        return sdf::U({b1, b2});
    }

    virtual inline scalar signedDistance(const vector& p) const override
    {
        vector p2d = m_com + p; 
        p2d.z() = 0.0;

        scalar d1 = sdf::circle(p2d, m_radius);
        scalar d2 = sdf::triangle(sdf::offset(sdf::rot90(p2d), vector(0.0, -0.5, 0.0)), m_base, m_height);

        return sdf::filter(sdf::U({d1, d2}));
    }
};

}
#endif

