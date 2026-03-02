#include "geometry/Point.h"
#include <cmath>

namespace DCP9 {
namespace Geometry {

Point::Point() = default;

Point::Point(double x, double y, double z)
    : m_x(x)
    , m_y(y)
    , m_z(z)
{
}

double Point::distanceTo(const Point& other) const {
    double dx = m_x - other.m_x;
    double dy = m_y - other.m_y;
    double dz = m_z - other.m_z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

bool Point::isValid() const {
    return !m_id.empty() && m_precision >= 0.0;
}

} // namespace Geometry
} // namespace DCP9 