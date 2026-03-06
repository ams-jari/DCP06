#pragma once

#include <vector>
#include <memory>
#include <Eigen/Dense>
#include "geometry/Point.h"

namespace DCP9 {
namespace Core {
namespace Geometry {

/**
 * @brief Result structure for rigid transformation operations
 */
struct RigidTransformResult {
    Eigen::Matrix3d rotation;           // 3x3 rotation matrix
    Eigen::Vector3d translation;       // 3x1 translation vector
    Eigen::Matrix4d transformMatrix;   // 4x4 homogeneous transformation matrix
    double rms;                       // Root mean square error
    std::vector<double> residuals;     // Residuals for each point
    std::vector<size_t> sourceIndices; // Indices of source points used
    std::vector<size_t> targetIndices; // Indices of target points used
    bool isValid;                     // Whether the transformation was successful
    
    RigidTransformResult() : rms(0.0), isValid(false) {}
};

/**
 * @brief Solves the rigid 3D transformation between two corresponding point sets
 * 
 * This function finds the optimal rotation and translation between two sets of 
 * corresponding 3D points using the Kabsch algorithm (SVD-based method).
 * 
 * Algorithm:
 * 1. Calculate centroids of both point sets
 * 2. Center points about their centroids
 * 3. Optionally find point correspondences by distance from origin
 * 4. Compute covariance matrix H = A^T * B
 * 5. Perform SVD: H = U * S * V^T
 * 6. Calculate rotation: R = V * U^T (with reflection handling)
 * 7. Calculate translation: t = -R * centroid_A + centroid_B
 * 
 * This is essential for:
 * - Change station routine (transforming measurements after tracker relocation)
 * - SCS to OCS transformation (Sensor Coordinate System to Object Coordinate System)
 * - DOM/POM/COM alignment calculations (Direct/Point/Cylinder Orientation Methods)
 * 
 * @param sourcePoints Source point set (e.g., measured points in sensor CS)
 * @param targetPoints Target point set (e.g., design points in object CS)
 * @param sortPoints If true, automatically find point correspondences by distance
 * @return RigidTransformResult containing rotation, translation, RMS, and residuals
 * 
 * @note Requires at least 3 non-collinear points
 * @note Points should be corresponding (same order or use sortPoints=true)
 * @note Reference: http://nghiaho.com/?page_id=671
 */
RigidTransformResult solveRigidTransform(
    const std::vector<DCP9::Geometry::Point>& sourcePoints,
    const std::vector<DCP9::Geometry::Point>& targetPoints,
    bool sortPoints = true
);

/**
 * @brief Transforms points using a rigid transformation
 * 
 * Applies rotation and translation to transform points from one coordinate 
 * system to another.
 * 
 * Forward transformation: p' = R * p + t
 * Inverse transformation: p' = R^T * (p - t)
 * 
 * @param points Points to transform
 * @param rotation 3x3 rotation matrix
 * @param translation 3x1 translation vector
 * @param inverse If true, applies inverse transformation
 * @return Vector of transformed points
 */
std::vector<DCP9::Geometry::Point> transformPoints(
    const std::vector<DCP9::Geometry::Point>& points,
    const Eigen::Matrix3d& rotation,
    const Eigen::Vector3d& translation,
    bool inverse = false
);

/**
 * @brief Transforms a single point using a rigid transformation
 * 
 * @param point Point to transform
 * @param rotation 3x3 rotation matrix
 * @param translation 3x1 translation vector
 * @param inverse If true, applies inverse transformation
 * @return Transformed point
 */
DCP9::Geometry::Point transformPoint(
    const DCP9::Geometry::Point& point,
    const Eigen::Matrix3d& rotation,
    const Eigen::Vector3d& translation,
    bool inverse = false
);

/**
 * @brief Transforms points using a 4x4 homogeneous transformation matrix
 * 
 * @param points Points to transform
 * @param transformMatrix 4x4 homogeneous transformation matrix
 * @return Vector of transformed points
 */
std::vector<DCP9::Geometry::Point> transformPoints(
    const std::vector<DCP9::Geometry::Point>& points,
    const Eigen::Matrix4d& transformMatrix
);

/**
 * @brief Transforms a single point using a 4x4 homogeneous transformation matrix
 * 
 * @param point Point to transform
 * @param transformMatrix 4x4 homogeneous transformation matrix
 * @return Transformed point
 */
DCP9::Geometry::Point transformPoint(
    const DCP9::Geometry::Point& point,
    const Eigen::Matrix4d& transformMatrix
);

/**
 * @brief Creates a rotation matrix from axis-angle representation (Rodrigues formula)
 * 
 * Creates a rotation matrix for counterclockwise rotation about the given axis
 * by theta radians. Uses quaternion-based approach for numerical stability.
 * 
 * @param axis Rotation axis (normalized automatically)
 * @param theta Rotation angle in radians
 * @return 3x3 rotation matrix
 */
Eigen::Matrix3d rotationMatrixFromAxisAngle(
    const Eigen::Vector3d& axis,
    double theta
);

/**
 * @brief Finds axis and angle to align two vectors
 * 
 * Calculates the axis and angle needed to rotate oldDir to align with newDir.
 * 
 * @param oldDir Vector to be aligned
 * @param newDir Target direction vector
 * @return Pair of (axis, angle) where axis is normalized and angle is in radians
 */
std::pair<Eigen::Vector3d, double> axisAndAngle(
    const Eigen::Vector3d& oldDir,
    const Eigen::Vector3d& newDir
);

/**
 * @brief Rotates points about an axis by a given angle
 * 
 * @param points Points to rotate
 * @param axis Rotation axis (normalized automatically)
 * @param theta Rotation angle in radians
 * @return Vector of rotated points
 */
std::vector<DCP9::Geometry::Point> rotateAboutAxis(
    const std::vector<DCP9::Geometry::Point>& points,
    const Eigen::Vector3d& axis,
    double theta
);

/**
 * @brief Rotates a single point about an axis by a given angle
 * 
 * @param point Point to rotate
 * @param axis Rotation axis (normalized automatically)
 * @param theta Rotation angle in radians
 * @return Rotated point
 */
DCP9::Geometry::Point rotateAboutAxis(
    const DCP9::Geometry::Point& point,
    const Eigen::Vector3d& axis,
    double theta
);

/**
 * @brief Calculates centroid (center of mass) of a point set
 * 
 * @param points Point set
 * @return Centroid point
 */
DCP9::Geometry::Point calculateCentroid(const std::vector<DCP9::Geometry::Point>& points);

/**
 * @brief Centers points about a given point
 * 
 * Subtracts the center point from all points to center the point set.
 * 
 * @param points Points to center
 * @param center Center point
 * @return Vector of centered points
 */
std::vector<DCP9::Geometry::Point> centerPoints(
    const std::vector<DCP9::Geometry::Point>& points,
    const DCP9::Geometry::Point& center
);

/**
 * @brief Finds point correspondences between two point sets by distance from origin
 * 
 * Matches points by sorting them by distance from origin. This is useful when
 * point sets are in different coordinate systems but have similar relative positions.
 * 
 * @param sourcePoints Source point set
 * @param targetPoints Target point set
 * @return Pair of index vectors (sourceIndices, targetIndices) for matching
 */
std::pair<std::vector<size_t>, std::vector<size_t>> findCorrespondingPoints(
    const std::vector<DCP9::Geometry::Point>& sourcePoints,
    const std::vector<DCP9::Geometry::Point>& targetPoints
);

/**
 * @brief Calculates RMS error for transformed points
 * 
 * @param sourcePoints Source points
 * @param targetPoints Target points
 * @param rotation Rotation matrix
 * @param translation Translation vector
 * @return RMS value
 */
double calculateTransformRMS(
    const std::vector<DCP9::Geometry::Point>& sourcePoints,
    const std::vector<DCP9::Geometry::Point>& targetPoints,
    const Eigen::Matrix3d& rotation,
    const Eigen::Vector3d& translation
);

/**
 * @brief Calculates residuals for transformed points
 * 
 * @param sourcePoints Source points
 * @param targetPoints Target points
 * @param rotation Rotation matrix
 * @param translation Translation vector
 * @return Vector of residual distances (one per point)
 */
std::vector<double> calculateTransformResiduals(
    const std::vector<DCP9::Geometry::Point>& sourcePoints,
    const std::vector<DCP9::Geometry::Point>& targetPoints,
    const Eigen::Matrix3d& rotation,
    const Eigen::Vector3d& translation
);

} // namespace Geometry
} // namespace Core
} // namespace DCP9
