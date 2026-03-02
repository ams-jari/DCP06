#pragma once

#include "geometry/Point.h"
#include <string>
#include <vector>

namespace DCP9 {
namespace Geometry {

class Circle {
public:
    Circle();
    Circle(const Point& center, double radius, const std::vector<double>& normal);
    ~Circle() = default;

    // Getters
    const Point& center() const { return m_center; }
    double radius() const { return m_radius; }
    double diameter() const { return 2.0 * m_radius; }
    const std::vector<double>& normal() const { return m_normal; }
    const std::string& id() const { return m_id; }
    bool isValid() const;

    // Setters
    void setCenter(const Point& center) { m_center = center; }
    void setRadius(double radius) { m_radius = radius; }
    void setNormal(const std::vector<double>& normal);
    void setId(const std::string& id) { m_id = id; }

    // Operations
    double distanceToPoint(const Point& point) const;
    Point projectPoint(const Point& point) const;
    bool isOnPlane(const Point& point, double tolerance = 1e-6) const;

private:
    Point m_center;
    double m_radius{0.0};
    std::vector<double> m_normal;  // Normal vector of the plane containing the circle
    std::string m_id;
};

} // namespace Geometry
} // namespace DCP9
