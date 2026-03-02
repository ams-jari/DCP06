#include "core/geometry/PlaneFitting.h"
#include "geometry/Plane.h"
#include "geometry/Point.h"
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <numeric>

namespace DCP9 {
namespace Core {
namespace Geometry {

PlaneFitResult bestFitPlane(const std::vector<DCP9::Geometry::Point>& points) {
    if (points.size() < 3) {
        PlaneFitResult result;
        result.isValid = false;
        return result;
    }

    // Calculate centroid
    double sumX = 0.0, sumY = 0.0, sumZ = 0.0;
    for (const auto& point : points) {
        sumX += point.x();
        sumY += point.y();
        sumZ += point.z();
    }
    
    const size_t n = points.size();
    const double centroidX = sumX / n;
    const double centroidY = sumY / n;
    const double centroidZ = sumZ / n;

    // Center points about centroid and build covariance matrix
    Eigen::MatrixXd centered(n, 3);
    for (size_t i = 0; i < n; ++i) {
        centered(i, 0) = points[i].x() - centroidX;
        centered(i, 1) = points[i].y() - centroidY;
        centered(i, 2) = points[i].z() - centroidZ;
    }

    // Compute covariance matrix: C = (1/n) * X^T * X
    Eigen::Matrix3d covariance = (centered.transpose() * centered) / static_cast<double>(n);

    // Perform SVD to find eigenvectors
    Eigen::JacobiSVD<Eigen::Matrix3d> svd(covariance, Eigen::ComputeFullV);
    
    // The normal vector is the eigenvector corresponding to the smallest eigenvalue
    // (last column of V matrix)
    Eigen::Vector3d normal = svd.matrixV().col(2);
    
    // Ensure normal points in positive direction (convention)
    if (normal(2) < 0) {
        normal = -normal;
    }

    // Create plane using centroid as point and normal vector
    DCP9::Geometry::Point centroidPoint(centroidX, centroidY, centroidZ);
    
    // Create two additional points on the plane to construct it
    // Use two vectors perpendicular to the normal
    Eigen::Vector3d u, v;
    if (std::abs(normal(0)) < 0.9) {
        u = Eigen::Vector3d(1, 0, 0).cross(normal).normalized();
    } else {
        u = Eigen::Vector3d(0, 1, 0).cross(normal).normalized();
    }
    v = normal.cross(u).normalized();
    
    // Create two points on the plane
    DCP9::Geometry::Point p2(centroidX + u(0), centroidY + u(1), centroidZ + u(2));
    DCP9::Geometry::Point p3(centroidX + v(0), centroidY + v(1), centroidZ + v(2));
    
    DCP9::Geometry::Plane plane(centroidPoint, p2, p3);
    
    // Verify and set the correct normal (plane constructor may flip it)
    std::vector<double> currentNormal = plane.normal();
    double dot = currentNormal[0] * normal(0) + 
                 currentNormal[1] * normal(1) + 
                 currentNormal[2] * normal(2);
    
    // If normals are opposite, flip the plane's normal
    if (dot < 0) {
        std::vector<double> flippedNormal = {-normal(0), -normal(1), -normal(2)};
        plane.setNormal(flippedNormal);
    } else {
        std::vector<double> normalVec = {normal(0), normal(1), normal(2)};
        plane.setNormal(normalVec);
    }

    // Calculate RMS and residuals
    std::vector<double> residuals = calculatePlaneResiduals(points, plane);
    double rms = calculatePlaneRMS(points, plane);

    PlaneFitResult result;
    result.plane = plane;  // Copy assignment
    result.rms = rms;
    result.residuals = residuals;  // Copy assignment
    result.isValid = true;

    return result;
}

PlaneFitResult bestFitPlaneValidated(
    const std::vector<DCP9::Geometry::Point>& points,
    size_t minPoints) {
    
    if (points.size() < minPoints) {
        throw std::invalid_argument(
            "Insufficient points for plane fitting. Required: " + 
            std::to_string(minPoints) + ", provided: " + 
            std::to_string(points.size())
        );
    }

    // Check for collinearity (all points on a line)
    if (points.size() == 3) {
        // For 3 points, check if they form a valid triangle
        DCP9::Geometry::Point v1(points[1].x() - points[0].x(),
                 points[1].y() - points[0].y(),
                 points[1].z() - points[0].z());
        DCP9::Geometry::Point v2(points[2].x() - points[0].x(),
                 points[2].y() - points[0].y(),
                 points[2].z() - points[0].z());
        
        // Cross product magnitude should be non-zero
        double crossX = v1.y() * v2.z() - v1.z() * v2.y();
        double crossY = v1.z() * v2.x() - v1.x() * v2.z();
        double crossZ = v1.x() * v2.y() - v1.y() * v2.x();
        double crossMag = std::sqrt(crossX * crossX + crossY * crossY + crossZ * crossZ);
        
        if (crossMag < 1e-10) {
            throw std::invalid_argument("Points are collinear, cannot form a plane");
        }
    }

    PlaneFitResult result = bestFitPlane(points);
    
    if (!result.isValid) {
        throw std::runtime_error("Plane fitting failed");
    }

    return result;
}

double calculatePlaneRMS(const std::vector<DCP9::Geometry::Point>& points, const DCP9::Geometry::Plane& plane) {
    if (points.empty()) {
        return 0.0;
    }

    double sumSquared = 0.0;
    for (const auto& point : points) {
        double distance = plane.distanceToPoint(point);
        sumSquared += distance * distance;
    }

    return std::sqrt(sumSquared / static_cast<double>(points.size()));
}

std::vector<double> calculatePlaneResiduals(
    const std::vector<DCP9::Geometry::Point>& points,
    const DCP9::Geometry::Plane& plane) {
    
    std::vector<double> residuals;
    residuals.reserve(points.size());

    for (const auto& point : points) {
        double distance = plane.distanceToPoint(point);
        residuals.push_back(distance);
    }

    return residuals;
}

} // namespace Geometry
} // namespace Core
} // namespace DCP9

