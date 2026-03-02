#include "core/geometry/CircleFitting.h"
#include "core/geometry/PlaneFitting.h"
#include "geometry/Circle.h"
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <cmath>
#include <algorithm>

namespace DCP9 {
namespace Core {
namespace Geometry {

CircleFitResult bestFitCircle(const std::vector<DCP9::Geometry::Point>& points) {
    CircleFitResult result;
    
    // Need at least 3 points for circle fitting
    if (points.size() < 3) {
        result.isValid = false;
        return result;
    }
    
    // Step 1: Fit plane to points
    PlaneFitResult planeResult = bestFitPlane(points);
    if (!planeResult.isValid) {
        result.isValid = false;
        return result;
    }
    
    // Step 2: Project points onto plane and fit 2D circle
    return bestFitCircleInPlane(points, planeResult.plane);
}

CircleFitResult bestFitCircleInPlane(
    const std::vector<DCP9::Geometry::Point>& points,
    const DCP9::Geometry::Plane& plane)
{
    CircleFitResult result;
    
    if (points.size() < 3) {
        result.isValid = false;
        return result;
    }
    
    // Get plane normal and point
    const std::vector<double>& normal = plane.normal();
    const DCP9::Geometry::Point& planePoint = plane.point();
    
    // Step 1: Project all points onto the plane
    std::vector<DCP9::Geometry::Point> projectedPoints;
    projectedPoints.reserve(points.size());
    
    for (const auto& point : points) {
        projectedPoints.push_back(plane.projectPoint(point));
    }
    
    // Step 2: Create 2D coordinate system on the plane
    // Find two orthogonal vectors in the plane
    Eigen::Vector3d n(normal[0], normal[1], normal[2]);
    Eigen::Vector3d u, v;
    
    // Choose u as a vector perpendicular to normal
    if (std::abs(n(0)) < 0.9) {
        u = Eigen::Vector3d(1.0 - n(0)*n(0), -n(0)*n(1), -n(0)*n(2));
    } else {
        u = Eigen::Vector3d(-n(1)*n(0), 1.0 - n(1)*n(1), -n(1)*n(2));
    }
    u.normalize();
    
    // v = n × u (perpendicular to both)
    v = n.cross(u);
    v.normalize();
    
    // Step 3: Convert 3D points to 2D coordinates in plane
    std::vector<Eigen::Vector2d> points2D;
    points2D.reserve(projectedPoints.size());
    
    for (const auto& pt : projectedPoints) {
        Eigen::Vector3d vec(
            pt.x() - planePoint.x(),
            pt.y() - planePoint.y(),
            pt.z() - planePoint.z()
        );
        
        double uCoord = vec.dot(u);
        double vCoord = vec.dot(v);
        points2D.push_back(Eigen::Vector2d(uCoord, vCoord));
    }
    
    // Step 4: Fit 2D circle using least squares
    // Solve: A * [cx, cy, c] = b
    // where: A[i] = [2*x[i], 2*y[i], 1]
    //        b[i] = x[i]^2 + y[i]^2
    //        radius = sqrt(c + cx^2 + cy^2)
    
    size_t nPoints = points2D.size();
    Eigen::MatrixXd A(nPoints, 3);
    Eigen::VectorXd b(nPoints);
    
    for (size_t i = 0; i < nPoints; ++i) {
        double x = points2D[i](0);
        double y = points2D[i](1);
        
        A(i, 0) = 2.0 * x;
        A(i, 1) = 2.0 * y;
        A(i, 2) = 1.0;
        b(i) = x * x + y * y;
    }
    
    // Solve using least squares (SVD)
    Eigen::VectorXd params = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);
    
    double cx2D = params(0);
    double cy2D = params(1);
    double c = params(2);
    
    // Calculate radius
    double radius = std::sqrt(c + cx2D * cx2D + cy2D * cy2D);
    
    if (radius <= 0.0 || !std::isfinite(radius)) {
        result.isValid = false;
        return result;
    }
    
    // Step 5: Convert 2D center back to 3D
    Eigen::Vector3d center3D = Eigen::Vector3d(planePoint.x(), planePoint.y(), planePoint.z()) +
                                cx2D * u + cy2D * v;
    
    DCP9::Geometry::Point center(center3D(0), center3D(1), center3D(2));
    
    // Create circle
    DCP9::Geometry::Circle circle(center, radius, normal);
    
    // Calculate RMS and residuals
    double rms = calculateCircleRMS(points, circle);
    std::vector<double> residuals = calculateCircleResiduals(points, circle);
    
    result.circle = circle;
    result.rms = rms;
    result.residuals = residuals;
    result.isValid = true;
    
    return result;
}

CircleFitResult bestFitCircle2D(const std::vector<DCP9::Geometry::Point>& points) {
    CircleFitResult result;
    
    if (points.size() < 3) {
        result.isValid = false;
        return result;
    }
    
    // Fit 2D circle in XY plane
    size_t nPoints = points.size();
    Eigen::MatrixXd A(nPoints, 3);
    Eigen::VectorXd b(nPoints);
    
    for (size_t i = 0; i < nPoints; ++i) {
        double x = points[i].x();
        double y = points[i].y();
        
        A(i, 0) = 2.0 * x;
        A(i, 1) = 2.0 * y;
        A(i, 2) = 1.0;
        b(i) = x * x + y * y;
    }
    
    // Solve using least squares (SVD)
    Eigen::VectorXd params = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);
    
    double cx = params(0);
    double cy = params(1);
    double c = params(2);
    
    // Calculate radius
    double radius = std::sqrt(c + cx * cx + cy * cy);
    
    if (radius <= 0.0 || !std::isfinite(radius)) {
        result.isValid = false;
        return result;
    }
    
    // Create circle in XY plane (normal = [0, 0, 1])
    DCP9::Geometry::Point center(cx, cy, 0.0);
    std::vector<double> normal = {0.0, 0.0, 1.0};
    DCP9::Geometry::Circle circle(center, radius, normal);
    
    // Calculate RMS and residuals
    double rms = calculateCircleRMS(points, circle);
    std::vector<double> residuals = calculateCircleResiduals(points, circle);
    
    result.circle = circle;
    result.rms = rms;
    result.residuals = residuals;
    result.isValid = true;
    
    return result;
}

double calculateCircleRMS(const std::vector<DCP9::Geometry::Point>& points, const DCP9::Geometry::Circle& circle) {
    if (points.empty()) {
        return 0.0;
    }
    
    double sumSquares = 0.0;
    for (const auto& point : points) {
        double distance = distancePointToCircle(point, circle);
        sumSquares += distance * distance;
    }
    
    return std::sqrt(sumSquares / points.size());
}

std::vector<double> calculateCircleResiduals(
    const std::vector<DCP9::Geometry::Point>& points,
    const DCP9::Geometry::Circle& circle)
{
    std::vector<double> residuals;
    residuals.reserve(points.size());
    
    for (const auto& point : points) {
        residuals.push_back(distancePointToCircle(point, circle));
    }
    
    return residuals;
}

double distancePointToCircle(const DCP9::Geometry::Point& point, const DCP9::Geometry::Circle& circle) {
    return circle.distanceToPoint(point);
}

} // namespace Geometry
} // namespace Core
} // namespace DCP9

