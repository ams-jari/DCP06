#pragma once

#include <dcp06/database/IDatabase.hpp>
#include <string>
#include <vector>

namespace DCP {
namespace Database {

/**
 * Stub implementation of IDatabase for VS2008 (C++03) builds where
 * JsonDatabase is not available (e.g. ARM/embedded builds).
 * All operations return false or empty; JSON job features are disabled.
 */
class StubDatabase : public IDatabase {
public:
    StubDatabase() {}
    virtual ~StubDatabase() {}

    bool createJob(const std::string&) { return false; }
    bool loadJob(const std::string&) { return false; }
    bool saveJob(const std::string&) { return false; }
    bool deleteJob(const std::string&) { return false; }
    bool copyJob(const std::string&, const std::string&) { return false; }
    bool swapJob(const std::string&, const std::string&) { return false; }

    bool addPoint(const std::string&, const PointData&) { return false; }
    bool updatePoint(const std::string&, const PointData&) { return false; }
    bool deletePoint(const std::string&) { return false; }
    bool getPoint(const std::string&, PointData&) const { return false; }
    std::vector<DCP_SHARED_PTR<PointData> > getAllPoints() const {
        return std::vector<DCP_SHARED_PTR<PointData> >();
    }

    bool addMidpoint(const std::string&, const MidpointData&) { return false; }
    bool updateMidpoint(const std::string&, const MidpointData&) { return false; }
    bool deleteMidpoint(const std::string&) { return false; }
    bool getMidpoint(const std::string&, MidpointData&) { return false; }
    bool addCircle(const std::string&, const CircleData&) { return false; }
    bool updateCircle(const std::string&, const CircleData&) { return false; }
    bool deleteCircle(const std::string&) { return false; }
    bool getCircle(const std::string&, CircleData&) { return false; }
    bool addSurfaceScan(const std::string&, const SurfaceScanData&, const std::vector<ScanPoint3D>&) { return false; }
    bool updateSurfaceScan(const std::string&, const SurfaceScanData&, const std::vector<ScanPoint3D>&) { return false; }
    bool deleteSurfaceScan(const std::string&) { return false; }
    bool getSurfaceScan(const std::string&, SurfaceScanData&) { return false; }
    bool addShaftAlignment(const std::string&, const ShaftAlignmentData&) { return false; }
    bool updateShaftAlignment(const std::string&, const ShaftAlignmentData&) { return false; }
    bool deleteShaftAlignment(const std::string&) { return false; }
    bool getShaftAlignment(const std::string&, ShaftAlignmentData&) { return false; }
    bool addChangeStation(const std::string&, const ChangeStationData&) { return false; }
    bool updateChangeStation(const std::string&, const ChangeStationData&) { return false; }
    bool deleteChangeStation(const std::string&) { return false; }
    bool getChangeStation(const std::string&, ChangeStationData&) { return false; }
    bool setActiveChangeStation(const std::string&) { return false; }
    bool add321Alignment(const std::string&, const Alignment321Data&) { return false; }
    bool update321Alignment(const std::string&, const Alignment321Data&) { return false; }
    bool delete321Alignment(const std::string&) { return false; }
    bool get321Alignment(const std::string&, Alignment321Data&) { return false; }
    bool addCylinderAlignment(const std::string&, const CylinderAlignmentData&) { return false; }
    bool updateCylinderAlignment(const std::string&, const CylinderAlignmentData&) { return false; }
    bool deleteCylinderAlignment(const std::string&) { return false; }
    bool getCylinderAlignment(const std::string&, CylinderAlignmentData&) { return false; }
    bool addBestFitAlignment(const std::string&, const BestFitAlignmentData&) { return false; }
    bool updateBestFitAlignment(const std::string&, const BestFitAlignmentData&) { return false; }
    bool deleteBestFitAlignment(const std::string&) { return false; }
    bool getBestFitAlignment(const std::string&, BestFitAlignmentData&) { return false; }

    bool importFromADF(const std::string&) { return false; }
    bool exportToADF(const std::string&) { return false; }
    bool importFromTXT(const std::string&, const std::string&) { return false; }
    bool exportToTXT(const std::string&, const std::string&) { return false; }
    bool importFromXLS(const std::string&) { return false; }
    bool exportToXLS(const std::string&) { return false; }
    bool importFromPRD(const std::string&) { return false; }
    bool exportToPRD(const std::string&) { return false; }
};

}  // namespace Database
}  // namespace DCP
