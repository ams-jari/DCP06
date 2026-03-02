#pragma once

#include <dcp06/database/DatabaseTypes.hpp>
#include <string>
#include <vector>
#include <memory>

namespace DCP {
namespace Database {

class IDatabase {
public:
    virtual ~IDatabase() = default;

    // Job operations
    virtual bool createJob(const std::string& jobId) = 0;
    virtual bool loadJob(const std::string& jobId) = 0;
    virtual bool saveJob(const std::string& jobId) = 0;
    virtual bool deleteJob(const std::string& jobId) = 0;
    virtual bool copyJob(const std::string& sourceId, const std::string& targetId) = 0;
    virtual bool swapJob(const std::string& jobId1, const std::string& jobId2) = 0;

    // Point operations
    virtual bool addPoint(const std::string& pointId, const PointData& data) = 0;
    virtual bool updatePoint(const std::string& pointId, const PointData& data) = 0;
    virtual bool deletePoint(const std::string& pointId) = 0;
    virtual bool getPoint(const std::string& pointId, PointData& data) const = 0;
    virtual std::vector<std::shared_ptr<PointData>> getAllPoints() const = 0;

    // Midpoint operations (Phase 1: stub)
    virtual bool addMidpoint(const std::string& midpointId, const MidpointData& data) = 0;
    virtual bool updateMidpoint(const std::string& midpointId, const MidpointData& data) = 0;
    virtual bool deleteMidpoint(const std::string& midpointId) = 0;
    virtual bool getMidpoint(const std::string& midpointId, MidpointData& data) = 0;

    // Circle operations (Phase 1: stub)
    virtual bool addCircle(const std::string& circleId, const CircleData& data) = 0;
    virtual bool updateCircle(const std::string& circleId, const CircleData& data) = 0;
    virtual bool deleteCircle(const std::string& circleId) = 0;
    virtual bool getCircle(const std::string& circleId, CircleData& data) = 0;

    // Surface scan (Phase 1: stub)
    virtual bool addSurfaceScan(const std::string& scanId, const SurfaceScanData& data,
                               const std::vector<ScanPoint3D>& points) = 0;
    virtual bool updateSurfaceScan(const std::string& scanId, const SurfaceScanData& data,
                                  const std::vector<ScanPoint3D>& points) = 0;
    virtual bool deleteSurfaceScan(const std::string& scanId) = 0;
    virtual bool getSurfaceScan(const std::string& scanId, SurfaceScanData& data) = 0;

    // ShaftAlignment (Phase 1: stub)
    virtual bool addShaftAlignment(const std::string& alignId, const ShaftAlignmentData& data) = 0;
    virtual bool updateShaftAlignment(const std::string& alignId, const ShaftAlignmentData& data) = 0;
    virtual bool deleteShaftAlignment(const std::string& alignId) = 0;
    virtual bool getShaftAlignment(const std::string& alignId, ShaftAlignmentData& data) = 0;

    // ChangeStation (Phase 1: stub)
    virtual bool addChangeStation(const std::string& stationId, const ChangeStationData& data) = 0;
    virtual bool updateChangeStation(const std::string& stationId, const ChangeStationData& data) = 0;
    virtual bool deleteChangeStation(const std::string& stationId) = 0;
    virtual bool getChangeStation(const std::string& stationId, ChangeStationData& data) = 0;
    virtual bool setActiveChangeStation(const std::string& stationId) = 0;

    // 321 Alignment / DOM (Phase 1: stub)
    virtual bool add321Alignment(const std::string& alignment321Id, const Alignment321Data& data) = 0;
    virtual bool update321Alignment(const std::string& alignment321Id, const Alignment321Data& data) = 0;
    virtual bool delete321Alignment(const std::string& alignment321Id) = 0;
    virtual bool get321Alignment(const std::string& alignment321Id, Alignment321Data& data) = 0;

    // Cylinder Alignment / COM (Phase 1: stub)
    virtual bool addCylinderAlignment(const std::string& cylinderAlignmentId, const CylinderAlignmentData& data) = 0;
    virtual bool updateCylinderAlignment(const std::string& cylinderAlignmentId, const CylinderAlignmentData& data) = 0;
    virtual bool deleteCylinderAlignment(const std::string& cylinderAlignmentId) = 0;
    virtual bool getCylinderAlignment(const std::string& cylinderAlignmentId, CylinderAlignmentData& data) = 0;

    // Best Fit Alignment / POM (Phase 1: stub)
    virtual bool addBestFitAlignment(const std::string& bestFitAlignmentId, const BestFitAlignmentData& data) = 0;
    virtual bool updateBestFitAlignment(const std::string& bestFitAlignmentId, const BestFitAlignmentData& data) = 0;
    virtual bool deleteBestFitAlignment(const std::string& bestFitAlignmentId) = 0;
    virtual bool getBestFitAlignment(const std::string& bestFitAlignmentId, BestFitAlignmentData& data) = 0;

    // Paths (for DB-primary workflow)
    virtual std::string getDataDirectory() const { return ""; }
    virtual std::string getJobWorkingPath(const std::string& jobId) const { return ""; }
    virtual std::vector<std::string> listJobIds() const { return {}; }

    // Import/export
    virtual bool importFromADF(const std::string& filename) = 0;
    virtual bool exportToADF(const std::string& filename) = 0;
    virtual bool importFromTXT(const std::string& filename, const std::string& separator = ",") = 0;
    virtual bool exportToTXT(const std::string& filename, const std::string& separator = ",") = 0;
    virtual bool importFromXLS(const std::string& filename) = 0;
    virtual bool exportToXLS(const std::string& filename) = 0;
    virtual bool importFromPRD(const std::string& filename) = 0;
    virtual bool exportToPRD(const std::string& filename) = 0;
};

}  // namespace Database
}  // namespace DCP
