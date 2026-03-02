#pragma once

#include "geometry/Point.h"
#include <string>
#include <vector>

namespace DCP9 {
namespace Geometry {

class Plane {
public:
    Plane();
    Plane(const Point& p1, const Point& p2, const Point& p3);
    ~Plane() = default;

    // Getters
    const Point& point() const { return m_point; }
    const std::vector<double>& normal() const { return m_normal; }
    const std::string& id() const { return m_id; }
    double a() const { return m_normal[0]; }
    double b() const { return m_normal[1]; }
    double c() const { return m_normal[2]; }
    double d() const { return m_d; }
    bool isValid() const;

    // Setters
    void setPoint(const Point& point) { m_point = point; }
    void setNormal(const std::vector<double>& normal);
    void setId(const std::string& id) { m_id = id; }

    // Operations
    double distanceToPoint(const Point& point) const;
    Point projectPoint(const Point& point) const;
    std::vector<Point> projectPoints(const std::vector<Point>& points) const;  // Project multiple points onto plane
    bool isParallelTo(const Plane& other) const;
    bool isPerpendicularTo(const Plane& other) const;

private:
    Point m_point;
    std::vector<double> m_normal;
    double m_d{0.0}; // Plane equation: ax + by + cz + d = 0
    std::string m_id;

    void updatePlaneEquation();
};

} // namespace Geometry
} // namespace DCP9 
