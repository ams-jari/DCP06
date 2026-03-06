#include "geometry/Point.h"
#include <cmath>

namespace DCP9 {
namespace Geometry {

Point::Point() : m_x(0.0), m_y(0.0), m_z(0.0), m_precision(0.0), m_deviation(0.0) {}

Point::~Point() {}

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