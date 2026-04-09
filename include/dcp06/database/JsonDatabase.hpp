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

    /// LIST: Fill S_SELECT_POINTS with points having given tag. Backward compat: empty source treated as "3d_meas".
    /// If designValuesOnly, only points with valid design coords (pointHasDesignValues).
    short getPointListAsSelectPointsForList(S_SELECT_POINTS* pList, short iMaxPoints, short iDef, const std::string& tag,
        bool designValuesOnly = false) const;

    /// PICK: Fill S_SELECT_POINTS with points that have actual values (any tag). For copying measured coords.
    short getPointListAsSelectPointsForPick(S_SELECT_POINTS* pList, short iMaxPoints) const;

    /// Fill S_SELECT_POINT from current job (no/point_id/point_status format). Returns count.
    short getPointListAsSelectPoint(S_SELECT_POINT* pList, short iMaxPoints) const;

    /// LIST: Fill S_SELECT_POINT with points having given tag (for SelectPoint dialog).
    short getPointListAsSelectPointForList(S_SELECT_POINT* pList, short iMaxPoints, const std::string& tag) const;

    /// Get 1-based index of point in full job (by ID). Returns 0 if not found.
    int getPointIndexInJob(const std::string& pointId) const;

    /// Get 1-based index of point in tag-filtered list. Returns 0 if not found.
    int getPointIndexForList(const std::string& tag, const std::string& pointId) const;

    /// Get point by 1-based index (sorted by point ID). Fills coord buffers like form_pnt1. Returns true if found.
    bool getPointByIndex(int index1Based, bool useActual, char* pid,
        char* xact, char* xdes, char* yact, char* ydes, char* zact, char* zdes, char* note) const;

    /// Get point by 1-based index from LIST-filtered list (tag). Use after getPointListAsSelectPointsForList.
    /// designValuesOnly must match the flag used when building that list.
    bool getPointByIndexForList(const std::string& tag, int index1Based, bool useActual, char* pid,
        char* xact, char* xdes, char* yact, char* ydes, char* zact, char* zdes, char* note,
        bool designValuesOnly = false) const;

    /// Get point by 1-based index from PICK-filtered list (has actual values). Use after getPointListAsSelectPointsForPick.
    bool getPointByIndexForPick(int index1Based, char* pid,
        char* xact, char* xdes, char* yact, char* ydes, char* zact, char* zdes, char* note) const;

    /// Job display helpers (replace AdfFileFunc getFileName/getPointsCountString/etc when using DB)
    std::string getJobDisplayName() const;
    int getJobPointsCount() const;
    /// Survey points only (source `3d_meas`; empty source treated as survey). Matches LIST; used for export / next ID.
    int getSurveyPointCount() const;
    bool getLastSurveyPointId(char* out, size_t outSize) const;
    std::string getJobFileSizeString() const;
    std::string getJobModDateString() const;
    std::string getJobModTimeString() const;
    /// Swap measured/design coords for all points in current job (replaces swap_meas_design)
    bool swapMeasDesign();

    bool addMidpoint(const std::string& midpointId, const MidpointData& data) DCP_OVERRIDE;
    bool updateMidpoint(const std::string& midpointId, const MidpointData& data) DCP_OVERRIDE;
    bool deleteMidpoint(const std::string& midpointId) DCP_OVERRIDE;
    bool getMidpoint(const std::string& midpointId, MidpointData& data) DCP_OVERRIDE;
    std::vector<std::string> getMidpointIdsInJob() const;

    /// Fill S_SELECT_MIDPOINT from current job for midpoint selection dialogs. Returns count.
    short getMidpointListAsSelectMidpoint(S_SELECT_MIDPOINT* pList, short iMaxMidpoints) const;

    bool addCircle(const std::string& circleId, const CircleData& data) DCP_OVERRIDE;
    bool updateCircle(const std::string& circleId, const CircleData& data) DCP_OVERRIDE;
    bool deleteCircle(const std::string& circleId) DCP_OVERRIDE;
    bool getCircle(const std::string& circleId, CircleData& data) DCP_OVERRIDE;
    std::vector<std::string> getCircleIdsInJob() const;

    /// Fill S_SELECT_CIRCLE from current job for circle selection dialogs. Returns count.
    short getCircleListAsSelectCircle(S_SELECT_CIRCLE* pList, short iMaxCircles) const;

    bool addSurfaceScan(const std::string&, const SurfaceScanData&, const std::vector<ScanPoint3D>&) DCP_OVERRIDE { return false; }
    bool updateSurfaceScan(const std::string&, const SurfaceScanData&, const std::vector<ScanPoint3D>&) DCP_OVERRIDE { return false; }
    bool deleteSurfaceScan(const std::string&) DCP_OVERRIDE { return false; }
    bool getSurfaceScan(const std::string&, SurfaceScanData&) DCP_OVERRIDE { return false; }

    bool addShaftAlignment(const std::string&, const ShaftAlignmentData&) DCP_OVERRIDE { return false; }
    bool updateShaftAlignment(const std::string&, const ShaftAlignmentData&) DCP_OVERRIDE { return false; }
    bool deleteShaftAlignment(const std::string&) DCP_OVERRIDE { return false; }
    bool getShaftAlignment(const std::string&, ShaftAlignmentData&) DCP_OVERRIDE { return false; }

    bool addChangeStation(const std::string& changeStationId, const ChangeStationData& data) DCP_OVERRIDE;
    bool updateChangeStation(const std::string& changeStationId, const ChangeStationData& data) DCP_OVERRIDE;
    bool deleteChangeStation(const std::string& changeStationId) DCP_OVERRIDE;
    bool getChangeStation(const std::string& changeStationId, ChangeStationData& data) DCP_OVERRIDE;
    bool setActiveChangeStation(const std::string&) DCP_OVERRIDE { return false; }

    bool add321Alignment(const std::string& alignment321Id, const Alignment321Data& data) DCP_OVERRIDE;
    bool update321Alignment(const std::string& alignment321Id, const Alignment321Data& data) DCP_OVERRIDE;
    bool delete321Alignment(const std::string& alignment321Id) DCP_OVERRIDE;
    bool get321Alignment(const std::string& alignment321Id, Alignment321Data& data) DCP_OVERRIDE;
    std::vector<std::string> getAllAlignment321Ids() const;

    bool addCylinderAlignment(const std::string&, const CylinderAlignmentData&) DCP_OVERRIDE { return false; }
    bool updateCylinderAlignment(const std::string&, const CylinderAlignmentData&) DCP_OVERRIDE { return false; }
    bool deleteCylinderAlignment(const std::string&) DCP_OVERRIDE { return false; }
    bool getCylinderAlignment(const std::string&, CylinderAlignmentData&) DCP_OVERRIDE { return false; }

    bool addBestFitAlignment(const std::string& bestFitAlignmentId, const BestFitAlignmentData& data) DCP_OVERRIDE;
    bool updateBestFitAlignment(const std::string& bestFitAlignmentId, const BestFitAlignmentData& data) DCP_OVERRIDE;
    bool deleteBestFitAlignment(const std::string& bestFitAlignmentId) DCP_OVERRIDE;
    bool getBestFitAlignment(const std::string& bestFitAlignmentId, BestFitAlignmentData& data) DCP_OVERRIDE;

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
    Json::Value midpointDataToJson(const MidpointData& data);
    bool jsonToMidpointData(const Json::Value& j, MidpointData& data);
    Json::Value bestFitAlignmentDataToJson(const BestFitAlignmentData& data);
    bool jsonToBestFitAlignmentData(const Json::Value& j, BestFitAlignmentData& data);
    Json::Value changeStationDataToJson(const ChangeStationData& data);
    bool jsonToChangeStationData(const Json::Value& j, ChangeStationData& data);
    Json::Value alignment321DataToJson(const Alignment321Data& data);
    bool jsonToAlignment321Data(const Json::Value& j, Alignment321Data& data);
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
