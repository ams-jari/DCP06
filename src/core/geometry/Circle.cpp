#include "geometry/Circle.h"
#include <cmath>

namespace DCP9 {
namespace Geometry {

Circle::Circle() {
    m_normal = {0.0, 0.0, 1.0}; // Default normal is along z-axis
}

Circle::Circle(const Point& center, double radius, const std::vector<double>& normal)
    : m_center(center)
    , m_radius(radius)
    , m_normal(normal)
{
    // Normalize the normal vector
    if (normal.size() == 3) {
        double length = std::sqrt(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]);
        if (length > 1e-10) {
            m_normal[0] /= length;
            m_normal[1] /= length;
            m_normal[2] /= length;
        }
    }
}

void Circle::setNormal(const std::vector<double>& normal) {
    if (normal.size() != 3) {
        return;
    }
    
    // Normalize the normal vector
    double length = std::sqrt(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]);
    if (length > 1e-10) {
        m_normal[0] = normal[0] / length;
        m_normal[1] = normal[1] / length;
        m_normal[2] = normal[2] / length;
    }
}

double Circle::distanceToPoint(const Point& point) const {
    // Distance from point to circle arc in 3D space
    // Project point onto plane containing the circle
    // Then calculate distance from projected point to circle
    
    // Vector from center to point
    double dx = point.x() - m_center.x();
    double dy = point.y() - m_center.y();
    double dz = point.z() - m_center.z();
    
    // Project onto plane (remove component along normal)
    double dotProduct = dx * m_normal[0] + dy * m_normal[1] + dz * m_normal[2];
    double projX = dx - dotProduct * m_normal[0];
    double projY = dy - dotProduct * m_normal[1];
    double projZ = dz - dotProduct * m_normal[2];
    
    // Distance from center in plane
    double distInPlane = std::sqrt(projX*projX + projY*projY + projZ*projZ);
    
    // Distance from point to circle = |distInPlane - radius|
    return std::abs(distInPlane - m_radius);
}

Point Circle::projectPoint(const Point& point) const {
    // Project point onto circle (nearest point on circle arc)
    
    // Vector from center to point
    double dx = point.x() - m_center.x();
    double dy = point.y() - m_center.y();
    double dz = point.z() - m_center.z();
    
    // Project onto plane (remove component along normal)
    double dotProduct = dx * m_normal[0] + dy * m_normal[1] + dz * m_normal[2];
    double projX = dx - dotProduct * m_normal[0];
    double projY = dy - dotProduct * m_normal[1];
    double projZ = dz - dotProduct * m_normal[2];
    
    // Normalize to get direction from center
    double distInPlane = std::sqrt(projX*projX + projY*projY + projZ*projZ);
    if (distInPlane > 1e-10) {
        projX /= distInPlane;
        projY /= distInPlane;
        projZ /= distInPlane;
    } else {
        // Point is at center, use arbitrary direction
        // Find a vector perpendicular to normal
        if (std::abs(m_normal[0]) < 0.9) {
            projX = 1.0 - m_normal[0] * m_normal[0];
            projY = -m_normal[0] * m_normal[1];
            projZ = -m_normal[0] * m_normal[2];
        } else {
            projX = -m_normal[1] * m_normal[0];
            projY = 1.0 - m_normal[1] * m_normal[1];
            projZ = -m_normal[1] * m_normal[2];
        }
        double len = std::sqrt(projX*projX + projY*projY + projZ*projZ);
        if (len > 1e-10) {
            projX /= len;
            projY /= len;
            projZ /= len;
        }
    }
    
    // Point on circle at radius distance
    return Point(
        m_center.x() + m_radius * projX,
        m_center.y() + m_radius * projY,
        m_center.z() + m_radius * projZ
    );
}

bool Circle::isOnPlane(const Point& point, double tolerance) const {
    // Check if point is on the plane containing the circle
    double dx = point.x() - m_center.x();
    double dy = point.y() - m_center.y();
    double dz = point.z() - m_center.z();
    
    double distance = std::abs(dx * m_normal[0] + dy * m_normal[1] + dz * m_normal[2]);
    return distance < tolerance;
}

bool Circle::isValid() const {
    return !m_id.empty() && m_radius > 0.0 && m_normal.size() == 3;
}

} // namespace Geometry
} // namespace DCP9





















