#include "geometry/Plane.h"
#include <cmath>

namespace DCP9 {
namespace Geometry {

Plane::~Plane() {}

Plane::Plane() : m_d(0.0) {
    m_normal.resize(3);
    m_normal[0] = 0.0;
    m_normal[1] = 0.0;
    m_normal[2] = 1.0;  // Default normal is along z-axis
    updatePlaneEquation();
}

Plane::Plane(const Point& p1, const Point& p2, const Point& p3) : m_d(0.0) {
    m_point = p1;
    
    // Calculate normal vector using cross product
    double v1x = p2.x() - p1.x();
    double v1y = p2.y() - p1.y();
    double v1z = p2.z() - p1.z();
    
    double v2x = p3.x() - p1.x();
    double v2y = p3.y() - p1.y();
    double v2z = p3.z() - p1.z();
    
    double nx = v1y * v2z - v1z * v2y;
    double ny = v1z * v2x - v1x * v2z;
    double nz = v1x * v2y - v1y * v2x;
    
    // Normalize the normal vector
    double length = std::sqrt(nx*nx + ny*ny + nz*nz);
    m_normal.resize(3);
    m_normal[0] = nx/length;
    m_normal[1] = ny/length;
    m_normal[2] = nz/length;
    
    updatePlaneEquation();
}

void Plane::setNormal(const std::vector<double>& normal) {
    if (normal.size() != 3) {
        return;
    }
    
    // Normalize the normal vector
    double length = std::sqrt(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]);
    m_normal.resize(3);
    m_normal[0] = normal[0]/length;
    m_normal[1] = normal[1]/length;
    m_normal[2] = normal[2]/length;
    
    updatePlaneEquation();
}

void Plane::updatePlaneEquation() {
    // Calculate d in the plane equation ax + by + cz + d = 0
    m_d = -(m_normal[0] * m_point.x() + m_normal[1] * m_point.y() + m_normal[2] * m_point.z());
}

double Plane::distanceToPoint(const Point& point) const {
    // Distance from point to plane: |ax + by + cz + d| / sqrt(a² + b² + c²)
    double numerator = std::abs(m_normal[0] * point.x() + m_normal[1] * point.y() + m_normal[2] * point.z() + m_d);
    double denominator = std::sqrt(m_normal[0]*m_normal[0] + m_normal[1]*m_normal[1] + m_normal[2]*m_normal[2]);
    return numerator / denominator;
}

Point Plane::projectPoint(const Point& point) const {
    // Project point onto plane
    // Formula: proj = point - ((point - planePoint) · normal) * normal
    double dx = point.x() - m_point.x();
    double dy = point.y() - m_point.y();
    double dz = point.z() - m_point.z();
    
    double dotProduct = dx * m_normal[0] + dy * m_normal[1] + dz * m_normal[2];
    
    return Point(
        point.x() - dotProduct * m_normal[0],
        point.y() - dotProduct * m_normal[1],
        point.z() - dotProduct * m_normal[2]
    );
}

std::vector<Point> Plane::projectPoints(const std::vector<Point>& points) const {
    std::vector<Point> projected;
    projected.reserve(points.size());
    
    for (std::size_t i = 0; i < points.size(); ++i) {
        projected.push_back(projectPoint(points[i]));
    }
    
    return projected;
}

bool Plane::isValid() const {
    return !m_id.empty() && m_point.isValid() && m_normal.size() == 3;
}

bool Plane::isParallelTo(const Plane& other) const {
    // Two planes are parallel if their normal vectors are parallel
    double dotProduct = m_normal[0] * other.m_normal[0] + 
                       m_normal[1] * other.m_normal[1] + 
                       m_normal[2] * other.m_normal[2];
    return std::abs(std::abs(dotProduct) - 1.0) < 1e-10;
}

bool Plane::isPerpendicularTo(const Plane& other) const {
    // Two planes are perpendicular if their normal vectors are perpendicular
    double dotProduct = m_normal[0] * other.m_normal[0] + 
                       m_normal[1] * other.m_normal[1] + 
                       m_normal[2] * other.m_normal[2];
    return std::abs(dotProduct) < 1e-10;
}

} // namespace Geometry
} // namespace DCP9 