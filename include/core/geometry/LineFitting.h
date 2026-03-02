#pragma once

#include <vector>
#include <memory>
#include "geometry/Point.h"

namespace DCP9 {
namespace Core {
namespace Geometry {

/**
 * @brief Result structure for line fitting operations
 */
struct LineFitResult {
    DCP9::Geometry::Point point;      ///< A point on the fitted line (typically the centroid)
    std::vector<double> direction;    ///< Direction vector of the line (unit vector)
    double rms{0.0};                   ///< Root mean square error of the fit
    std::vector<double> residuals;     ///< Residual distances of points to the line
    bool isValid{false};               ///< Whether the fitting was successful
    
    LineFitResult() = default;
};

/**
 * @brief Calculates the best-fit line through a set of points using Principal Component Analysis (PCA)
 * 
 * This function uses Singular Value Decomposition (SVD) of the covariance matrix
 * to find the line that minimizes the sum of squared distances from all points.
 * 
 * Algorithm:
 * 1. Calculate centroid of all points
 * 2. Center points about centroid
 * 3. Compute covariance matrix
 * 4. Find eigenvector with largest eigenvalue (direction vector)
 * 5. Construct line equation
 * 
 * @param points Vector of points to fit line through
 * @return LineFitResult containing the fitted line, RMS, and residuals
 * 
 * @note Requires at least 2 points
 * @note Points should use measured coordinates (x_mea, y_mea, z_mea)
 */
LineFitResult bestFitLine(const std::vector<DCP9::Geometry::Point>& points);

/**
 * @brief Calculates the best-fit line through a set of points with validation
 * 
 * Same as bestFitLine but includes additional validation and error checking.
 * 
 * @param points Vector of points to fit line through
 * @param minPoints Minimum number of points required (default: 2)
 * @return LineFitResult containing the fitted line, RMS, and residuals
 * 
 * @throws std::invalid_argument if insufficient points or all points are coincident
 */
LineFitResult bestFitLineValidated(
    const std::vector<DCP9::Geometry::Point>& points,
    size_t minPoints = 2
);

/**
 * @brief Calculates RMS (Root Mean Square) error for points relative to a line
 * 
 * @param points Vector of points
 * @param linePoint A point on the line
 * @param lineDirection Direction vector of the line (unit vector)
 * @return RMS value
 */
double calculateLineRMS(
    const std::vector<DCP9::Geometry::Point>& points,
    const DCP9::Geometry::Point& linePoint,
    const std::vector<double>& lineDirection
);

/**
 * @brief Calculates residuals (distances) for points relative to a line
 * 
 * @param points Vector of points
 * @param linePoint A point on the line
 * @param lineDirection Direction vector of the line (unit vector)
 * @return Vector of residual distances (one per point)
 */
std::vector<double> calculateLineResiduals(
    const std::vector<DCP9::Geometry::Point>& points,
    const DCP9::Geometry::Point& linePoint,
    const std::vector<double>& lineDirection
);

/**
 * @brief Calculates the distance from a point to a line
 * 
 * @param point The point
 * @param linePoint A point on the line
 * @param lineDirection Direction vector of the line (unit vector)
 * @return Distance from point to line
 */
double distancePointToLine(
    const DCP9::Geometry::Point& point,
    const DCP9::Geometry::Point& linePoint,
    const std::vector<double>& lineDirection
);

} // namespace Geometry
} // namespace Core
} // namespace DCP9
