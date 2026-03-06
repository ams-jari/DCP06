#pragma once

#include <vector>
#include <memory>
#include "geometry/Point.h"
#include "geometry/Plane.h"
#include "geometry/Circle.h"

namespace DCP9 {
namespace Core {
namespace Geometry {

/**
 * @brief Result structure for circle fitting operations
 */
struct CircleFitResult {
    DCP9::Geometry::Circle circle;    // Fitted circle
    double rms;                       // Root mean square error
    std::vector<double> residuals;    // Residuals for each point
    bool isValid;                     // Whether the fit was successful
    
    CircleFitResult() : rms(0.0), isValid(false) {}
};

/**
 * @brief Calculates the best-fit circle through a set of points in 3D space
 * 
 * This function fits a circle to a set of 3D points using a multi-step approach:
 * 1. Fit a plane to the points
 * 2. Project points onto the plane
 * 3. Fit a 2D circle to the projected points
 * 4. Convert back to 3D coordinates
 * 
 * Algorithm:
 * - Uses SVD for plane fitting
 * - Uses least squares for 2D circle fitting
 * - Projects points onto plane before circle fitting
 * 
 * @param points Vector of points to fit circle through
 * @return CircleFitResult containing the fitted circle, RMS, and residuals
 * 
 * @note Requires at least 3 points
 * @note Points should be approximately coplanar
 */
CircleFitResult bestFitCircle(const std::vector<DCP9::Geometry::Point>& points);

/**
 * @brief Calculates the best-fit circle in a given plane
 * 
 * Fits a circle to points that are already known to lie on a specific plane.
 * This is more efficient than bestFitCircle when the plane is already known.
 * 
 * @param points Vector of points to fit circle through
 * @param plane Plane containing the points
 * @return CircleFitResult containing the fitted circle, RMS, and residuals
 * 
 * @note Requires at least 3 points
 * @note Points should lie on the given plane
 */
CircleFitResult bestFitCircleInPlane(
    const std::vector<DCP9::Geometry::Point>& points,
    const DCP9::Geometry::Plane& plane
);

/**
 * @brief Calculates the best-fit 2D circle (for points in XY plane)
 * 
 * Fits a circle to 2D points using least squares method.
 * 
 * @param points Vector of 2D points (z-coordinate ignored)
 * @return CircleFitResult containing the fitted circle, RMS, and residuals
 * 
 * @note Requires at least 3 points
 * @note Circle will be in XY plane (normal = [0,0,1])
 */
CircleFitResult bestFitCircle2D(const std::vector<DCP9::Geometry::Point>& points);

/**
 * @brief Calculates RMS (Root Mean Square) error for points relative to a circle
 * 
 * @param points Vector of points
 * @param circle Circle to calculate RMS against
 * @return RMS value
 */
double calculateCircleRMS(const std::vector<DCP9::Geometry::Point>& points, const DCP9::Geometry::Circle& circle);

/**
 * @brief Calculates residuals (distances) for points relative to a circle
 * 
 * @param points Vector of points
 * @param circle Circle to calculate residuals against
 * @return Vector of residual distances (one per point)
 */
std::vector<double> calculateCircleResiduals(
    const std::vector<DCP9::Geometry::Point>& points,
    const DCP9::Geometry::Circle& circle
);

/**
 * @brief Calculates distance from a point to a circle arc in 3D
 * 
 * @param point Point to measure distance from
 * @param circle Circle to measure distance to
 * @return Distance from point to nearest point on circle arc
 */
double distancePointToCircle(const DCP9::Geometry::Point& point, const DCP9::Geometry::Circle& circle);

} // namespace Geometry
} // namespace Core
} // namespace DCP9
