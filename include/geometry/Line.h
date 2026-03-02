#pragma once

#include <string>
#include <vector>
#include "geometry/Point.h"

namespace DCP9 {
namespace Geometry {

class Line {
public:
    Line();
    Line(const Point& p1, const Point& p2);
    ~Line() = default;

    // Getters
    const Point& startPoint() const { return m_startPoint; }
    const Point& endPoint() const { return m_endPoint; }
    const std::string& id() const { return m_id; }
    
    // Setters
    void setStartPoint(const Point& point) { m_startPoint = point; }
    void setEndPoint(const Point& point) { m_endPoint = point; }
    void setId(const std::string& id) { m_id = id; }
    
    // Methods
    double length() const;
    bool isValid() const;
    Point pointAt(double t) const;  // t in [0,1] for point along line
    double distanceToPoint(const Point& point) const;
    bool isParallelTo(const Line& other) const;
    
    // Projection methods
    Point projectPoint(const Point& point) const;  // Project single point onto line
    std::vector<Point> projectPoints(const std::vector<Point>& points) const;  // Project multiple points onto line
    
    // Direction vector (unit vector along line)
    std::vector<double> direction() const;

private:
    Point m_startPoint;
    Point m_endPoint;
    std::string m_id;
};

} // namespace Geometry
} // namespace DCP9 
