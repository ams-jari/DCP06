#include "geometry/Line.h"
#include <cmath>

namespace DCP9 {
namespace Geometry {

Line::~Line() {}

Line::Line() {
    // Default constructor - members are default-initialized
}

Line::Line(const Point& p1, const Point& p2)
    : m_startPoint(p1)
    , m_endPoint(p2)
{
}

double Line::length() const {
    return m_startPoint.distanceTo(m_endPoint);
}

bool Line::isValid() const {
    return !m_id.empty() && m_startPoint.isValid() && m_endPoint.isValid();
}

Point Line::pointAt(double t) const {
    double x = m_startPoint.x() + t * (m_endPoint.x() - m_startPoint.x());
    double y = m_startPoint.y() + t * (m_endPoint.y() - m_startPoint.y());
    double z = m_startPoint.z() + t * (m_endPoint.z() - m_startPoint.z());
    return Point(x, y, z);
}

double Line::distanceToPoint(const Point& point) const {
    // Implementation of point-to-line distance calculation
    // Using the formula: |(point - start) × (end - start)| / |end - start|
    double dx = m_endPoint.x() - m_startPoint.x();
    double dy = m_endPoint.y() - m_startPoint.y();
    double dz = m_endPoint.z() - m_startPoint.z();

    double crossX = (point.y() - m_startPoint.y()) * dz - (point.z() - m_startPoint.z()) * dy;
    double crossY = (point.z() - m_startPoint.z()) * dx - (point.x() - m_startPoint.x()) * dz;
    double crossZ = (point.x() - m_startPoint.x()) * dy - (point.y() - m_startPoint.y()) * dx;

    double crossLength = std::sqrt(crossX*crossX + crossY*crossY + crossZ*crossZ);
    double lineLength = std::sqrt(dx*dx + dy*dy + dz*dz);

    return crossLength / lineLength;
}

bool Line::isParallelTo(const Line& other) const {
    // Two lines are parallel if their direction vectors are scalar multiples
    double dx1 = m_endPoint.x() - m_startPoint.x();
    double dy1 = m_endPoint.y() - m_startPoint.y();
    double dz1 = m_endPoint.z() - m_startPoint.z();

    double dx2 = other.m_endPoint.x() - other.m_startPoint.x();
    double dy2 = other.m_endPoint.y() - other.m_startPoint.y();
    double dz2 = other.m_endPoint.z() - other.m_startPoint.z();

    // Check if the cross product is zero (or very close to zero)
    double crossX = dy1 * dz2 - dz1 * dy2;
    double crossY = dz1 * dx2 - dx1 * dz2;
    double crossZ = dx1 * dy2 - dy1 * dx2;

    double crossLength = std::sqrt(crossX*crossX + crossY*crossY + crossZ*crossZ);
    return crossLength < 1e-10; // Using a small epsilon for floating point comparison
}

std::vector<double> Line::direction() const {
    double dx = m_endPoint.x() - m_startPoint.x();
    double dy = m_endPoint.y() - m_startPoint.y();
    double dz = m_endPoint.z() - m_startPoint.z();
    
    double length = std::sqrt(dx*dx + dy*dy + dz*dz);
    std::vector<double> result(3);
    if (length < 1e-10) {
        result[0] = 0.0;
        result[1] = 0.0;
        result[2] = 0.0;
    } else {
        result[0] = dx/length;
        result[1] = dy/length;
        result[2] = dz/length;
    }
    return result;
}

Point Line::projectPoint(const Point& point) const {
    // Project point onto line using formula:
    // proj = startPoint + t * direction, where t = (point - startPoint) · direction
    std::vector<double> dir = direction();
    
    double dx = point.x() - m_startPoint.x();
    double dy = point.y() - m_startPoint.y();
    double dz = point.z() - m_startPoint.z();
    
    double t = dx * dir[0] + dy * dir[1] + dz * dir[2];
    
    return Point(
        m_startPoint.x() + t * dir[0],
        m_startPoint.y() + t * dir[1],
        m_startPoint.z() + t * dir[2]
    );
}

std::vector<Point> Line::projectPoints(const std::vector<Point>& points) const {
    std::vector<Point> projected;
    projected.reserve(points.size());
    
    for (size_t i = 0; i < points.size(); ++i) {
        projected.push_back(projectPoint(points[i]));
    }
    
    return projected;
}

} // namespace Geometry
} // namespace DCP9 