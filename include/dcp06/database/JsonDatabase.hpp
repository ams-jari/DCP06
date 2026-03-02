#pragma once

#include <dcp06/database/IDatabase.hpp>
#include <dcp06/database/DatabaseTypes.hpp>
#include <dcp06/database/nlohmann/json.hpp>
#include <string>
#include <memory>

namespace DCP {
namespace Database {

class JsonDatabase : public IDatabase {
public:
    JsonDatabase();
    ~JsonDatabase() override;

    void setDataDirectory(const std::string& path);
    std::string getDataDirectory() const;
    bool isJobLoaded() const;
    std::unique_ptr<JobData> getCurrentJob() const;

    bool createJob(const std::string& jobId) override;
    bool loadJob(const std::string& jobId) override;
    bool saveJob(const std::string& jobId) override;
    bool deleteJob(const std::string& jobId) override;
    bool copyJob(const std::string& sourceId, const std::string& targetId) override;
    bool swapJob(const std::string& jobId1, const std::string& jobId2) override;

    bool addPoint(const std::string& pointId, const PointData& data) override;
    bool updatePoint(const std::string& pointId, const PointData& data) override;
    bool deletePoint(const std::string& pointId) override;
    bool getPoint(const std::string& pointId, PointData& data) const override;
    std::vector<std::shared_ptr<PointData>> getAllPoints() const override;

    bool addMidpoint(const std::string&, const MidpointData&) override { return false; }
    bool updateMidpoint(const std::string&, const MidpointData&) override { return false; }
    bool deleteMidpoint(const std::string&) override { return false; }
    bool getMidpoint(const std::string&, MidpointData&) override { return false; }

    bool addCircle(const std::string&, const CircleData&) override { return false; }
    bool updateCircle(const std::string&, const CircleData&) override { return false; }
    bool deleteCircle(const std::string&) override { return false; }
    bool getCircle(const std::string&, CircleData&) override { return false; }

    bool addSurfaceScan(const std::string&, const SurfaceScanData&, const std::vector<ScanPoint3D>&) override { return false; }
    bool updateSurfaceScan(const std::string&, const SurfaceScanData&, const std::vector<ScanPoint3D>&) override { return false; }
    bool deleteSurfaceScan(const std::string&) override { return false; }
    bool getSurfaceScan(const std::string&, SurfaceScanData&) override { return false; }

    bool addShaftAlignment(const std::string&, const ShaftAlignmentData&) override { return false; }
    bool updateShaftAlignment(const std::string&, const ShaftAlignmentData&) override { return false; }
    bool deleteShaftAlignment(const std::string&) override { return false; }
    bool getShaftAlignment(const std::string&, ShaftAlignmentData&) override { return false; }

    bool addChangeStation(const std::string&, const ChangeStationData&) override { return false; }
    bool updateChangeStation(const std::string&, const ChangeStationData&) override { return false; }
    bool deleteChangeStation(const std::string&) override { return false; }
    bool getChangeStation(const std::string&, ChangeStationData&) override { return false; }
    bool setActiveChangeStation(const std::string&) override { return false; }

    bool add321Alignment(const std::string&, const Alignment321Data&) override { return false; }
    bool update321Alignment(const std::string&, const Alignment321Data&) override { return false; }
    bool delete321Alignment(const std::string&) override { return false; }
    bool get321Alignment(const std::string&, Alignment321Data&) override { return false; }

    bool addCylinderAlignment(const std::string&, const CylinderAlignmentData&) override { return false; }
    bool updateCylinderAlignment(const std::string&, const CylinderAlignmentData&) override { return false; }
    bool deleteCylinderAlignment(const std::string&) override { return false; }
    bool getCylinderAlignment(const std::string&, CylinderAlignmentData&) override { return false; }

    bool addBestFitAlignment(const std::string&, const BestFitAlignmentData&) override { return false; }
    bool updateBestFitAlignment(const std::string&, const BestFitAlignmentData&) override { return false; }
    bool deleteBestFitAlignment(const std::string&) override { return false; }
    bool getBestFitAlignment(const std::string&, BestFitAlignmentData&) override { return false; }

    bool importFromADF(const std::string& filename) override;
    bool exportToADF(const std::string& filename) override;
    bool importFromTXT(const std::string&, const std::string& = ",") override { return false; }
    bool exportToTXT(const std::string&, const std::string& = ",") override { return false; }
    bool importFromXLS(const std::string&) override { return false; }
    bool exportToXLS(const std::string&) override { return false; }
    bool importFromPRD(const std::string&) override { return false; }
    bool exportToPRD(const std::string&) override { return false; }

private:
    std::string getJobFilePath(const std::string& jobId) const;
    bool writeJsonToFile(const std::string& filePath, const nlohmann::json& j);
    bool readJsonFromFile(const std::string& filePath, nlohmann::json& j);
    nlohmann::json pointDataToJson(const PointData& data);
    bool jsonToPointData(const nlohmann::json& j, PointData& data);
    nlohmann::json jobDataToJson(const JobData& data);
    bool jsonToJobData(const nlohmann::json& j, JobData& data);
    static double parseDouble(const std::string& s);
    static std::string getCurrentIsoTime();

    std::string m_dataDirectory;
    std::unique_ptr<JobData> m_currentJob;
    bool m_isJobLoaded{false};
};

}  // namespace Database
}  // namespace DCP
