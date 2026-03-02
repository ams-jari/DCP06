#include "core/geometry/AlignmentCalculator.h"
#include "core/geometry/Transformations.h"
#include "core/geometry/PlaneFitting.h"
#include "core/geometry/LineFitting.h"
#include "geometry/Point.h"
#include "geometry/Plane.h"
#include "geometry/Line.h"
#include <Eigen/Dense>
#include <cmath>
#include <algorithm>
#include <numeric>

namespace DCP9 {
namespace Core {
namespace Geometry {

AlignmentType AlignmentCalculator::determineAlignmentType(
    size_t numPoints,
    size_t numPlanes,
    size_t numLines)
{
    // If we have more than 2 points and no planes/lines, use BestFit
    if (numPoints > 2 && numPlanes == 0 && numLines == 0) {
        return AlignmentType::BestFit;
    }
    
    // If we have 1 plane, 1 line, and 1 point, use 3-2-1 alignment
    if (numPlanes == 1 && numLines == 1 && numPoints == 1) {
        return AlignmentType::Alignment321;
    }
    
    // If we have 1 plane and 2 points, we can create a line from the points
    if (numPlanes == 1 && numPoints == 2) {
        return AlignmentType::Alignment321;
    }
    
    // If we have multiple planes and 1 point, select best plane and line
    if (numPlanes > 1 && numPoints == 1) {
        return AlignmentType::Alignment321;
    }
    
    // If we have 1 plane and 1 point, use default Z-axis and create line
    if (numPlanes == 1 && numPoints == 1) {
        return AlignmentType::Alignment321;
    }
    
    // If we have 2 points and no planes, use default Z-axis plane and create line
    if (numPlanes == 0 && numPoints == 2) {
        return AlignmentType::Alignment321;
    }
    
    return AlignmentType::Unknown;
}

AlignmentData AlignmentCalculator::validateAlignmentData(
    const std::vector<DCP9::Geometry::Point>& designPoints,
    const std::vector<DCP9::Geometry::Point>& actualPoints,
    const std::vector<DCP9::Geometry::Plane>& designPlanes,
    const std::vector<DCP9::Geometry::Plane>& actualPlanes,
    const std::vector<DCP9::Geometry::Line>& designLines,
    const std::vector<DCP9::Geometry::Line>& actualLines,
    const std::vector<std::string>& objectIds)
{
    AlignmentData data;
    
    // Determine alignment type
    data.type = determineAlignmentType(
        designPoints.size(),
        designPlanes.size(),
        designLines.size()
    );
    
    if (data.type == AlignmentType::Unknown) {
        data.isValid = false;
        return data;
    }
    
    // Copy data
    data.designPoints = designPoints;
    data.actualPoints = actualPoints;
    data.designPlanes = designPlanes;
    data.actualPlanes = actualPlanes;
    data.designLines = designLines;
    data.actualLines = actualLines;
    
    // Handle object IDs
    if (objectIds.empty()) {
        // Generate default IDs
        for (size_t i = 0; i < designPoints.size(); ++i) {
            data.ids.push_back("Point_" + std::to_string(i));
        }
        for (size_t i = 0; i < designPlanes.size(); ++i) {
            data.ids.push_back("Plane_" + std::to_string(i));
        }
        for (size_t i = 0; i < designLines.size(); ++i) {
            data.ids.push_back("Line_" + std::to_string(i));
        }
    } else {
        data.ids = objectIds;
    }
    
    // Handle special cases for 3-2-1 alignment
    if (data.type == AlignmentType::Alignment321) {
        // If we have 1 plane and 2 points, create line from points
        if (designPlanes.size() == 1 && designPoints.size() == 2) {
            // Create line from the two points
            DCP9::Geometry::Line designLine(designPoints[0], designPoints[1]);
            DCP9::Geometry::Line actualLine(actualPoints[0], actualPoints[1]);
            data.designLines.push_back(designLine);
            data.actualLines.push_back(actualLine);
            // Use first point for position
            data.designPoints = {designPoints[0]};
            data.actualPoints = {actualPoints[0]};
            data.ids.insert(data.ids.begin() + designPoints.size(), "Line_from_points");
        }
        // If we have multiple planes and 1 point, select best plane and line
        else if (designPlanes.size() > 1 && designPoints.size() == 1) {
            // Select plane with largest Z-component (or best fit)
            size_t bestPlaneIdx = 0;
            double maxZ = 0.0;
            for (size_t i = 0; i < designPlanes.size(); ++i) {
                const auto& normal = designPlanes[i].normal();
                if (std::abs(normal[2]) > maxZ) {
                    maxZ = std::abs(normal[2]);
                    bestPlaneIdx = i;
                }
            }
            // Use selected plane
            data.designPlanes = {designPlanes[bestPlaneIdx]};
            data.actualPlanes = {actualPlanes[bestPlaneIdx]};
            // Select best line from remaining planes
            if (designPlanes.size() > 1) {
                // Use first remaining plane as line direction
                size_t lineIdx = (bestPlaneIdx == 0) ? 1 : 0;
                // Create line from plane normal (simplified)
                DCP9::Geometry::Point lineStart(0, 0, 0);
                DCP9::Geometry::Point lineEnd(
                    designPlanes[lineIdx].normal()[0],
                    designPlanes[lineIdx].normal()[1],
                    designPlanes[lineIdx].normal()[2]
                );
                DCP9::Geometry::Line designLine(lineStart, lineEnd);
                DCP9::Geometry::Point actualLineEnd(
                    actualPlanes[lineIdx].normal()[0],
                    actualPlanes[lineIdx].normal()[1],
                    actualPlanes[lineIdx].normal()[2]
                );
                DCP9::Geometry::Line actualLine(DCP9::Geometry::Point(0, 0, 0), actualLineEnd);
                data.designLines.push_back(designLine);
                data.actualLines.push_back(actualLine);
            }
        }
        // If we have 1 plane and 1 point, use default Z-axis and create line
        else if (designPlanes.size() == 1 && designPoints.size() == 1) {
            // Add default Z-axis plane (already have one plane)
            // Use plane normal as line direction
            const auto& normal = designPlanes[0].normal();
            DCP9::Geometry::Point lineEnd(normal[0], normal[1], normal[2]);
            DCP9::Geometry::Line designLine(DCP9::Geometry::Point(0, 0, 0), lineEnd);
            const auto& actualNormal = actualPlanes[0].normal();
            DCP9::Geometry::Point actualLineEnd(actualNormal[0], actualNormal[1], actualNormal[2]);
            DCP9::Geometry::Line actualLine(DCP9::Geometry::Point(0, 0, 0), actualLineEnd);
            data.designLines.push_back(designLine);
            data.actualLines.push_back(actualLine);
            data.ids.insert(data.ids.begin() + designPoints.size(), "Line_from_plane");
        }
        // If we have 2 points and no planes, use default Z-axis plane
        else if (designPlanes.size() == 0 && designPoints.size() == 2) {
            // Create default Z-axis plane
            DCP9::Geometry::Plane designPlane(DCP9::Geometry::Point(0, 0, 0), DCP9::Geometry::Point(1, 0, 0), DCP9::Geometry::Point(0, 1, 0));
            DCP9::Geometry::Plane actualPlane(DCP9::Geometry::Point(0, 0, 0), DCP9::Geometry::Point(1, 0, 0), DCP9::Geometry::Point(0, 1, 0));
            data.designPlanes.push_back(designPlane);
            data.actualPlanes.push_back(actualPlane);
            // Create line from points
            DCP9::Geometry::Line designLine(designPoints[0], designPoints[1]);
            DCP9::Geometry::Line actualLine(actualPoints[0], actualPoints[1]);
            data.designLines.push_back(designLine);
            data.actualLines.push_back(actualLine);
            // Use first point for position
            data.designPoints = {designPoints[0]};
            data.actualPoints = {actualPoints[0]};
            data.ids.insert(data.ids.begin(), "Zaxis_plane");
            data.ids.insert(data.ids.begin() + 1, "Xaxis_line");
        }
    }
    
    // Validate that we have matching counts
    if (data.type == AlignmentType::BestFit) {
        if (data.designPoints.size() != data.actualPoints.size() || data.designPoints.size() < 3) {
            data.isValid = false;
            return data;
        }
    } else if (data.type == AlignmentType::Alignment321) {
        if (data.designPlanes.size() != 1 || data.actualPlanes.size() != 1 ||
            data.designLines.size() != 1 || data.actualLines.size() != 1 ||
            data.designPoints.size() != 1 || data.actualPoints.size() != 1) {
            data.isValid = false;
            return data;
        }
    }
    
    data.isValid = true;
    return data;
}

AlignmentResult AlignmentCalculator::solveBestFitAlignment(
    const std::vector<DCP9::Geometry::Point>& designPoints,
    const std::vector<DCP9::Geometry::Point>& actualPoints,
    const std::vector<std::string>& objectIds,
    bool sortPoints)
{
    AlignmentResult result;
    result.type = AlignmentType::BestFit;
    
    if (designPoints.size() != actualPoints.size() || designPoints.size() < 3) {
        result.isValid = false;
        return result;
    }
    
    // Use solveRigidTransform (which is equivalent to solve3Drigid)
    RigidTransformResult transformResult = solveRigidTransform(
        actualPoints,  // Source: SCS (actual/measured)
        designPoints,  // Target: OCS (design)
        sortPoints
    );
    
    if (!transformResult.isValid) {
        result.isValid = false;
        return result;
    }
    
    // Copy transformation results
    result.rotation = transformResult.rotation;
    result.translation = transformResult.translation;
    result.transformMatrix = transformResult.transformMatrix;
    result.rms = transformResult.rms;
    result.residuals = transformResult.residuals;
    
    // Calculate point-wise residuals
    result.pointResiduals = calculateResiduals(
        designPoints,
        actualPoints,
        result.rotation,
        result.translation,
        objectIds
    );
    
    // Set object IDs
    if (objectIds.empty()) {
        for (size_t i = 0; i < designPoints.size(); ++i) {
            result.objectIds.push_back("Point_" + std::to_string(i));
        }
    } else {
        result.objectIds = objectIds;
    }
    
    result.isValid = true;
    return result;
}

AlignmentResult AlignmentCalculator::solve321Alignment(
    const DCP9::Geometry::Plane& designPlane,
    const DCP9::Geometry::Plane& actualPlane,
    const DCP9::Geometry::Line& designLine,
    const DCP9::Geometry::Line& actualLine,
    const DCP9::Geometry::Point& designPoint,
    const DCP9::Geometry::Point& actualPoint)
{
    AlignmentResult result;
    result.type = AlignmentType::Alignment321;
    
    // Step 1: Rotate plane normal from actual to design direction
    const auto& designNormal = designPlane.normal();
    const auto& actualNormal = actualPlane.normal();
    
    Eigen::Vector3d designNormalVec(designNormal[0], designNormal[1], designNormal[2]);
    Eigen::Vector3d actualNormalVec(actualNormal[0], actualNormal[1], actualNormal[2]);
    
    // Normalize (should already be normalized, but ensure)
    designNormalVec.normalize();
    actualNormalVec.normalize();
    
    // Calculate axis and angle for plane rotation
    auto planeAxisAngle = axisAndAngle(actualNormalVec, designNormalVec);
    Eigen::Vector3d planeAxis = planeAxisAngle.first;
    double planeAngle = planeAxisAngle.second;
    
    // Create rotation matrix for plane
    Eigen::Matrix3d RPlane = rotationMatrixFromAxisAngle(planeAxis, planeAngle);
    
    // Step 2: Rotate line direction (after plane rotation) to design direction
    // Get line direction vectors
    std::vector<double> designLineDir = designLine.direction();
    std::vector<double> actualLineDir = actualLine.direction();
    
    // Transform actual line direction using plane rotation
    Eigen::Vector3d actualLineDirVec(actualLineDir[0], actualLineDir[1], actualLineDir[2]);
    Eigen::Vector3d transformedLineDir = RPlane * actualLineDirVec;
    
    // Project transformed line onto the design plane
    // This is done by removing the component along the plane normal
    double dotProduct = transformedLineDir.dot(designNormalVec);
    Eigen::Vector3d projectedLineDir = transformedLineDir - dotProduct * designNormalVec;
    projectedLineDir.normalize();
    
    // Project design line direction onto design plane
    Eigen::Vector3d designLineDirVec(designLineDir[0], designLineDir[1], designLineDir[2]);
    double designDot = designLineDirVec.dot(designNormalVec);
    Eigen::Vector3d projectedDesignLineDir = designLineDirVec - designDot * designNormalVec;
    projectedDesignLineDir.normalize();
    
    // Calculate axis and angle for line rotation (in plane)
    auto lineAxisAngle = axisAndAngle(projectedLineDir, projectedDesignLineDir);
    Eigen::Vector3d lineAxis = lineAxisAngle.first;
    double lineAngle = lineAxisAngle.second;
    
    // Create rotation matrix for line
    Eigen::Matrix3d RLine = rotationMatrixFromAxisAngle(lineAxis, lineAngle);
    
    // Step 3: Compound rotation: R = RLine * RPlane
    Eigen::Matrix3d R = RLine * RPlane;
    
    // Step 4: Calculate translation
    // Transform actual point using rotation
    Eigen::Vector3d actualPointVec(actualPoint.x(), actualPoint.y(), actualPoint.z());
    Eigen::Vector3d transformedPoint = R * actualPointVec;
    
    // Calculate translation to match design point
    Eigen::Vector3d designPointVec(designPoint.x(), designPoint.y(), designPoint.z());
    Eigen::Vector3d t = designPointVec - transformedPoint;
    
    // Store results
    result.rotation = R;
    result.translation = t;
    
    // Build 4x4 transformation matrix
    result.transformMatrix = Eigen::Matrix4d::Identity();
    result.transformMatrix.block<3, 3>(0, 0) = R;
    result.transformMatrix.block<3, 1>(0, 3) = t;
    
    // Calculate RMS (using the point)
    std::vector<DCP9::Geometry::Point> transformedPoints = transformPoints({actualPoint}, R, t, false);
    if (!transformedPoints.empty()) {
        double dx = transformedPoints[0].x() - designPoint.x();
        double dy = transformedPoints[0].y() - designPoint.y();
        double dz = transformedPoints[0].z() - designPoint.z();
        result.rms = std::sqrt(dx*dx + dy*dy + dz*dz);
        result.residuals = {result.rms};
    }
    
    result.isValid = true;
    return result;
}

AlignmentResult AlignmentCalculator::solveAlignment(
    const std::vector<DCP9::Geometry::Point>& designPoints,
    const std::vector<DCP9::Geometry::Point>& actualPoints,
    const std::vector<DCP9::Geometry::Plane>& designPlanes,
    const std::vector<DCP9::Geometry::Plane>& actualPlanes,
    const std::vector<DCP9::Geometry::Line>& designLines,
    const std::vector<DCP9::Geometry::Line>& actualLines,
    const std::vector<std::string>& objectIds,
    AlignmentType alignmentType,
    bool sortPoints)
{
    AlignmentResult result;
    
    // Validate and prepare data
    AlignmentData data = validateAlignmentData(
        designPoints, actualPoints,
        designPlanes, actualPlanes,
        designLines, actualLines,
        objectIds
    );
    
    if (!data.isValid) {
        result.isValid = false;
        return result;
    }
    
    // Determine alignment type if not specified
    if (alignmentType == AlignmentType::Unknown) {
        alignmentType = data.type;
    }
    
    // Perform alignment based on type
    if (alignmentType == AlignmentType::BestFit) {
        result = solveBestFitAlignment(
            data.designPoints,
            data.actualPoints,
            data.ids,
            sortPoints
        );
    } else if (alignmentType == AlignmentType::Alignment321) {
        if (data.designPlanes.size() == 1 && data.actualPlanes.size() == 1 &&
            data.designLines.size() == 1 && data.actualLines.size() == 1 &&
            data.designPoints.size() == 1 && data.actualPoints.size() == 1) {
            result = solve321Alignment(
                data.designPlanes[0],
                data.actualPlanes[0],
                data.designLines[0],
                data.actualLines[0],
                data.designPoints[0],
                data.actualPoints[0]
            );
            result.objectIds = data.ids;
        } else {
            result.isValid = false;
        }
    } else {
        result.isValid = false;
    }
    
    return result;
}

std::map<std::string, std::vector<double>> AlignmentCalculator::calculateResiduals(
    const std::vector<DCP9::Geometry::Point>& designPoints,
    const std::vector<DCP9::Geometry::Point>& actualPoints,
    const Eigen::Matrix3d& rotation,
    const Eigen::Vector3d& translation,
    const std::vector<std::string>& objectIds)
{
    std::map<std::string, std::vector<double>> residuals;
    
    // Transform actual points
    std::vector<DCP9::Geometry::Point> transformedPoints = transformPoints(
        actualPoints, rotation, translation, false);
    
    // Calculate residuals for each point
    for (size_t i = 0; i < transformedPoints.size() && i < designPoints.size(); ++i) {
        double dx = transformedPoints[i].x() - designPoints[i].x();
        double dy = transformedPoints[i].y() - designPoints[i].y();
        double dz = transformedPoints[i].z() - designPoints[i].z();
        double distance = std::sqrt(dx*dx + dy*dy + dz*dz);
        
        std::string id;
        if (i < objectIds.size()) {
            id = objectIds[i];
        } else {
            id = "Point_" + std::to_string(i);
        }
        
        residuals[id] = {distance, dx, dy, dz};
    }
    
    return residuals;
}

double AlignmentCalculator::calculateRMS(const std::map<std::string, std::vector<double>>& residuals)
{
    if (residuals.empty()) {
        return 0.0;
    }
    
    double sumSquares = 0.0;
    for (const auto& pair : residuals) {
        if (!pair.second.empty()) {
            double distance = pair.second[0]; // First element is total distance
            sumSquares += distance * distance;
        }
    }
    
    return std::sqrt(sumSquares / residuals.size());
}

} // namespace Geometry
} // namespace Core
} // namespace DCP9

