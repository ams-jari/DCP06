#include "core/geometry/LineFitting.h"
#include "geometry/Point.h"
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <limits>
#include <sstream>

namespace DCP9 {
namespace Core {
namespace Geometry {

LineFitResult bestFitLine(const std::vector<DCP9::Geometry::Point>& points) {
    LineFitResult result;
    
    if (points.size() < 2) {
        result.isValid = false;
        return result;
    }

    // Calculate centroid
    double sumX = 0.0, sumY = 0.0, sumZ = 0.0;
    for (size_t i = 0; i < points.size(); ++i) {
        sumX += points[i].x();
        sumY += points[i].y();
        sumZ += points[i].z();
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
    
    // The direction vector is the eigenvector corresponding to the largest eigenvalue
    // (first column of V matrix, since eigenvalues are sorted in descending order)
    Eigen::Vector3d direction = svd.matrixV().col(0);
    
    // Normalize the direction vector
    double dirLength = direction.norm();
    if (dirLength < 1e-10) {
        // All points are coincident
        result.isValid = false;
        return result;
    }
    direction.normalize();

    // Create point on line (centroid)
    DCP9::Geometry::Point linePoint(centroidX, centroidY, centroidZ);
    
    // Store direction vector
    std::vector<double> directionVec(3);
    directionVec[0] = direction(0);
    directionVec[1] = direction(1);
    directionVec[2] = direction(2);

    // Calculate RMS and residuals
    std::vector<double> residuals = calculateLineResiduals(points, linePoint, directionVec);
    double rms = calculateLineRMS(points, linePoint, directionVec);

    result.point = linePoint;  // Copy assignment
    result.direction = directionVec;  // Copy assignment
    result.rms = rms;
    result.residuals = residuals;  // Copy assignment
    result.isValid = true;

    return result;
}

LineFitResult bestFitLineValidated(
    const std::vector<DCP9::Geometry::Point>& points,
    size_t minPoints) {
    
    if (points.size() < minPoints) {
        std::ostringstream oss;
        oss << "Insufficient points for line fitting. Required: " << minPoints
            << ", provided: " << points.size();
        throw std::invalid_argument(oss.str());
    }

    // Check if all points are coincident
    if (points.size() >= 2) {
        double minDist = std::numeric_limits<double>::max();
        for (size_t i = 0; i < points.size() - 1; ++i) {
            for (size_t j = i + 1; j < points.size(); ++j) {
                double dist = points[i].distanceTo(points[j]);
                if (dist < minDist) {
                    minDist = dist;
                }
            }
        }
        
        if (minDist < 1e-10) {
            throw std::invalid_argument("All points are coincident, cannot form a line");
        }
    }

    LineFitResult result = bestFitLine(points);
    
    if (!result.isValid) {
        throw std::runtime_error("Line fitting failed");
    }

    return result;
}

double distancePointToLine(
    const DCP9::Geometry::Point& point,
    const DCP9::Geometry::Point& linePoint,
    const std::vector<double>& lineDirection) {
    
    if (lineDirection.size() != 3) {
        return 0.0;
    }
    
    // Vector from line point to the point
    double dx = point.x() - linePoint.x();
    double dy = point.y() - linePoint.y();
    double dz = point.z() - linePoint.z();
    
    // Project this vector onto the line direction
    double projection = dx * lineDirection[0] + 
                       dy * lineDirection[1] + 
                       dz * lineDirection[2];
    
    // The point on the line closest to the given point
    double closestX = linePoint.x() + projection * lineDirection[0];
    double closestY = linePoint.y() + projection * lineDirection[1];
    double closestZ = linePoint.z() + projection * lineDirection[2];
    
    // Distance from point to closest point on line
    double distX = point.x() - closestX;
    double distY = point.y() - closestY;
    double distZ = point.z() - closestZ;
    
    return std::sqrt(distX * distX + distY * distY + distZ * distZ);
}

double calculateLineRMS(
    const std::vector<DCP9::Geometry::Point>& points,
    const DCP9::Geometry::Point& linePoint,
    const std::vector<double>& lineDirection) {
    
    if (points.empty()) {
        return 0.0;
    }

    double sumSquared = 0.0;
    for (size_t i = 0; i < points.size(); ++i) {
        double distance = distancePointToLine(points[i], linePoint, lineDirection);
        sumSquared += distance * distance;
    }

    return std::sqrt(sumSquared / static_cast<double>(points.size()));
}

std::vector<double> calculateLineResiduals(
    const std::vector<DCP9::Geometry::Point>& points,
    const DCP9::Geometry::Point& linePoint,
    const std::vector<double>& lineDirection) {
    
    std::vector<double> residuals;
    residuals.reserve(points.size());

    for (size_t i = 0; i < points.size(); ++i) {
        double distance = distancePointToLine(points[i], linePoint, lineDirection);
        residuals.push_back(distance);
    }

    return residuals;
}

} // namespace Geometry
} // namespace Core
} // namespace DCP9
