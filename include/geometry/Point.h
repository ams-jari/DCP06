#pragma once

#include <string>
#include <memory>

namespace DCP9 {
namespace Geometry {

class Point {
public:
    Point();
    Point(double x, double y, double z);
    ~Point();

    // Getters
    double x() const { return m_x; }
    double y() const { return m_y; }
    double z() const { return m_z; }
    const std::string& id() const { return m_id; }
    double precision() const { return m_precision; }
    double deviation() const { return m_deviation; }
    const std::string& status() const { return m_status; }

    // Setters
    void setX(double x) { m_x = x; }
    void setY(double y) { m_y = y; }
    void setZ(double z) { m_z = z; }
    void setId(const std::string& id) { m_id = id; }
    void setPrecision(double precision) { m_precision = precision; }
    void setDeviation(double deviation) { m_deviation = deviation; }
    void setStatus(const std::string& status) { m_status = status; }

    // Operations
    double distanceTo(const Point& other) const;
    bool isValid() const;

private:
    double m_x;
    double m_y;
    double m_z;
    std::string m_id;
    double m_precision;
    double m_deviation;
    std::string m_status;
};

} // namespace Geometry
} // namespace DCP9 
