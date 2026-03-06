#pragma once

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <Eigen/Dense>
#include "geometry/Point.h"
#include "geometry/Plane.h"
#include "geometry/Line.h"
#include "core/geometry/Transformations.h"

namespace DCP9 {
namespace Core {
namespace Geometry {

/**
 * @brief Alignment type enumeration (C++03 compatible)
 */
enum AlignmentType {
    AlignmentType_Unknown,      // Cannot determine alignment type
    AlignmentType_BestFit,      // Best Fit Alignment using points only
    AlignmentType_321           // 321 Alignment using plane + line + reference point
};

/**
 * @brief Result structure for alignment operations
 */
struct AlignmentResult {
    Eigen::Matrix3d rotation;           // 3x3 rotation matrix
    Eigen::Vector3d translation;       // 3x1 translation vector
    Eigen::Matrix4d transformMatrix;    // 4x4 homogeneous transformation matrix
    AlignmentType type;                 // Type of alignment performed
    double rms;                         // Root mean square error
    std::vector<double> residuals;     // Residuals for each point
    std::map<std::string, std::vector<double> > pointResiduals;  // Residuals by point ID
    std::vector<std::string> objectIds; // IDs of objects used in alignment
    bool isValid;                      // Whether the alignment was successful

    AlignmentResult() : type(AlignmentType_Unknown), rms(0.0), isValid(false) {}
};

/**
 * @brief Validated alignment data structure
 */
struct AlignmentData {
    AlignmentType type;
    std::vector<std::string> ids;
    std::vector<DCP9::Geometry::Point> designPoints;
    std::vector<DCP9::Geometry::Point> actualPoints;
    std::vector<DCP9::Geometry::Plane> designPlanes;
    std::vector<DCP9::Geometry::Plane> actualPlanes;
    std::vector<DCP9::Geometry::Line> designLines;
    std::vector<DCP9::Geometry::Line> actualLines;
    bool isValid;

    AlignmentData() : type(AlignmentType_Unknown), isValid(false) {}
};

/**
 * @brief Alignment calculator for 321 Alignment, Best Fit Alignment, and Cylinder Alignment workflows
 * 
 * This class implements alignment calculations that transform points from
 * Sensor Coordinate System (SCS) to Object Coordinate System (OCS).
 * 
 * Supports:
 * - Best Fit Alignment (using points only)
 * - 321 Alignment (using plane + line + reference point)
 * - Cylinder Alignment (using cylinder features)
 */
class AlignmentCalculator {
public:
    /**
     * @brief Main alignment calculation function (equivalent to PMP9's solveAlignment)
     * 
     * Solves rotation matrix and translation vector to align two data sets.
     * 
     * @param designPoints Design points (OCS coordinates)
     * @param actualPoints Actual/measured points (SCS coordinates)
     * @param designPlanes Design planes (OCS)
     * @param actualPlanes Actual planes (SCS)
     * @param designLines Design lines (OCS)
     * @param actualLines Actual lines (SCS)
     * @param objectIds IDs of objects used in alignment
     * @param alignmentType Type of alignment (Auto, BestFit, or Alignment321)
     * @param sortPoints If true, automatically find point correspondences
     * @return AlignmentResult containing rotation, translation, RMS, and residuals
     */
    static AlignmentResult solveAlignment(
        const std::vector<DCP9::Geometry::Point>& designPoints,
        const std::vector<DCP9::Geometry::Point>& actualPoints,
        const std::vector<DCP9::Geometry::Plane>& designPlanes = std::vector<DCP9::Geometry::Plane>(),
        const std::vector<DCP9::Geometry::Plane>& actualPlanes = std::vector<DCP9::Geometry::Plane>(),
        const std::vector<DCP9::Geometry::Line>& designLines = std::vector<DCP9::Geometry::Line>(),
        const std::vector<DCP9::Geometry::Line>& actualLines = std::vector<DCP9::Geometry::Line>(),
        const std::vector<std::string>& objectIds = std::vector<std::string>(),
        AlignmentType alignmentType = AlignmentType_Unknown,
        bool sortPoints = true
    );
    
    /**
     * @brief Validates alignment data and determines alignment type (equivalent to checkAlignData)
     * 
     * Checks what kind of alignment computation can be performed with the given data.
     * 
     * @param numPoints Number of points
     * @param numPlanes Number of planes
     * @param numLines Number of lines
     * @return AlignmentType that can be performed, or Unknown if not possible
     */
    static AlignmentType determineAlignmentType(
        size_t numPoints,
        size_t numPlanes,
        size_t numLines
    );
    
    /**
     * @brief Validates and prepares alignment data
     * 
     * @param designPoints Design points
     * @param actualPoints Actual points
     * @param designPlanes Design planes
     * @param actualPlanes Actual planes
     * @param designLines Design lines
     * @param actualLines Actual lines
     * @param objectIds Object IDs
     * @return AlignmentData structure with validated data
     */
    static AlignmentData validateAlignmentData(
        const std::vector<DCP9::Geometry::Point>& designPoints,
        const std::vector<DCP9::Geometry::Point>& actualPoints,
        const std::vector<DCP9::Geometry::Plane>& designPlanes = std::vector<DCP9::Geometry::Plane>(),
        const std::vector<DCP9::Geometry::Plane>& actualPlanes = std::vector<DCP9::Geometry::Plane>(),
        const std::vector<DCP9::Geometry::Line>& designLines = std::vector<DCP9::Geometry::Line>(),
        const std::vector<DCP9::Geometry::Line>& actualLines = std::vector<DCP9::Geometry::Line>(),
        const std::vector<std::string>& objectIds = std::vector<std::string>()
    );
    
    /**
     * @brief Solves Best Fit Alignment using points only
     * 
     * Uses solveRigidTransform() with corresponding points.
     * 
     * @param designPoints Design points (OCS)
     * @param actualPoints Actual points (SCS)
     * @param objectIds Point IDs
     * @param sortPoints If true, automatically find point correspondences
     * @return AlignmentResult
     */
    static AlignmentResult solveBestFitAlignment(
        const std::vector<DCP9::Geometry::Point>& designPoints,
        const std::vector<DCP9::Geometry::Point>& actualPoints,
        const std::vector<std::string>& objectIds = std::vector<std::string>(),
        bool sortPoints = true
    );
    
    /**
     * @brief Solves 321 Alignment using plane + line + reference point
     * 
     * Multi-step rotation:
     * 1. Rotate plane normal from actual to design direction
     * 2. Rotate line direction (after plane rotation) to design direction
     * 3. Calculate compound rotation: R = RLine * RPlane
     * 4. Calculate translation to match design point
     * 
     * @param designPlane Design plane (OCS)
     * @param actualPlane Actual plane (SCS)
     * @param designLine Design line (OCS)
     * @param actualLine Actual line (SCS)
     * @param designPoint Design point (OCS)
     * @param actualPoint Actual point (SCS)
     * @return AlignmentResult
     */
    static AlignmentResult solve321Alignment(
        const DCP9::Geometry::Plane& designPlane,
        const DCP9::Geometry::Plane& actualPlane,
        const DCP9::Geometry::Line& designLine,
        const DCP9::Geometry::Line& actualLine,
        const DCP9::Geometry::Point& designPoint,
        const DCP9::Geometry::Point& actualPoint
    );
    
    /**
     * @brief Calculates alignment residuals
     * 
     * @param designPoints Design points
     * @param actualPoints Actual points
     * @param rotation Rotation matrix
     * @param translation Translation vector
     * @param objectIds Point IDs
     * @return Map of point ID to residual vector [total, x, y, z]
     */
    static std::map<std::string, std::vector<double>> calculateResiduals(
        const std::vector<DCP9::Geometry::Point>& designPoints,
        const std::vector<DCP9::Geometry::Point>& actualPoints,
        const Eigen::Matrix3d& rotation,
        const Eigen::Vector3d& translation,
        const std::vector<std::string>& objectIds = std::vector<std::string>()
    );
    
    /**
     * @brief Calculates RMS error for alignment
     * 
     * @param residuals Map of point residuals
     * @return RMS value
     */
    static double calculateRMS(const std::map<std::string, std::vector<double>>& residuals);
};

} // namespace Geometry
} // namespace Core
} // namespace DCP9
