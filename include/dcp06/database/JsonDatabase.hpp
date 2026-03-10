#pragma once

#include <dcp06/database/IDatabase.hpp>
#include <dcp06/database/DatabaseTypes.hpp>
#include <dcp06/core/Defs.hpp>
#include <json/json.h>
#include <string>
#include <memory>
#include <vector>

namespace DCP {
namespace Database {

class JsonDatabase : public IDatabase {
public:
    JsonDatabase();
    ~JsonDatabase();

    void setDataDirectory(const std::string& path);
    std::string getDataDirectory() const DCP_OVERRIDE;
    std::string getJobWorkingPath(const std::string& jobId) const DCP_OVERRIDE;
    std::vector<std::string> listJobIds() const DCP_OVERRIDE;
    bool isJobLoaded() const;
    DCP_UNIQUE_PTR<JobData> getCurrentJob() const;

    bool createJob(const std::string& jobId) DCP_OVERRIDE;
    bool loadJob(const std::string& jobId) DCP_OVERRIDE;
    bool saveJob(const std::string& jobId) DCP_OVERRIDE;
    void closeJob();
    bool deleteJob(const std::string& jobId) DCP_OVERRIDE;
    bool copyJob(const std::string& sourceId, const std::string& targetId) DCP_OVERRIDE;
    bool swapJob(const std::string& jobId1, const std::string& jobId2) DCP_OVERRIDE;

    bool addPoint(const std::string& pointId, const PointData& data) DCP_OVERRIDE;
    bool updatePoint(const std::string& pointId, const PointData& data) DCP_OVERRIDE;
    bool deletePoint(const std::string& pointId) DCP_OVERRIDE;
    bool getPoint(const std::string& pointId, PointData& data) const DCP_OVERRIDE;
    std::vector<DCP_SHARED_PTR<PointData> > getAllPoints() const DCP_OVERRIDE;

    /// DCP9-style: all points in current job (main points; extend later for alignments/circles/etc.)
    std::vector<DCP_SHARED_PTR<PointData> > getAllPointsInJob() const;

    /// Fill S_SELECT_POINTS from current job for point selection dialogs. Returns count.
    short getPointListAsSelectPoints(S_SELECT_POINTS* pList, short iMaxPoints, short iDef) const;

    /// Fill S_SELECT_POINT from current job (no/point_id/point_status format). Returns count.
    short getPointListAsSelectPoint(S_SELECT_POINT* pList, short iMaxPoints) const;

    /// Get point by 1-based index (sorted by point ID). Fills coord buffers like form_pnt1. Returns true if found.
    bool getPointByIndex(int index1Based, bool useActual, char* pid,
        char* xact, char* xdes, char* yact, char* ydes, char* zact, char* zdes, char* note) const;

    /// Job display helpers (replace AdfFileFunc getFileName/getPointsCountString/etc when using DB)
    std::string getJobDisplayName() const;
    int getJobPointsCount() const;
    std::string getJobFileSizeString() const;
    std::string getJobModDateString() const;
    std::string getJobModTimeString() const;
    /// Swap measured/design coords for all points in current job (replaces swap_meas_design)
    bool swapMeasDesign();

    bool addMidpoint(const std::string&, const MidpointData&) DCP_OVERRIDE { return false; }
    bool updateMidpoint(const std::string&, const MidpointData&) DCP_OVERRIDE { return false; }
    bool deleteMidpoint(const std::string&) DCP_OVERRIDE { return false; }
    bool getMidpoint(const std::string&, MidpointData&) DCP_OVERRIDE { return false; }

    bool addCircle(const std::string& circleId, const CircleData& data) DCP_OVERRIDE;
    bool updateCircle(const std::string& circleId, const CircleData& data) DCP_OVERRIDE;
    bool deleteCircle(const std::string& circleId) DCP_OVERRIDE;
    bool getCircle(const std::string& circleId, CircleData& data) DCP_OVERRIDE;
    std::vector<std::string> getCircleIdsInJob() const;

    bool addSurfaceScan(const std::string&, const SurfaceScanData&, const std::vector<ScanPoint3D>&) DCP_OVERRIDE { return false; }
    bool updateSurfaceScan(const std::string&, const SurfaceScanData&, const std::vector<ScanPoint3D>&) DCP_OVERRIDE { return false; }
    bool deleteSurfaceScan(const std::string&) DCP_OVERRIDE { return false; }
    bool getSurfaceScan(const std::string&, SurfaceScanData&) DCP_OVERRIDE { return false; }

    bool addShaftAlignment(const std::string&, const ShaftAlignmentData&) DCP_OVERRIDE { return false; }
    bool updateShaftAlignment(const std::string&, const ShaftAlignmentData&) DCP_OVERRIDE { return false; }
    bool deleteShaftAlignment(const std::string&) DCP_OVERRIDE { return false; }
    bool getShaftAlignment(const std::string&, ShaftAlignmentData&) DCP_OVERRIDE { return false; }

    bool addChangeStation(const std::string&, const ChangeStationData&) DCP_OVERRIDE { return false; }
    bool updateChangeStation(const std::string&, const ChangeStationData&) DCP_OVERRIDE { return false; }
    bool deleteChangeStation(const std::string&) DCP_OVERRIDE { return false; }
    bool getChangeStation(const std::string&, ChangeStationData&) DCP_OVERRIDE { return false; }
    bool setActiveChangeStation(const std::string&) DCP_OVERRIDE { return false; }

    bool add321Alignment(const std::string&, const Alignment321Data&) DCP_OVERRIDE { return false; }
    bool update321Alignment(const std::string&, const Alignment321Data&) DCP_OVERRIDE { return false; }
    bool delete321Alignment(const std::string&) DCP_OVERRIDE { return false; }
    bool get321Alignment(const std::string&, Alignment321Data&) DCP_OVERRIDE { return false; }

    bool addCylinderAlignment(const std::string&, const CylinderAlignmentData&) DCP_OVERRIDE { return false; }
    bool updateCylinderAlignment(const std::string&, const CylinderAlignmentData&) DCP_OVERRIDE { return false; }
    bool deleteCylinderAlignment(const std::string&) DCP_OVERRIDE { return false; }
    bool getCylinderAlignment(const std::string&, CylinderAlignmentData&) DCP_OVERRIDE { return false; }

    bool addBestFitAlignment(const std::string&, const BestFitAlignmentData&) DCP_OVERRIDE { return false; }
    bool updateBestFitAlignment(const std::string&, const BestFitAlignmentData&) DCP_OVERRIDE { return false; }
    bool deleteBestFitAlignment(const std::string&) DCP_OVERRIDE { return false; }
    bool getBestFitAlignment(const std::string&, BestFitAlignmentData&) DCP_OVERRIDE { return false; }

    bool importFromADF(const std::string& filename) DCP_OVERRIDE;
    bool exportToADF(const std::string& filename) DCP_OVERRIDE;
    bool importFromTXT(const std::string& filename, const std::string& separator = ",") DCP_OVERRIDE;
    bool exportToTXT(const std::string& filename, const std::string& separator = ",") DCP_OVERRIDE;
    bool importFromXLS(const std::string&) DCP_OVERRIDE { return false; }
    bool exportToXLS(const std::string&) DCP_OVERRIDE { return false; }
    bool importFromPRD(const std::string&) DCP_OVERRIDE { return false; }
    bool exportToPRD(const std::string&) DCP_OVERRIDE { return false; }

private:
    std::string getJobFilePath(const std::string& jobId) const;
    bool writeJsonToFile(const std::string& filePath, const Json::Value& j);
    bool readJsonFromFile(const std::string& filePath, Json::Value& j);
    Json::Value pointDataToJson(const PointData& data);
    bool jsonToPointData(const Json::Value& j, PointData& data);
    Json::Value circleDataToJson(const CircleData& data);
    bool jsonToCircleData(const Json::Value& j, CircleData& data);
    Json::Value jobDataToJson(const JobData& data);
    bool jsonToJobData(const Json::Value& j, JobData& data);
    static double parseDouble(const std::string& s);
    static std::string getCurrentIsoTime();

    std::string m_dataDirectory;
    DCP_UNIQUE_PTR<JobData> m_currentJob;
    bool m_isJobLoaded;
};

}  // namespace Database
}  // namespace DCP
