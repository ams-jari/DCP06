#include "stdafx.h"
#include <dcp06/database/JsonDatabase.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <cmath>

namespace DCP {
namespace Database {

namespace {
std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}
}  // namespace

JsonDatabase::JsonDatabase() : m_isJobLoaded(false) {}

JsonDatabase::~JsonDatabase() {
    m_currentJob.reset();
}

void JsonDatabase::setDataDirectory(const std::string& path) {
    m_dataDirectory = path;
    boost::filesystem::path p(path);
    if (!boost::filesystem::exists(p)) {
        boost::system::error_code ec;
        boost::filesystem::create_directories(p, ec);
    }
}

std::string JsonDatabase::getDataDirectory() const {
    return m_dataDirectory;
}

bool JsonDatabase::isJobLoaded() const {
    return m_isJobLoaded && m_currentJob;
}

std::unique_ptr<JobData> JsonDatabase::getCurrentJob() const {
    if (!m_currentJob) return nullptr;
    // Return a copy for safety
    auto copy = std::make_unique<JobData>(*m_currentJob);
    copy->points.clear();
    for (const auto& kv : m_currentJob->points) {
        if (kv.second)
            copy->points[kv.first] = std::make_shared<PointData>(*kv.second);
    }
    return copy;
}

std::string JsonDatabase::getCurrentIsoTime() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    struct tm buf;
#ifdef _WIN32
    localtime_s(&buf, &time);
#else
    localtime_r(&time, &buf);
#endif
    oss << std::put_time(&buf, "%Y-%m-%dT%H:%M:%S");
    return oss.str();
}

std::string JsonDatabase::getJobFilePath(const std::string& jobId) const {
    if (m_dataDirectory.empty()) return jobId + ".json";
    boost::filesystem::path p(m_dataDirectory);
    p /= jobId + ".json";
    return p.string();
}

double JsonDatabase::parseDouble(const std::string& s) {
    std::string t = trim(s);
    if (t.empty()) return std::numeric_limits<double>::quiet_NaN();
    try {
        return std::stod(t);
    } catch (...) {
        return std::numeric_limits<double>::quiet_NaN();
    }
}

bool JsonDatabase::writeJsonToFile(const std::string& filePath, const nlohmann::json& j) {
    std::ofstream f(filePath);
    if (!f.is_open()) return false;
    f << j.dump(2);
    return true;
}

bool JsonDatabase::readJsonFromFile(const std::string& filePath, nlohmann::json& j) {
    std::ifstream f(filePath);
    if (!f.is_open()) return false;
    try {
        j = nlohmann::json::parse(f);
        return true;
    } catch (...) {
        return false;
    }
}

nlohmann::json JsonDatabase::pointDataToJson(const PointData& data) {
    nlohmann::json j;
    j["id"] = data.id;
    j["type"] = data.type.empty() ? "point" : data.type;
    j["meas_time"] = data.measTime;
    j["measurer"] = data.measurer;
    j["instrument_id"] = data.instrumentId;
    j["calculated"] = data.calculated;
    j["rms"] = data.rms;
    auto setD = [&j](const char* key, double v) {
        j[key] = (v != v || std::isnan(v)) ? nlohmann::json() : v;
    };
    setD("x_dsg", data.x_dsg); setD("y_dsg", data.y_dsg); setD("z_dsg", data.z_dsg);
    setD("x_mea", data.x_mea); setD("y_mea", data.y_mea); setD("z_mea", data.z_mea);
    setD("x_scs", data.x_scs); setD("y_scs", data.y_scs); setD("z_scs", data.z_scs);
    setD("x_scs0", data.x_scs0); setD("y_scs0", data.y_scs0); setD("z_scs0", data.z_scs0);
    j["hor_angle"] = data.hor_angle; j["ver_angle"] = data.ver_angle; j["distance"] = data.distance;
    setD("precision", data.precision); setD("dev_x", data.dev_x); setD("dev_y", data.dev_y); setD("dev_z", data.dev_z);
    j["status"] = data.status; j["note"] = data.note;
    j["offset_x"] = data.offset_x; j["offset_y"] = data.offset_y; j["offset_z"] = data.offset_z;
    j["partial"] = data.partial; j["prism"] = data.prism; j["prism_constant"] = data.prism_constant;
    j["spherical_prism_correction"] = data.spherical_prism_correction;
    j["orientation_type"] = data.orientation_type; j["orientation_id"] = data.orientation_id;
    j["orientation_object_key"] = data.orientation_object_key; j["change_station_object_key"] = data.change_station_object_key;
    return j;
}

bool JsonDatabase::jsonToPointData(const nlohmann::json& j, PointData& data) {
    auto getD = [&j](const char* key) -> double {
        if (!j.contains(key) || j[key].is_null()) return std::numeric_limits<double>::quiet_NaN();
        if (j[key].is_number()) return j[key].get<double>();
        if (j[key].is_string()) { try { return std::stod(j[key].get<std::string>()); } catch (...) { return std::numeric_limits<double>::quiet_NaN(); } }
        return std::numeric_limits<double>::quiet_NaN();
    };
    data.id = j.value("id", "");
    data.type = j.value("type", "point");
    data.measTime = j.value("meas_time", "");
    data.measurer = j.value("measurer", "");
    data.instrumentId = j.value("instrument_id", "");
    data.calculated = j.value("calculated", false);
    data.rms = j.value("rms", 0.0);
    data.x_dsg = getD("x_dsg"); data.y_dsg = getD("y_dsg"); data.z_dsg = getD("z_dsg");
    data.x_mea = getD("x_mea"); data.y_mea = getD("y_mea"); data.z_mea = getD("z_mea");
    data.x_scs = getD("x_scs"); data.y_scs = getD("y_scs"); data.z_scs = getD("z_scs");
    data.x_scs0 = getD("x_scs0"); data.y_scs0 = getD("y_scs0"); data.z_scs0 = getD("z_scs0");
    data.hor_angle = j.value("hor_angle", 0.0); data.ver_angle = j.value("ver_angle", 0.0); data.distance = j.value("distance", 0.0);
    data.precision = getD("precision"); data.dev_x = getD("dev_x"); data.dev_y = getD("dev_y"); data.dev_z = getD("dev_z");
    data.status = j.value("status", 0); data.note = j.value("note", "");
    data.offset_x = j.value("offset_x", 0.0); data.offset_y = j.value("offset_y", 0.0); data.offset_z = j.value("offset_z", 0.0);
    data.partial = j.value("partial", false); data.prism = j.value("prism", ""); data.prism_constant = j.value("prism_constant", 0.0);
    data.spherical_prism_correction = j.value("spherical_prism_correction", false);
    data.orientation_type = j.value("orientation_type", ""); data.orientation_id = j.value("orientation_id", "");
    data.orientation_object_key = j.value("orientation_object_key", 0); data.change_station_object_key = j.value("change_station_object_key", 0);
    return true;
}

nlohmann::json JsonDatabase::jobDataToJson(const JobData& data) {
    nlohmann::json j;
    j["type"] = data.type; j["object_key"] = data.object_key; j["id"] = data.id;
    j["date"] = data.date; j["measurer"] = data.measurer; j["instrument_id"] = data.instrument_id;
    j["units"] = data.units; j["active_change_station"] = data.active_change_station;
    j["target"] = data.target; j["decimals"] = data.decimals; j["temperature"] = data.temperature;
    j["pressure"] = data.pressure; j["humidity"] = data.humidity; j["remote"] = data.remote;
    j["flow_time"] = data.flow_time; j["flow_move"] = data.flow_move;
    j["overwrite_info"] = data.overwrite_info; j["auto_increment"] = data.auto_increment;
    j["save_tool_settings"] = data.save_tool_settings; j["tool_info"] = data.tool_info;
    j["inclination_threshold"] = data.inclination_threshold; j["default_folder"] = data.default_folder;
    j["simulator"] = data.simulator; j["show_design_coords"] = data.show_design_coords;
    j["show_deviation_coords"] = data.show_deviation_coords; j["show_tool_offset"] = data.show_tool_offset;
    j["active_orientation"] = {{"type", data.active_orientation.type}, {"id", data.active_orientation.id}, {"object_key", data.active_orientation.object_key}};
    nlohmann::json pointsJson;
    for (const auto& kv : data.points) {
        if (kv.second) pointsJson[kv.first] = pointDataToJson(*kv.second);
    }
    j["points"] = pointsJson;
    return j;
}

bool JsonDatabase::jsonToJobData(const nlohmann::json& j, JobData& data) {
    data.type = j.value("type", "job"); data.object_key = j.value("object_key", 1);
    data.id = j.value("id", ""); data.date = j.value("date", "");
    data.measurer = j.value("measurer", ""); data.instrument_id = j.value("instrument_id", "");
    data.units = j.value("units", "mm"); data.active_change_station = j.value("active_change_station", "");
    data.target = j.value("target", ""); data.decimals = j.value("decimals", 2);
    data.temperature = j.value("temperature", 0.0); data.pressure = j.value("pressure", 0.0); data.humidity = j.value("humidity", 0.0);
    data.remote = j.value("remote", "yes"); data.flow_time = j.value("flow_time", 2.0); data.flow_move = j.value("flow_move", 2.0);
    data.overwrite_info = j.value("overwrite_info", "yes"); data.auto_increment = j.value("auto_increment", "no");
    data.save_tool_settings = j.value("save_tool_settings", "yes"); data.tool_info = j.value("tool_info", "yes");
    data.inclination_threshold = j.value("inclination_threshold", 0.015); data.default_folder = j.value("default_folder", "");
    data.simulator = j.value("simulator", "no");
    data.show_design_coords = j.value("show_design_coords", true);
    data.show_deviation_coords = j.value("show_deviation_coords", true);
    data.show_tool_offset = j.value("show_tool_offset", true);
    if (j.contains("active_orientation")) {
        data.active_orientation.type = j["active_orientation"].value("type", "?");
        data.active_orientation.id = j["active_orientation"].value("id", "SCS");
        data.active_orientation.object_key = j["active_orientation"].value("object_key", 0);
    }
    data.points.clear();
    if (j.contains("points") && j["points"].is_object()) {
        for (auto it = j["points"].begin(); it != j["points"].end(); ++it) {
            auto pt = std::make_shared<PointData>();
            if (jsonToPointData(it.value(), *pt)) data.points[it.key()] = pt;
        }
    }
    return true;
}

bool JsonDatabase::createJob(const std::string& jobId) {
    m_currentJob = std::make_unique<JobData>();
    std::string tid = trim(jobId);
    m_currentJob->type = "job";
    m_currentJob->object_key = 1;
    m_currentJob->id = tid.empty() ? "job1" : tid;
    m_currentJob->date = getCurrentIsoTime();
    m_currentJob->measurer = "";
    m_currentJob->instrument_id = "";
    m_currentJob->units = "mm";
    m_currentJob->active_change_station = "";
    m_currentJob->active_orientation.type = "orientation";
    m_currentJob->active_orientation.id = "SCS";
    m_currentJob->active_orientation.object_key = 0;
    m_isJobLoaded = true;
    return true;
}

bool JsonDatabase::loadJob(const std::string& jobId) {
    nlohmann::json j;
    if (!readJsonFromFile(getJobFilePath(jobId), j)) return false;
    m_currentJob = std::make_unique<JobData>();
    if (!jsonToJobData(j, *m_currentJob)) {
        m_currentJob.reset();
        return false;
    }
    m_isJobLoaded = true;
    return true;
}

bool JsonDatabase::saveJob(const std::string& jobId) {
    if (!m_isJobLoaded || !m_currentJob) return false;
    return writeJsonToFile(getJobFilePath(jobId), jobDataToJson(*m_currentJob));
}

bool JsonDatabase::deleteJob(const std::string& jobId) {
    std::string path = getJobFilePath(jobId);
    if (!boost::filesystem::exists(path)) return true;
    boost::system::error_code ec;
    return boost::filesystem::remove(path, ec);
}

bool JsonDatabase::copyJob(const std::string& sourceId, const std::string& targetId) {
    nlohmann::json j;
    if (!readJsonFromFile(getJobFilePath(sourceId), j)) return false;
    j["id"] = targetId;
    return writeJsonToFile(getJobFilePath(targetId), j);
}

bool JsonDatabase::swapJob(const std::string& jobId1, const std::string& jobId2) {
    nlohmann::json j1, j2;
    if (!readJsonFromFile(getJobFilePath(jobId1), j1) || !readJsonFromFile(getJobFilePath(jobId2), j2))
        return false;
    std::string t = j1["id"].get<std::string>();
    j1["id"] = jobId2;
    j2["id"] = t;
    return writeJsonToFile(getJobFilePath(jobId1), j2) && writeJsonToFile(getJobFilePath(jobId2), j1);
}

bool JsonDatabase::addPoint(const std::string& pointId, const PointData& data) {
    if (!m_currentJob || (int)m_currentJob->points.size() >= MAX_POINTS_PER_JOB) return false;
    auto pt = std::make_shared<PointData>(data);
    pt->id = pointId;
    if (pt->measTime.empty()) pt->measTime = getCurrentIsoTime();
    m_currentJob->points[pointId] = pt;
    return true;
}

bool JsonDatabase::updatePoint(const std::string& pointId, const PointData& data) {
    if (!m_currentJob) return false;
    auto it = m_currentJob->points.find(pointId);
    if (it == m_currentJob->points.end()) return false;
    *it->second = data;
    it->second->id = pointId;
    return true;
}

bool JsonDatabase::deletePoint(const std::string& pointId) {
    if (!m_currentJob) return false;
    return m_currentJob->points.erase(pointId) > 0;
}

bool JsonDatabase::getPoint(const std::string& pointId, PointData& data) const {
    if (!m_currentJob) return false;
    auto it = m_currentJob->points.find(pointId);
    if (it == m_currentJob->points.end() || !it->second) return false;
    data = *it->second;
    return true;
}

std::vector<std::shared_ptr<PointData>> JsonDatabase::getAllPoints() const {
    std::vector<std::shared_ptr<PointData>> out;
    if (!m_currentJob) return out;
    for (const auto& kv : m_currentJob->points)
        if (kv.second) out.push_back(kv.second);
    return out;
}

bool JsonDatabase::importFromADF(const std::string& filename) {
    if (!m_currentJob) return false;
    std::ifstream f(filename);
    if (!f.is_open()) return false;
    std::string line;
    bool headerFound = false;
    int pointCount = 0;
    while (std::getline(f, line)) {
        if (line.size() < 3) continue;
        if (line.substr(0, 3) == "PID") { headerFound = true; continue; }
        if (!headerFound || line.size() < 7) continue;
        std::string pid = trim(line.substr(0, 7));
        if (pid.empty()) continue;
        PointData pt;
        pt.id = pid;
        pt.type = "point";
        pt.measTime = getCurrentIsoTime();
        auto sub = [&line](size_t pos, size_t len) {
            if (pos + len > line.size()) return std::string();
            return trim(line.substr(pos, len));
        };
        pt.x_mea = parseDouble(sub(9, 9));  pt.x_dsg = parseDouble(sub(19, 9));
        pt.y_mea = parseDouble(sub(30, 10)); pt.y_dsg = parseDouble(sub(41, 9));
        pt.z_mea = parseDouble(sub(53, 9));  pt.z_dsg = parseDouble(sub(63, 9));
        if (line.size() > 73) pt.note = trim(line.substr(73));
        bool hasActuals = !std::isnan(pt.x_mea) || !std::isnan(pt.y_mea) || !std::isnan(pt.z_mea);
        bool hasDesign = !std::isnan(pt.x_dsg) || !std::isnan(pt.y_dsg) || !std::isnan(pt.z_dsg);
        pt.status = hasActuals ? 2 : (hasDesign ? 1 : 0);
        pt.x_scs = pt.x_mea; pt.y_scs = pt.y_mea; pt.z_scs = pt.z_mea;
        pt.x_scs0 = pt.x_mea; pt.y_scs0 = pt.y_mea; pt.z_scs0 = pt.z_mea;
        addPoint(pt.id, pt);
        pointCount++;
    }
    return pointCount > 0;
}

bool JsonDatabase::exportToADF(const std::string& filename) {
    if (!m_currentJob) return false;
    std::ofstream f(filename);
    if (!f.is_open()) return false;
    f << "ADF File\nJob: " << m_currentJob->id << "\nDate: " << getCurrentIsoTime() << "\n\nPoints:\n";
    f << "ID\tX\tY\tZ\tNote\tStatus\n----------------------------------------\n";
    f << std::fixed << std::setprecision(6);
    for (const auto& kv : m_currentJob->points) {
        const auto& pt = kv.second;
        if (!pt) continue;
        f << pt->id << "\t" << pt->x_mea << "\t" << pt->y_mea << "\t" << pt->z_mea << "\t" << pt->note << "\t" << pt->status << "\n";
    }
    return true;
}

}  // namespace Database
}  // namespace DCP
