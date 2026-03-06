#include "core/geometry/Transformations.h"
#include "geometry/Point.h"
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <cmath>
#include <algorithm>
#include <numeric>

namespace DCP9 {
namespace Core {
namespace Geometry {

DCP9::Geometry::Point calculateCentroid(const std::vector<DCP9::Geometry::Point>& points) {
    if (points.empty()) {
        return DCP9::Geometry::Point(0.0, 0.0, 0.0);
    }
    
    double sumX = 0.0, sumY = 0.0, sumZ = 0.0;
    for (size_t i = 0; i < points.size(); ++i) {
        sumX += points[i].x();
        sumY += points[i].y();
        sumZ += points[i].z();
    }
    
    const size_t n = points.size();
    return DCP9::Geometry::Point(sumX / n, sumY / n, sumZ / n);
}

std::vector<DCP9::Geometry::Point> centerPoints(
    const std::vector<DCP9::Geometry::Point>& points,
    const DCP9::Geometry::Point& center)
{
    std::vector<DCP9::Geometry::Point> centered;
    centered.reserve(points.size());
    
    for (size_t i = 0; i < points.size(); ++i) {
        const DCP9::Geometry::Point& point = points[i];
        centered.push_back(DCP9::Geometry::Point(
            point.x() - center.x(),
            point.y() - center.y(),
            point.z() - center.z()
        ));
    }
    
    return centered;
}

std::pair<std::vector<size_t>, std::vector<size_t>> findCorrespondingPoints(
    const std::vector<DCP9::Geometry::Point>& sourcePoints,
    const std::vector<DCP9::Geometry::Point>& targetPoints)
{
    // Calculate distances from origin for each point set
    std::vector<std::pair<double, size_t>> sourceDistances;
    std::vector<std::pair<double, size_t>> targetDistances;
    
    for (size_t i = 0; i < sourcePoints.size(); ++i) {
        const DCP9::Geometry::Point& p = sourcePoints[i];
        double dist = std::sqrt(p.x() * p.x() + p.y() * p.y() + p.z() * p.z());
        sourceDistances.push_back(std::make_pair(dist, i));
    }
    
    for (size_t i = 0; i < targetPoints.size(); ++i) {
        const DCP9::Geometry::Point& p = targetPoints[i];
        double dist = std::sqrt(p.x() * p.x() + p.y() * p.y() + p.z() * p.z());
        targetDistances.push_back(std::make_pair(dist, i));
    }
    
    // Sort by distance
    struct PairLess {
        bool operator()(const std::pair<double, size_t>& a, const std::pair<double, size_t>& b) const {
            return a.first < b.first;
        }
    };
    std::sort(sourceDistances.begin(), sourceDistances.end(), PairLess());
    std::sort(targetDistances.begin(), targetDistances.end(), PairLess());
    
    // Extract indices
    std::vector<size_t> sourceIndices;
    std::vector<size_t> targetIndices;
    
    size_t minSize = std::min(sourceDistances.size(), targetDistances.size());
    for (size_t i = 0; i < minSize; ++i) {
        sourceIndices.push_back(sourceDistances[i].second);
        targetIndices.push_back(targetDistances[i].second);
    }
    
    return std::make_pair(sourceIndices, targetIndices);
}

RigidTransformResult solveRigidTransform(
    const std::vector<DCP9::Geometry::Point>& sourcePoints,
    const std::vector<DCP9::Geometry::Point>& targetPoints,
    bool sortPoints)
{
    RigidTransformResult result;
    
    // Need at least 3 points
    if (sourcePoints.size() < 3 || targetPoints.size() < 3) {
        result.isValid = false;
        return result;
    }
    
    if (sourcePoints.size() != targetPoints.size()) {
        result.isValid = false;
        return result;
    }
    
    // Step 1: Calculate centroids
    DCP9::Geometry::Point centroidA = calculateCentroid(sourcePoints);
    DCP9::Geometry::Point centroidB = calculateCentroid(targetPoints);
    
    // Step 2: Center points about centroids
    std::vector<DCP9::Geometry::Point> centeredA = centerPoints(sourcePoints, centroidA);
    std::vector<DCP9::Geometry::Point> centeredB = centerPoints(targetPoints, centroidB);
    
    // Step 3: Find point correspondences if needed
    std::vector<size_t> indicesA, indicesB;
    if (sortPoints) {
        std::pair<std::vector<size_t>, std::vector<size_t> > correspondence = findCorrespondingPoints(centeredA, centeredB);
        indicesA = correspondence.first;
        indicesB = correspondence.second;
    } else {
        // Use points in order
        indicesA.reserve(centeredA.size());
        indicesB.reserve(centeredB.size());
        for (size_t i = 0; i < centeredA.size(); ++i) {
            indicesA.push_back(i);
            indicesB.push_back(i);
        }
    }
    
    // Step 4: Build matrices for matched points
    size_t n = std::min(indicesA.size(), indicesB.size());
    Eigen::MatrixXd A(n, 3);
    Eigen::MatrixXd B(n, 3);
    
    for (size_t i = 0; i < n; ++i) {
        const DCP9::Geometry::Point& pa = centeredA[indicesA[i]];
        const DCP9::Geometry::Point& pb = centeredB[indicesB[i]];
        
        A(i, 0) = pa.x();
        A(i, 1) = pa.y();
        A(i, 2) = pa.z();
        
        B(i, 0) = pb.x();
        B(i, 1) = pb.y();
        B(i, 2) = pb.z();
    }
    
    // Step 5: Compute covariance matrix H = A^T * B
    Eigen::Matrix3d H = A.transpose() * B;
    
    // Step 6: Perform SVD: H = U * S * V^T
    Eigen::JacobiSVD<Eigen::Matrix3d> svd(H, Eigen::ComputeFullU | Eigen::ComputeFullV);
    Eigen::Matrix3d U = svd.matrixU();
    Eigen::Matrix3d V = svd.matrixV();
    
    // Step 7: Calculate rotation: R = V * U^T
    Eigen::Matrix3d R = V * U.transpose();
    
    // Step 8: Handle reflection case (det(R) < 0)
    if (R.determinant() < 0) {
        // Reflection detected - flip the last column of V
        V.col(2) *= -1.0;
        R = V * U.transpose();
    }
    
    // Step 9: Calculate translation: t = -R * centroid_A + centroid_B
    Eigen::Vector3d centroidAVec(centroidA.x(), centroidA.y(), centroidA.z());
    Eigen::Vector3d centroidBVec(centroidB.x(), centroidB.y(), centroidB.z());
    Eigen::Vector3d t = -R * centroidAVec + centroidBVec;
    
    // Store results
    result.rotation = R;
    result.translation = t;
    
    // Build 4x4 homogeneous transformation matrix
    result.transformMatrix = Eigen::Matrix4d::Identity();
    result.transformMatrix.block<3, 3>(0, 0) = R;
    result.transformMatrix.block<3, 1>(0, 3) = t;
    
    // Store indices
    result.sourceIndices = indicesA;
    result.targetIndices = indicesB;
    
    // Calculate RMS and residuals
    result.rms = calculateTransformRMS(sourcePoints, targetPoints, R, t);
    result.residuals = calculateTransformResiduals(sourcePoints, targetPoints, R, t);
    
    result.isValid = true;
    
    return result;
}

std::vector<DCP9::Geometry::Point> transformPoints(
    const std::vector<DCP9::Geometry::Point>& points,
    const Eigen::Matrix3d& rotation,
    const Eigen::Vector3d& translation,
    bool inverse)
{
    std::vector<DCP9::Geometry::Point> transformed;
    transformed.reserve(points.size());
    
    Eigen::Matrix3d R = rotation;
    Eigen::Vector3d t = translation;
    
    if (inverse) {
        R = rotation.transpose();
        t = -R * translation;
    }
    
    for (size_t i = 0; i < points.size(); ++i) {
        const DCP9::Geometry::Point& point = points[i];
        Eigen::Vector3d p(point.x(), point.y(), point.z());
        Eigen::Vector3d pTransformed = R * p + t;
        transformed.push_back(DCP9::Geometry::Point(
            pTransformed(0), pTransformed(1), pTransformed(2)
        ));
    }
    
    return transformed;
}

DCP9::Geometry::Point transformPoint(
    const DCP9::Geometry::Point& point,
    const Eigen::Matrix3d& rotation,
    const Eigen::Vector3d& translation,
    bool inverse)
{
    Eigen::Matrix3d R = rotation;
    Eigen::Vector3d t = translation;
    
    if (inverse) {
        R = rotation.transpose();
        t = -R * translation;
    }
    
    Eigen::Vector3d p(point.x(), point.y(), point.z());
    Eigen::Vector3d pTransformed = R * p + t;
    
    return DCP9::Geometry::Point(
        pTransformed(0), pTransformed(1), pTransformed(2)
    );
}

std::vector<DCP9::Geometry::Point> transformPoints(
    const std::vector<DCP9::Geometry::Point>& points,
    const Eigen::Matrix4d& transformMatrix)
{
    std::vector<DCP9::Geometry::Point> transformed;
    transformed.reserve(points.size());
    
    for (size_t i = 0; i < points.size(); ++i) {
        const DCP9::Geometry::Point& point = points[i];
        Eigen::Vector4d p(point.x(), point.y(), point.z(), 1.0);
        Eigen::Vector4d pTransformed = transformMatrix * p;
        transformed.push_back(DCP9::Geometry::Point(
            pTransformed(0), pTransformed(1), pTransformed(2)
        ));
    }
    
    return transformed;
}

DCP9::Geometry::Point transformPoint(
    const DCP9::Geometry::Point& point,
    const Eigen::Matrix4d& transformMatrix)
{
    Eigen::Vector4d p(point.x(), point.y(), point.z(), 1.0);
    Eigen::Vector4d pTransformed = transformMatrix * p;
    
    return DCP9::Geometry::Point(
        pTransformed(0), pTransformed(1), pTransformed(2)
    );
}

Eigen::Matrix3d rotationMatrixFromAxisAngle(
    const Eigen::Vector3d& axis,
    double theta)
{
    // Normalize axis
    Eigen::Vector3d normalizedAxis = axis.normalized();
    
    // Use quaternion-based approach (Rodrigues formula via quaternion)
    double cosHalfTheta = std::cos(theta / 2.0);
    double sinHalfTheta = std::sin(theta / 2.0);
    
    double a = cosHalfTheta;
    double b = -normalizedAxis(0) * sinHalfTheta;
    double c = -normalizedAxis(1) * sinHalfTheta;
    double d = -normalizedAxis(2) * sinHalfTheta;
    
    double aa = a * a, bb = b * b, cc = c * c, dd = d * d;
    double bc = b * c, ad = a * d, ac = a * c, ab = a * b, bd = b * d, cd = c * d;
    
    Eigen::Matrix3d R;
    R(0, 0) = aa + bb - cc - dd;
    R(0, 1) = 2.0 * (bc + ad);
    R(0, 2) = 2.0 * (bd - ac);
    R(1, 0) = 2.0 * (bc - ad);
    R(1, 1) = aa + cc - bb - dd;
    R(1, 2) = 2.0 * (cd + ab);
    R(2, 0) = 2.0 * (bd + ac);
    R(2, 1) = 2.0 * (cd - ab);
    R(2, 2) = aa + dd - bb - cc;
    
    return R;
}

std::pair<Eigen::Vector3d, double> axisAndAngle(
    const Eigen::Vector3d& oldDir,
    const Eigen::Vector3d& newDir)
{
    // Check for zero vectors
    if (oldDir.norm() < 1e-10 || newDir.norm() < 1e-10) {
        return std::make_pair(Eigen::Vector3d::Zero(), 0.0);
    }
    
    // Normalize vectors
    Eigen::Vector3d oldNorm = oldDir.normalized();
    Eigen::Vector3d newNorm = newDir.normalized();
    
    // Calculate angle using dot product
    double dotProduct = oldNorm.dot(newNorm);
    // Clamp to [-1, 1] for acos
    dotProduct = std::max(-1.0, std::min(1.0, dotProduct));
    double angle = std::acos(dotProduct);
    
    // Calculate axis using cross product
    Eigen::Vector3d axis = newNorm.cross(oldNorm);
    
    // Handle parallel/antiparallel vectors
    if (axis.norm() < 1e-10) {
        // Vectors are parallel or antiparallel
        if (newNorm(0) == 0 && newNorm(1) == 0) {
            axis = Eigen::Vector3d(0.0, 1.0, 0.0);
        } else {
            axis = Eigen::Vector3d(-newNorm(1), newNorm(0), 0.0);
            axis.normalize();
        }
    } else {
        axis.normalize();
    }
    
    return std::make_pair(axis, angle);
}

std::vector<DCP9::Geometry::Point> rotateAboutAxis(
    const std::vector<DCP9::Geometry::Point>& points,
    const Eigen::Vector3d& axis,
    double theta)
{
    Eigen::Matrix3d R = rotationMatrixFromAxisAngle(axis, theta);
    Eigen::Vector3d zeroTranslation(0.0, 0.0, 0.0);
    
    return transformPoints(points, R, zeroTranslation, false);
}

DCP9::Geometry::Point rotateAboutAxis(
    const DCP9::Geometry::Point& point,
    const Eigen::Vector3d& axis,
    double theta)
{
    Eigen::Matrix3d R = rotationMatrixFromAxisAngle(axis, theta);
    Eigen::Vector3d zeroTranslation(0.0, 0.0, 0.0);
    
    return transformPoint(point, R, zeroTranslation, false);
}

double calculateTransformRMS(
    const std::vector<DCP9::Geometry::Point>& sourcePoints,
    const std::vector<DCP9::Geometry::Point>& targetPoints,
    const Eigen::Matrix3d& rotation,
    const Eigen::Vector3d& translation)
{
    if (sourcePoints.empty() || sourcePoints.size() != targetPoints.size()) {
        return 0.0;
    }
    
    std::vector<double> residuals = calculateTransformResiduals(
        sourcePoints, targetPoints, rotation, translation);
    
    double sumSquares = 0.0;
    for (size_t i = 0; i < residuals.size(); ++i) {
        double residual = residuals[i];
        sumSquares += residual * residual;
    }
    
    return std::sqrt(sumSquares / residuals.size());
}

std::vector<double> calculateTransformResiduals(
    const std::vector<DCP9::Geometry::Point>& sourcePoints,
    const std::vector<DCP9::Geometry::Point>& targetPoints,
    const Eigen::Matrix3d& rotation,
    const Eigen::Vector3d& translation)
{
    std::vector<double> residuals;
    residuals.reserve(sourcePoints.size());
    
    // Transform source points
    std::vector<DCP9::Geometry::Point> transformed = transformPoints(
        sourcePoints, rotation, translation, false);
    
    // Calculate distance between transformed and target points
    for (size_t i = 0; i < transformed.size() && i < targetPoints.size(); ++i) {
        double dx = transformed[i].x() - targetPoints[i].x();
        double dy = transformed[i].y() - targetPoints[i].y();
        double dz = transformed[i].z() - targetPoints[i].z();
        double distance = std::sqrt(dx * dx + dy * dy + dz * dz);
        residuals.push_back(distance);
    }
    
    return residuals;
}

} // namespace Geometry
} // namespace Core
} // namespace DCP9





















