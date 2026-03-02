#pragma once

#include <vector>
#include <memory>
#include "geometry/Point.h"
#include "geometry/Plane.h"

namespace DCP9 {
namespace Core {
namespace Geometry {

/**
 * @brief Result structure for plane fitting operations
 */
struct PlaneFitResult {
    DCP9::Geometry::Plane plane;    // Fitted plane
    double rms{0.0};                // Root mean square error
    std::vector<double> residuals;   // Residuals for each point
    bool isValid{false};            // Whether the fit was successful
    
    PlaneFitResult() = default;
};

/**
 * @brief Calculates the best-fit plane through a set of points using SVD
 * 
 * This function uses Singular Value Decomposition (SVD) of the covariance matrix
 * to find the plane that minimizes the sum of squared distances from all points.
 * 
 * Algorithm:
 * 1. Calculate centroid of all points
 * 2. Center points about centroid
 * 3. Compute covariance matrix
 * 4. Find eigenvector with smallest eigenvalue (normal vector)
 * 5. Construct plane equation
 * 
 * @param points Vector of points to fit plane through
 * @return PlaneFitResult containing the fitted plane, RMS, and residuals
 * 
 * @note Requires at least 3 non-collinear points
 * @note Points should use measured coordinates (x_mea, y_mea, z_mea)
 */
PlaneFitResult bestFitPlane(const std::vector<DCP9::Geometry::Point>& points);

/**
 * @brief Calculates the best-fit plane through a set of points with validation
 * 
 * Same as bestFitPlane but includes additional validation and error checking.
 * 
 * @param points Vector of points to fit plane through
 * @param minPoints Minimum number of points required (default: 3)
 * @return PlaneFitResult containing the fitted plane, RMS, and residuals
 * 
 * @throws std::invalid_argument if insufficient points or points are collinear
 */
PlaneFitResult bestFitPlaneValidated(
    const std::vector<DCP9::Geometry::Point>& points,
    size_t minPoints = 3
);

/**
 * @brief Calculates RMS (Root Mean Square) error for points relative to a plane
 * 
 * @param points Vector of points
 * @param plane Plane to calculate RMS against
 * @return RMS value
 */
double calculatePlaneRMS(const std::vector<DCP9::Geometry::Point>& points, const DCP9::Geometry::Plane& plane);

/**
 * @brief Calculates residuals (distances) for points relative to a plane
 * 
 * @param points Vector of points
 * @param plane Plane to calculate residuals against
 * @return Vector of residual distances (one per point)
 */
std::vector<double> calculatePlaneResiduals(
    const std::vector<DCP9::Geometry::Point>& points,
    const DCP9::Geometry::Plane& plane
);

} // namespace Geometry
} // namespace Core
} // namespace DCP9
