#include "vector.H"

/* This namespace collects sdf-related functions (mostly inlined for efficency). */
namespace sdf
{
    using Foam::scalar;
    using Foam::vector;

    const scalar TOL = 1e-8;
    // Helper function: Calculate the cross product of two vectors
    inline vector cross(const vector& v1, const vector& v2)
    {
        return vector(
            v1.y() * v2.z() - v1.z() * v2.y(),
            v1.z() * v2.x() - v1.x() * v2.z(),
            v1.x() * v2.y() - v1.y() * v2.x()
        );
    }
    // Helper function: Calculate the dot product of two vectors
    inline scalar dot(const vector& v1, const vector& v2) 
    {
        return v1.x() * v2.x() + v1.y() * v2.y();
    }
    // Helper function: Calculate the magnitude of a vector
    inline scalar mag(const vector& v)
    {
        return std::sqrt(v.x() * v.x() + v.y() * v.y());
    }
    /*********************************************************************************************
     *                                     Shape primitives                                      *
     * Note: all sdf are calculated in the object space                                          *
     ********************************************************************************************/
    // triangle
    inline bool triangle_bool(const vector& p, const scalar& base, const scalar& height)
   {
    // Vertex at the origin (0,0)
    // The endpoints of the base are (-base/2, -height) and (base/2, -height)
    scalar half_base = base / 2.0;
    
    // Check if the point is within the parallelogram enclosing the triangle
    if (p.y() < -height || p.y() > 0 || p.x() < -half_base || p.x() > half_base)
    {
        return false;
    }
    
    // Check if the point is within the left boundary
    if (p.x() <= 0 && p.y() >= (-height / half_base) * p.x() - height)
    {
        return true;
    }
    
    // Check if the point is within the right boundary
    if (p.x() > 0 && p.y() >= (height / half_base) * p.x() - height)
    {
        return true;
    }
    
    return false;
    }

    inline scalar triangle(const vector& p, const scalar& base, const scalar& height)
    {
    scalar half_base = base / 2.0;

    // Calculate the vertical distance from the point to the base
    scalar dist_to_base = std::fabs(p.y() + height);

    // Calculate the distance from the point to the left boundary
    scalar slope_left = height / half_base;
    scalar dist_to_left = std::fabs(p.y() - slope_left * p.x() + height) / std::sqrt(slope_left * slope_left + 1);

    // Calculate the distance from the point to the right boundary
    scalar slope_right = -height / half_base;
    scalar dist_to_right = std::fabs(p.y() - slope_right * p.x() + height) / std::sqrt(slope_right * slope_right + 1);

    // Return the minimum distance from the point to the triangle's boundaries
    return std::min(dist_to_base, std::min(dist_to_left, dist_to_right));
    }

    // circle
    inline bool circle_bool(const vector& p, const scalar& r)
    {
        return Foam::magSqr(p) < r*r;
    }
    inline bool circle_bool_fast(const vector& p, const scalar& rSQR)
    {
        return Foam::magSqr(p) < rSQR;
    }
    inline scalar circle(const vector& p, const scalar& r)
    {
        return Foam::mag(p) - r;
    }

    // rectangle
    inline scalar rectangle_bool(const vector& p, const scalar& ra, const scalar& rb)
    {
        return std::fabs(p.x()) < ra && std::fabs(p.y()) < rb;
    }
    inline scalar rectangle(const vector& p, const scalar& ra, const scalar& rb)
    {
        scalar dx = std::fabs(p.x()) - ra;
        scalar dy = std::fabs(p.y()) - rb;
        scalar dxp = std::max(0.0, dx);
        scalar dyp = std::max(0.0, dy);
        return std::sqrt(dxp*dxp + dyp*dyp) + std::min(0.0, std::max(dx, dy));
    }

    // box
    inline scalar box_bool(const vector& p, const scalar& ra, const scalar& rb, const scalar& rc)
    {
        return std::fabs(p.x()) < ra && std::fabs(p.y()) < rb && std::fabs(p.z()) < rc;
    }
    inline scalar box(const vector& p, const scalar& ra, const scalar& rb, const scalar& rc)
    {
        scalar dx = std::fabs(p.x()) - ra;
        scalar dy = std::fabs(p.y()) - rb;
        scalar dz = std::fabs(p.z()) - rc;
        scalar dxp = std::max(0.0, dx);
        scalar dyp = std::max(0.0, dy);
        scalar dzp = std::max(0.0, dz);
        return std::sqrt(dxp*dxp+dyp*dyp+dzp*dzp)+std::min(0.0,std::max(dz,std::max(dx, dy)));
    }

    // ellipse
    inline scalar ellipse_bool_fast(const vector& p, const scalar& raSQRINV, const scalar& rbSQRINV)
    {
        return p.x()*p.x()*raSQRINV + p.y()*p.y()*rbSQRINV < 1.0;
    }
    inline scalar ellipse(const vector& p, const scalar& raSQRINV, const scalar& rbSQRINV)
    {
        // raSQRINV = 1/(a*a) and so on
        scalar xbyaSQR = p.x()*p.x()*raSQRINV; // (x/a)^2
        scalar ybybSQR = p.y()*p.y()*rbSQRINV; // (y/b)^2
        return 0.5*(xbyaSQR+ybybSQR-1.0)/(std::sqrt(xbyaSQR*raSQRINV+ybybSQR*rbSQRINV));
    }

    // ellipsoid
    inline scalar ellipsoid_bool_fast(const vector& p, const scalar& raSQRINV, const scalar& rbSQRINV, const scalar& rcSQRINV)
    {
        return p.x()*p.x()*raSQRINV + p.y()*p.y()*rbSQRINV + p.z()*p.z()*rcSQRINV < 1.0;
    }
    inline scalar ellipsoid(const vector& p, const scalar& raSQRINV, const scalar& rbSQRINV, const scalar& rcSQRINV)
    {
        // raSQRINV = 1/(a*a), and so on
        scalar xbyaSQR = p.x()*p.x()*raSQRINV; // (x/a)^2
        scalar ybybSQR = p.y()*p.y()*rbSQRINV; // (y/b)^2
        scalar zbycSQR = p.z()*p.z()*rcSQRINV; // (z/c)^2
        return 0.5*(xbyaSQR+ybybSQR+zbycSQR-1.0)/(std::sqrt(xbyaSQR*raSQRINV+ybybSQR*rbSQRINV+zbycSQR*rcSQRINV));
    }

    /*********************************************************************************************
     *                                     Transformations                                       *
     * Note: to transform the field, the point transformation is reversed.                       *
     * For example, to rotate the field +30\degree, hte point is rotated -30\degree.             *
     ********************************************************************************************/
    // rotation
    inline vector rot30(const vector& p)
    {
        return vector(0.866025404*p.x()+0.5*p.y(), 0.866025404*p.y()-0.5*p.x(), 0.0);
    }
    inline vector rot45(const vector& p)
    {
        return 0.707106781*vector(p.x()+p.y(),-p.x()+p.y(), 0.0);
    }
    inline vector rot60(const vector& p)
    {
        return vector(0.866025404*p.y()+0.5*p.x(),-0.866025404*p.x()+0.5*p.y(), 0.0);
    }
    inline vector rot90(const vector& p)
    {
        return vector(p.y(), -p.x(), 0.0);
    }
    inline vector rotth(const vector& p, const scalar& th)
    {
        scalar s = std::sin(th);
        scalar c = std::cos(th);
        return vector(p.x()*c+p.y()*s,-p.x()*s+p.y()*c, 0.0);
    }

    // reflection
    inline vector flipy(const vector& p)
    {
        return vector( p.x(), -p.y(), p.z());
    }
    inline vector flipx(const vector& p)
    {
        return vector(-p.x(),  p.y(), p.z());
    }
    inline vector offset(const vector& p, const vector& offset)
    {
        return p - offset;
    }

    /*********************************************************************************************
     *                                    Boolean Operations                                     *
     * Note: the operation on Boolean and floating point values are different                    *
     ********************************************************************************************/
    // difference (a binary operation)
    inline scalar D(const scalar& d1, const scalar& d2) { return std::max(d1,-d2);}
    inline bool   D(bool d1,          bool d2         ) { return      d1 && (!d2);}

    // union
    inline scalar U(const std::initializer_list<scalar>& phis) {return std::min(phis);}
    inline bool   U(const std::initializer_list<bool>  & phis) {return std::max(phis);}

    // intersection
    inline scalar I(const std::initializer_list<scalar>& phis) {return std::max(phis);}
    inline bool   I(const std::initializer_list<bool>  & phis) {return std::min(phis);}

    /*********************************************************************************************
     *                                     Helper functions                                      *
     ********************************************************************************************/
    inline scalar filter(const scalar& phi)
    {
        return (std::fabs(phi)<TOL) ? -TOL : phi;
    }

    inline scalar clamp(scalar x, scalar a, scalar b)
    {
        return (x>b) ? b: ((x<a) ? a : x);
    }
}

