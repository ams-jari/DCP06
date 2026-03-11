#include "stdafx.h"
#include <dcp06/database/JsonDatabase.hpp>
#include <dcp06/core/Logger.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <cstring>
#if defined(_MSC_VER) && _MSC_VER < 1600
#include <float.h>
#define DCP_ISNAN(x) _isnan(x)
#else
#define DCP_ISNAN(x) std::isnan(x)
#endif

namespace DCP {
namespace Database {

namespace {
std::string trim(const std::string& s) {
    std::string::size_type start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    std::string::size_type end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

bool pointHasActualValues(const PointData& p) {
    return !(DCP_ISNAN(p.x_mea) && DCP_ISNAN(p.y_mea) && DCP_ISNAN(p.z_mea));
}
bool pointHasDesignValues(const PointData& p) {
    return !(DCP_ISNAN(p.x_dsg) && DCP_ISNAN(p.y_dsg) && DCP_ISNAN(p.z_dsg));
}
int pointIdSortKey(const std::string& id) {
    const char* p = id.c_str();
    while (*p && !std::isdigit(static_cast<unsigned char>(*p))) ++p;
    if (*p) {
        return static_cast<int>(std::strtol(p, 0, 10));
    }
    return 0;
}

void setJsonDouble(Json::Value& j, const char* key, double v) {
    if (v != v || DCP_ISNAN(v))
        j[key] = Json::Value();
    else
        j[key] = v;
}
double getJsonDouble(const Json::Value& j, const char* key) {
    if (!j.isMember(key) || j[key].isNull()) return std::numeric_limits<double>::quiet_NaN();
    if (j[key].isDouble()) return j[key].asDouble();
    if (j[key].isInt()) return static_cast<double>(j[key].asInt());
    if (j[key].isString()) {
        const char* p = j[key].asCString();
        char* end = 0;
        double v = std::strtod(p, &end);
        return (end != p) ? v : std::numeric_limits<double>::quiet_NaN();
    }
    return std::numeric_limits<double>::quiet_NaN();
}
std::string getJsonString(const Json::Value& j, const char* key, const std::string& def) {
    if (!j.isMember(key) || j[key].isNull()) return def;
    if (j[key].isString()) return j[key].asString();
    return def;
}
double getJsonDoubleDefault(const Json::Value& j, const char* key, double def) {
    if (!j.isMember(key) || j[key].isNull()) return def;
    if (j[key].isDouble()) return j[key].asDouble();
    if (j[key].isInt()) return static_cast<double>(j[key].asInt());
    if (j[key].isString()) {
        const char* p = j[key].asCString();
        char* end = 0;
        double v = std::strtod(p, &end);
        return (end != p) ? v : def;
    }
    return def;
}
bool getJsonBoolDefault(const Json::Value& j, const char* key, bool def) {
    if (!j.isMember(key) || j[key].isNull()) return def;
    if (j[key].isBool()) return j[key].asBool();
    return def;
}
int getJsonIntDefault(const Json::Value& j, const char* key, int def) {
    if (!j.isMember(key) || j[key].isNull()) return def;
    if (j[key].isInt()) return j[key].asInt();
    return def;
}

struct SortByPointId {
    bool operator()(const std::pair<int, DCP_SHARED_PTR<PointData> >& a,
                    const std::pair<int, DCP_SHARED_PTR<PointData> >& b) const {
        if (a.first != b.first) return a.first < b.first;
        return a.second->id < b.second->id;
    }
};
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

std::string JsonDatabase::getJobWorkingPath(const std::string& jobId) const {
    if (m_dataDirectory.empty() || jobId.empty()) return "";
    boost::filesystem::path p(m_dataDirectory);
    boost::system::error_code ec;
    if (!boost::filesystem::exists(p, ec))
        boost::filesystem::create_directories(p, ec);
    p /= jobId + ".adf";
    return p.string();
}

std::vector<std::string> JsonDatabase::listJobIds() const {
    std::vector<std::string> ids;
    if (m_dataDirectory.empty()) return ids;
    boost::filesystem::path dir(m_dataDirectory);
    boost::system::error_code ec;
    if (!boost::filesystem::exists(dir, ec) || !boost::filesystem::is_directory(dir, ec))
        return ids;
    for (boost::filesystem::directory_iterator it(dir, ec); it != boost::filesystem::directory_iterator(); it.increment(ec)) {
        if (ec) continue;
        boost::filesystem::path fp = it->path();
        if (!boost::filesystem::is_regular_file(fp, ec)) continue;
        std::string stem = fp.stem().string();
        std::string ext = fp.extension().string();
        if (ext == ".json" && !stem.empty() && stem[0] != '.')
            ids.push_back(stem);
    }
    std::sort(ids.begin(), ids.end());
    return ids;
}

bool JsonDatabase::isJobLoaded() const {
    return m_isJobLoaded && m_currentJob.get();
}

DCP_UNIQUE_PTR<JobData> JsonDatabase::getCurrentJob() const {
    if (!m_currentJob.get()) return DCP_UNIQUE_PTR<JobData>();
    DCP_UNIQUE_PTR<JobData> copy(new JobData(*m_currentJob));
    copy->points.clear();
    for (std::map<std::string, DCP_SHARED_PTR<PointData> >::const_iterator it = m_currentJob->points.begin();
         it != m_currentJob->points.end(); ++it) {
        if (it->second.get())
            copy->points[it->first] = DCP_SHARED_PTR<PointData>(new PointData(*it->second));
    }
    return copy;
}

std::string JsonDatabase::getCurrentIsoTime() {
    std::time_t t = std::time(0);
    struct tm buf;
#ifdef _WIN32
    if (localtime_s(&buf, &t) != 0) return "";
#else
    if (!localtime_r(&t, &buf)) return "";
#endif
    char out[32];
    std::strftime(out, sizeof(out), "%Y-%m-%dT%H:%M:%S", &buf);
    return std::string(out);
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
    const char* p = t.c_str();
    char* end = 0;
    double v = std::strtod(p, &end);
    return (end != p) ? v : std::numeric_limits<double>::quiet_NaN();
}

bool JsonDatabase::writeJsonToFile(const std::string& filePath, const Json::Value& j) {
    std::ofstream f(filePath.c_str());
    if (!f.is_open()) return false;
    Json::StyledWriter writer;
    f << writer.write(j);
    return true;
}

bool JsonDatabase::readJsonFromFile(const std::string& filePath, Json::Value& j) {
    std::ifstream f(filePath.c_str());
    if (!f.is_open()) return false;
    Json::Reader reader;
    return reader.parse(f, j);
}

Json::Value JsonDatabase::pointDataToJson(const PointData& data) {
    Json::Value j;
    j["id"] = data.id;
    j["type"] = data.type.empty() ? "point" : data.type;
    j["meas_time"] = data.measTime;
    j["measurer"] = data.measurer;
    j["instrument_id"] = data.instrumentId;
    j["calculated"] = data.calculated;
    j["rms"] = data.rms;
    setJsonDouble(j, "x_dsg", data.x_dsg); setJsonDouble(j, "y_dsg", data.y_dsg); setJsonDouble(j, "z_dsg", data.z_dsg);
    setJsonDouble(j, "x_mea", data.x_mea); setJsonDouble(j, "y_mea", data.y_mea); setJsonDouble(j, "z_mea", data.z_mea);
    setJsonDouble(j, "x_scs", data.x_scs); setJsonDouble(j, "y_scs", data.y_scs); setJsonDouble(j, "z_scs", data.z_scs);
    setJsonDouble(j, "x_scs0", data.x_scs0); setJsonDouble(j, "y_scs0", data.y_scs0); setJsonDouble(j, "z_scs0", data.z_scs0);
    j["hor_angle"] = data.hor_angle; j["ver_angle"] = data.ver_angle; j["distance"] = data.distance;
    setJsonDouble(j, "precision", data.precision); setJsonDouble(j, "dev_x", data.dev_x); setJsonDouble(j, "dev_y", data.dev_y); setJsonDouble(j, "dev_z", data.dev_z);
    j["status"] = data.status; j["note"] = data.note;
    j["offset_x"] = data.offset_x; j["offset_y"] = data.offset_y; j["offset_z"] = data.offset_z;
    j["partial"] = data.partial; j["prism"] = data.prism; j["prism_constant"] = data.prism_constant;
    j["spherical_prism_correction"] = data.spherical_prism_correction;
    j["orientation_type"] = data.orientation_type; j["orientation_id"] = data.orientation_id;
    j["orientation_object_key"] = data.orientation_object_key; j["change_station_object_key"] = data.change_station_object_key;
    return j;
}

bool JsonDatabase::jsonToPointData(const Json::Value& j, PointData& data) {
    data.id = getJsonString(j, "id", "");
    data.type = getJsonString(j, "type", "point");
    data.measTime = getJsonString(j, "meas_time", "");
    data.measurer = getJsonString(j, "measurer", "");
    data.instrumentId = getJsonString(j, "instrument_id", "");
    data.calculated = getJsonBoolDefault(j, "calculated", false);
    data.rms = getJsonDoubleDefault(j, "rms", 0.0);
    data.x_dsg = getJsonDouble(j, "x_dsg"); data.y_dsg = getJsonDouble(j, "y_dsg"); data.z_dsg = getJsonDouble(j, "z_dsg");
    data.x_mea = getJsonDouble(j, "x_mea"); data.y_mea = getJsonDouble(j, "y_mea"); data.z_mea = getJsonDouble(j, "z_mea");
    data.x_scs = getJsonDouble(j, "x_scs"); data.y_scs = getJsonDouble(j, "y_scs"); data.z_scs = getJsonDouble(j, "z_scs");
    data.x_scs0 = getJsonDouble(j, "x_scs0"); data.y_scs0 = getJsonDouble(j, "y_scs0"); data.z_scs0 = getJsonDouble(j, "z_scs0");
    data.hor_angle = getJsonDoubleDefault(j, "hor_angle", 0.0); data.ver_angle = getJsonDoubleDefault(j, "ver_angle", 0.0); data.distance = getJsonDoubleDefault(j, "distance", 0.0);
    data.precision = getJsonDouble(j, "precision"); data.dev_x = getJsonDouble(j, "dev_x"); data.dev_y = getJsonDouble(j, "dev_y"); data.dev_z = getJsonDouble(j, "dev_z");
    data.status = getJsonIntDefault(j, "status", 0); data.note = getJsonString(j, "note", "");
    data.offset_x = getJsonDoubleDefault(j, "offset_x", 0.0); data.offset_y = getJsonDoubleDefault(j, "offset_y", 0.0); data.offset_z = getJsonDoubleDefault(j, "offset_z", 0.0);
    data.partial = getJsonBoolDefault(j, "partial", false); data.prism = getJsonString(j, "prism", ""); data.prism_constant = getJsonDoubleDefault(j, "prism_constant", 0.0);
    data.spherical_prism_correction = getJsonBoolDefault(j, "spherical_prism_correction", false);
    data.orientation_type = getJsonString(j, "orientation_type", ""); data.orientation_id = getJsonString(j, "orientation_id", "");
    data.orientation_object_key = getJsonIntDefault(j, "orientation_object_key", 0); data.change_station_object_key = getJsonIntDefault(j, "change_station_object_key", 0);
    return true;
}

Json::Value JsonDatabase::jobDataToJson(const JobData& data) {
    Json::Value j;
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
    j["active_orientation"]["type"] = data.active_orientation.type;
    j["active_orientation"]["id"] = data.active_orientation.id;
    j["active_orientation"]["object_key"] = data.active_orientation.object_key;
    Json::Value pointsJson;
    for (std::map<std::string, DCP_SHARED_PTR<PointData> >::const_iterator it = data.points.begin();
         it != data.points.end(); ++it) {
        if (it->second.get())
            pointsJson[it->first] = pointDataToJson(*it->second);
    }
    j["points"] = pointsJson;
    Json::Value circlesJson;
    for (std::map<std::string, DCP_SHARED_PTR<CircleData> >::const_iterator it = data.circlesData.begin();
         it != data.circlesData.end(); ++it) {
        if (it->second.get())
            circlesJson[it->first] = circleDataToJson(*it->second);
    }
    j["circles_data"] = circlesJson;
    Json::Value bestFitJson;
    for (std::map<std::string, DCP_SHARED_PTR<BestFitAlignmentData> >::const_iterator it = data.bestFitAlignments.begin();
         it != data.bestFitAlignments.end(); ++it) {
        if (it->second.get())
            bestFitJson[it->first] = bestFitAlignmentDataToJson(*it->second);
    }
    j["bestfit_alignments"] = bestFitJson;
    Json::Value changeStationsJson;
    for (std::map<std::string, DCP_SHARED_PTR<ChangeStationData> >::const_iterator it = data.changeStationsData.begin();
         it != data.changeStationsData.end(); ++it) {
        if (it->second.get())
            changeStationsJson[it->first] = changeStationDataToJson(*it->second);
    }
    j["change_stations_data"] = changeStationsJson;
    Json::Value align321Json;
    for (std::map<std::string, DCP_SHARED_PTR<Alignment321Data> >::const_iterator it = data.alignments321.begin();
         it != data.alignments321.end(); ++it) {
        if (it->second.get())
            align321Json[it->first] = alignment321DataToJson(*it->second);
    }
    j["alignments321"] = align321Json;
    return j;
}

Json::Value JsonDatabase::changeStationDataToJson(const ChangeStationData& data) {
    Json::Value j;
    j["type"] = data.type.empty() ? "change_station" : data.type;
    j["id"] = data.id;
    j["chst_hz_plane"] = data.chst_hz_plane;
    j["rms"] = data.rms;
    j["matrix"]["a00"] = data.matrix.a00; j["matrix"]["a01"] = data.matrix.a01; j["matrix"]["a02"] = data.matrix.a02; j["matrix"]["a03"] = data.matrix.a03;
    j["matrix"]["a10"] = data.matrix.a10; j["matrix"]["a11"] = data.matrix.a11; j["matrix"]["a12"] = data.matrix.a12; j["matrix"]["a13"] = data.matrix.a13;
    j["matrix"]["a20"] = data.matrix.a20; j["matrix"]["a21"] = data.matrix.a21; j["matrix"]["a22"] = data.matrix.a22; j["matrix"]["a23"] = data.matrix.a23;
    Json::Value ptsPrev;
    for (std::map<std::string, PointData>::const_iterator it = data.points_prev.begin(); it != data.points_prev.end(); ++it)
        ptsPrev[it->first] = pointDataToJson(it->second);
    j["points_prev"] = ptsPrev;
    Json::Value ptsNew;
    for (std::map<std::string, PointData>::const_iterator it = data.points_new.begin(); it != data.points_new.end(); ++it)
        ptsNew[it->first] = pointDataToJson(it->second);
    j["points_new"] = ptsNew;
    Json::Value ptsRes;
    for (std::map<std::string, PointData>::const_iterator it = data.points_residuals.begin(); it != data.points_residuals.end(); ++it)
        ptsRes[it->first] = pointDataToJson(it->second);
    j["points_residuals"] = ptsRes;
    return j;
}

bool JsonDatabase::jsonToChangeStationData(const Json::Value& j, ChangeStationData& data) {
    data.type = getJsonString(j, "type", "change_station");
    data.id = getJsonString(j, "id", "");
    data.chst_hz_plane = getJsonIntDefault(j, "chst_hz_plane", 0);
    data.rms = getJsonDoubleDefault(j, "rms", 0.0);
    if (j.isMember("matrix") && j["matrix"].isObject()) {
        data.matrix.a00 = getJsonDoubleDefault(j["matrix"], "a00", 1.0); data.matrix.a01 = getJsonDoubleDefault(j["matrix"], "a01", 0.0);
        data.matrix.a02 = getJsonDoubleDefault(j["matrix"], "a02", 0.0); data.matrix.a03 = getJsonDoubleDefault(j["matrix"], "a03", 0.0);
        data.matrix.a10 = getJsonDoubleDefault(j["matrix"], "a10", 0.0); data.matrix.a11 = getJsonDoubleDefault(j["matrix"], "a11", 1.0);
        data.matrix.a12 = getJsonDoubleDefault(j["matrix"], "a12", 0.0); data.matrix.a13 = getJsonDoubleDefault(j["matrix"], "a13", 0.0);
        data.matrix.a20 = getJsonDoubleDefault(j["matrix"], "a20", 0.0); data.matrix.a21 = getJsonDoubleDefault(j["matrix"], "a21", 0.0);
        data.matrix.a22 = getJsonDoubleDefault(j["matrix"], "a22", 1.0); data.matrix.a23 = getJsonDoubleDefault(j["matrix"], "a23", 0.0);
    }
    data.points_prev.clear();
    if (j.isMember("points_prev") && j["points_prev"].isObject()) {
        Json::Value::Members m = j["points_prev"].getMemberNames();
        for (Json::Value::Members::const_iterator it = m.begin(); it != m.end(); ++it) {
            PointData pt;
            if (jsonToPointData(j["points_prev"][*it], pt)) { pt.id = *it; data.points_prev[*it] = pt; }
        }
    }
    data.points_new.clear();
    if (j.isMember("points_new") && j["points_new"].isObject()) {
        Json::Value::Members m = j["points_new"].getMemberNames();
        for (Json::Value::Members::const_iterator it = m.begin(); it != m.end(); ++it) {
            PointData pt;
            if (jsonToPointData(j["points_new"][*it], pt)) { pt.id = *it; data.points_new[*it] = pt; }
        }
    }
    data.points_residuals.clear();
    if (j.isMember("points_residuals") && j["points_residuals"].isObject()) {
        Json::Value::Members m = j["points_residuals"].getMemberNames();
        for (Json::Value::Members::const_iterator it = m.begin(); it != m.end(); ++it) {
            PointData pt;
            if (jsonToPointData(j["points_residuals"][*it], pt)) { pt.id = *it; data.points_residuals[*it] = pt; }
        }
    }
    return true;
}

Json::Value JsonDatabase::alignment321DataToJson(const Alignment321Data& data) {
    Json::Value j;
    j["type"] = data.type.empty() ? "321" : data.type;
    j["id"] = data.id;
    j["calculated"] = data.calculated;
    setJsonDouble(j, "rms", data.rms);
    j["meas_time"] = data.measTime;
    j["measurer"] = data.measurer;
    j["instrument_id"] = data.instrumentId;
    Json::Value planeJson;
    planeJson["px"] = data.plane.px; planeJson["py"] = data.plane.py; planeJson["pz"] = data.plane.pz;
    planeJson["nx"] = data.plane.nx; planeJson["ny"] = data.plane.ny; planeJson["nz"] = data.plane.nz;
    planeJson["rms"] = data.plane.rms; planeJson["calculated"] = data.plane.calculated;
    Json::Value planePts;
    for (std::map<std::string, PointData>::const_iterator it = data.plane.points.begin(); it != data.plane.points.end(); ++it)
        planePts[it->first] = pointDataToJson(it->second);
    planeJson["points"] = planePts;
    j["plane"] = planeJson;
    Json::Value lineJson;
    lineJson["px"] = data.line.px; lineJson["py"] = data.line.py; lineJson["pz"] = data.line.pz;
    lineJson["ux"] = data.line.ux; lineJson["uy"] = data.line.uy; lineJson["uz"] = data.line.uz;
    lineJson["rms"] = data.line.rms; lineJson["calculated"] = data.line.calculated;
    Json::Value linePts;
    for (std::map<std::string, PointData>::const_iterator it = data.line.points.begin(); it != data.line.points.end(); ++it)
        linePts[it->first] = pointDataToJson(it->second);
    lineJson["points"] = linePts;
    j["line"] = lineJson;
    j["offset_value"] = pointDataToJson(data.offset_value);
    j["offset_value_tool"] = pointDataToJson(data.offset_value_tool);
    j["reference_point"] = pointDataToJson(data.reference_point);
    j["rotate_plane_buff"] = pointDataToJson(data.rotate_plane_buff);
    j["rotate_line_buff"] = pointDataToJson(data.rotate_line_buff);
    Json::Value rotPlane;
    for (std::map<std::string, Alignment321Data::Angle>::const_iterator it = data.rotate_plane.begin(); it != data.rotate_plane.end(); ++it) {
        Json::Value ang;
        ang["value"] = it->second.value;
        ang["calculated"] = it->second.calculated;
        rotPlane[it->first] = ang;
    }
    j["rotate_plane"] = rotPlane;
    Json::Value rotLine;
    rotLine["value"] = data.rotate_line.value;
    rotLine["calculated"] = data.rotate_line.calculated;
    j["rotate_line"] = rotLine;
    j["rotate_plane_order"] = data.rotate_plane_order;
    j["matrix"]["a00"] = data.matrix.a00; j["matrix"]["a01"] = data.matrix.a01; j["matrix"]["a02"] = data.matrix.a02; j["matrix"]["a03"] = data.matrix.a03;
    j["matrix"]["a10"] = data.matrix.a10; j["matrix"]["a11"] = data.matrix.a11; j["matrix"]["a12"] = data.matrix.a12; j["matrix"]["a13"] = data.matrix.a13;
    j["matrix"]["a20"] = data.matrix.a20; j["matrix"]["a21"] = data.matrix.a21; j["matrix"]["a22"] = data.matrix.a22; j["matrix"]["a23"] = data.matrix.a23;
    return j;
}

bool JsonDatabase::jsonToAlignment321Data(const Json::Value& j, Alignment321Data& data) {
    data.type = getJsonString(j, "type", "321");
    data.id = getJsonString(j, "id", "");
    data.calculated = getJsonBoolDefault(j, "calculated", false);
    data.rms = getJsonDoubleDefault(j, "rms", 0.0);
    data.measTime = getJsonString(j, "meas_time", "");
    data.measurer = getJsonString(j, "measurer", "");
    data.instrumentId = getJsonString(j, "instrument_id", "");
    if (j.isMember("plane") && j["plane"].isObject()) {
        const Json::Value& p = j["plane"];
        data.plane.px = getJsonDoubleDefault(p, "px", 0.0); data.plane.py = getJsonDoubleDefault(p, "py", 0.0); data.plane.pz = getJsonDoubleDefault(p, "pz", 0.0);
        data.plane.nx = getJsonDoubleDefault(p, "nx", 0.0); data.plane.ny = getJsonDoubleDefault(p, "ny", 0.0); data.plane.nz = getJsonDoubleDefault(p, "nz", 0.0);
        data.plane.rms = getJsonDoubleDefault(p, "rms", 0.0); data.plane.calculated = getJsonBoolDefault(p, "calculated", false);
        data.plane.points.clear();
        if (p.isMember("points") && p["points"].isObject()) {
            Json::Value::Members m = p["points"].getMemberNames();
            for (Json::Value::Members::const_iterator it = m.begin(); it != m.end(); ++it) {
                PointData pt;
                if (jsonToPointData(p["points"][*it], pt)) { pt.id = *it; data.plane.points[*it] = pt; }
            }
        }
    }
    if (j.isMember("line") && j["line"].isObject()) {
        const Json::Value& l = j["line"];
        data.line.px = getJsonDoubleDefault(l, "px", 0.0); data.line.py = getJsonDoubleDefault(l, "py", 0.0); data.line.pz = getJsonDoubleDefault(l, "pz", 0.0);
        data.line.ux = getJsonDoubleDefault(l, "ux", 0.0); data.line.uy = getJsonDoubleDefault(l, "uy", 0.0); data.line.uz = getJsonDoubleDefault(l, "uz", 0.0);
        data.line.rms = getJsonDoubleDefault(l, "rms", 0.0); data.line.calculated = getJsonBoolDefault(l, "calculated", false);
        data.line.points.clear();
        if (l.isMember("points") && l["points"].isObject()) {
            Json::Value::Members m = l["points"].getMemberNames();
            for (Json::Value::Members::const_iterator it = m.begin(); it != m.end(); ++it) {
                PointData pt;
                if (jsonToPointData(l["points"][*it], pt)) { pt.id = *it; data.line.points[*it] = pt; }
            }
        }
    }
    if (j.isMember("offset_value")) jsonToPointData(j["offset_value"], data.offset_value);
    if (j.isMember("offset_value_tool")) jsonToPointData(j["offset_value_tool"], data.offset_value_tool);
    if (j.isMember("reference_point")) jsonToPointData(j["reference_point"], data.reference_point);
    if (j.isMember("rotate_plane_buff")) jsonToPointData(j["rotate_plane_buff"], data.rotate_plane_buff);
    if (j.isMember("rotate_line_buff")) jsonToPointData(j["rotate_line_buff"], data.rotate_line_buff);
    data.rotate_plane.clear();
    if (j.isMember("rotate_plane") && j["rotate_plane"].isObject()) {
        Json::Value::Members m = j["rotate_plane"].getMemberNames();
        for (Json::Value::Members::const_iterator it = m.begin(); it != m.end(); ++it) {
            Alignment321Data::Angle ang;
            ang.value = getJsonDoubleDefault(j["rotate_plane"][*it], "value", 0.0);
            ang.calculated = getJsonBoolDefault(j["rotate_plane"][*it], "calculated", false);
            data.rotate_plane[*it] = ang;
        }
    }
    if (j.isMember("rotate_line") && j["rotate_line"].isObject()) {
        data.rotate_line.value = getJsonDoubleDefault(j["rotate_line"], "value", 0.0);
        data.rotate_line.calculated = getJsonBoolDefault(j["rotate_line"], "calculated", false);
    }
    data.rotate_plane_order = getJsonIntDefault(j, "rotate_plane_order", 0);
    if (j.isMember("matrix") && j["matrix"].isObject()) {
        const Json::Value& mat = j["matrix"];
        data.matrix.a00 = getJsonDoubleDefault(mat, "a00", 1.0); data.matrix.a01 = getJsonDoubleDefault(mat, "a01", 0.0);
        data.matrix.a02 = getJsonDoubleDefault(mat, "a02", 0.0); data.matrix.a03 = getJsonDoubleDefault(mat, "a03", 0.0);
        data.matrix.a10 = getJsonDoubleDefault(mat, "a10", 0.0); data.matrix.a11 = getJsonDoubleDefault(mat, "a11", 1.0);
        data.matrix.a12 = getJsonDoubleDefault(mat, "a12", 0.0); data.matrix.a13 = getJsonDoubleDefault(mat, "a13", 0.0);
        data.matrix.a20 = getJsonDoubleDefault(mat, "a20", 0.0); data.matrix.a21 = getJsonDoubleDefault(mat, "a21", 0.0);
        data.matrix.a22 = getJsonDoubleDefault(mat, "a22", 1.0); data.matrix.a23 = getJsonDoubleDefault(mat, "a23", 0.0);
    }
    return true;
}

Json::Value JsonDatabase::bestFitAlignmentDataToJson(const BestFitAlignmentData& data) {
    Json::Value j;
    j["type"] = data.type.empty() ? "bestfit" : data.type;
    j["id"] = data.id;
    j["bestFitAlignmentType"] = data.bestFitAlignmentType;
    setJsonDouble(j, "position_x", data.position_x); setJsonDouble(j, "position_y", data.position_y); setJsonDouble(j, "position_z", data.position_z);
    setJsonDouble(j, "orientation_x", data.orientation_x); setJsonDouble(j, "orientation_y", data.orientation_y); setJsonDouble(j, "orientation_z", data.orientation_z);
    setJsonDouble(j, "tolerance", data.tolerance); setJsonDouble(j, "maxDeviation", data.maxDeviation);
    j["rms"] = data.rms; j["calculated"] = data.calculated;
    j["matrix"]["a00"] = data.matrix.a00; j["matrix"]["a01"] = data.matrix.a01; j["matrix"]["a02"] = data.matrix.a02; j["matrix"]["a03"] = data.matrix.a03;
    j["matrix"]["a10"] = data.matrix.a10; j["matrix"]["a11"] = data.matrix.a11; j["matrix"]["a12"] = data.matrix.a12; j["matrix"]["a13"] = data.matrix.a13;
    j["matrix"]["a20"] = data.matrix.a20; j["matrix"]["a21"] = data.matrix.a21; j["matrix"]["a22"] = data.matrix.a22; j["matrix"]["a23"] = data.matrix.a23;
    Json::Value ptsOcs;
    for (std::map<std::string, PointData>::const_iterator it = data.points_ocs.begin(); it != data.points_ocs.end(); ++it)
        ptsOcs[it->first] = pointDataToJson(it->second);
    j["points_ocs"] = ptsOcs;
    Json::Value ptsScs;
    for (std::map<std::string, PointData>::const_iterator it = data.points_scs.begin(); it != data.points_scs.end(); ++it)
        ptsScs[it->first] = pointDataToJson(it->second);
    j["points_scs"] = ptsScs;
    Json::Value ptsRes;
    for (std::map<std::string, PointData>::const_iterator it = data.points_residuals.begin(); it != data.points_residuals.end(); ++it)
        ptsRes[it->first] = pointDataToJson(it->second);
    j["points_residuals"] = ptsRes;
    return j;
}

bool JsonDatabase::jsonToBestFitAlignmentData(const Json::Value& j, BestFitAlignmentData& data) {
    data.type = getJsonString(j, "type", "bestfit");
    data.id = getJsonString(j, "id", "");
    data.bestFitAlignmentType = getJsonString(j, "bestFitAlignmentType", "");
    data.position_x = getJsonDoubleDefault(j, "position_x", 0.0); data.position_y = getJsonDoubleDefault(j, "position_y", 0.0); data.position_z = getJsonDoubleDefault(j, "position_z", 0.0);
    data.orientation_x = getJsonDoubleDefault(j, "orientation_x", 0.0); data.orientation_y = getJsonDoubleDefault(j, "orientation_y", 0.0); data.orientation_z = getJsonDoubleDefault(j, "orientation_z", 0.0);
    data.tolerance = getJsonDoubleDefault(j, "tolerance", 0.0); data.maxDeviation = getJsonDoubleDefault(j, "maxDeviation", 0.0);
    data.rms = getJsonDoubleDefault(j, "rms", 0.0); data.calculated = getJsonBoolDefault(j, "calculated", false);
    if (j.isMember("matrix") && j["matrix"].isObject()) {
        data.matrix.a00 = getJsonDoubleDefault(j["matrix"], "a00", 1.0); data.matrix.a01 = getJsonDoubleDefault(j["matrix"], "a01", 0.0);
        data.matrix.a02 = getJsonDoubleDefault(j["matrix"], "a02", 0.0); data.matrix.a03 = getJsonDoubleDefault(j["matrix"], "a03", 0.0);
        data.matrix.a10 = getJsonDoubleDefault(j["matrix"], "a10", 0.0); data.matrix.a11 = getJsonDoubleDefault(j["matrix"], "a11", 1.0);
        data.matrix.a12 = getJsonDoubleDefault(j["matrix"], "a12", 0.0); data.matrix.a13 = getJsonDoubleDefault(j["matrix"], "a13", 0.0);
        data.matrix.a20 = getJsonDoubleDefault(j["matrix"], "a20", 0.0); data.matrix.a21 = getJsonDoubleDefault(j["matrix"], "a21", 0.0);
        data.matrix.a22 = getJsonDoubleDefault(j["matrix"], "a22", 1.0); data.matrix.a23 = getJsonDoubleDefault(j["matrix"], "a23", 0.0);
    }
    data.points_ocs.clear();
    if (j.isMember("points_ocs") && j["points_ocs"].isObject()) {
        Json::Value::Members m = j["points_ocs"].getMemberNames();
        for (Json::Value::Members::const_iterator it = m.begin(); it != m.end(); ++it) {
            PointData pt;
            if (jsonToPointData(j["points_ocs"][*it], pt)) { pt.id = *it; data.points_ocs[*it] = pt; }
        }
    }
    data.points_scs.clear();
    if (j.isMember("points_scs") && j["points_scs"].isObject()) {
        Json::Value::Members m = j["points_scs"].getMemberNames();
        for (Json::Value::Members::const_iterator it = m.begin(); it != m.end(); ++it) {
            PointData pt;
            if (jsonToPointData(j["points_scs"][*it], pt)) { pt.id = *it; data.points_scs[*it] = pt; }
        }
    }
    data.points_residuals.clear();
    if (j.isMember("points_residuals") && j["points_residuals"].isObject()) {
        Json::Value::Members m = j["points_residuals"].getMemberNames();
        for (Json::Value::Members::const_iterator it = m.begin(); it != m.end(); ++it) {
            PointData pt;
            if (jsonToPointData(j["points_residuals"][*it], pt)) { pt.id = *it; data.points_residuals[*it] = pt; }
        }
    }
    return true;
}

Json::Value JsonDatabase::circleDataToJson(const CircleData& data) {
    Json::Value j;
    j["type"] = data.type;
    j["id"] = data.id;
    j["planeType"] = data.planeType;
    setJsonDouble(j, "center_x", data.center_x);
    setJsonDouble(j, "center_y", data.center_y);
    setJsonDouble(j, "center_z", data.center_z);
    setJsonDouble(j, "normal_x", data.normal_x);
    setJsonDouble(j, "normal_y", data.normal_y);
    setJsonDouble(j, "normal_z", data.normal_z);
    setJsonDouble(j, "diameter", data.diameter);
    j["toolRadius"] = data.toolRadius;
    j["calculated"] = data.calculated;
    j["rms"] = data.rms;
    return j;
}

bool JsonDatabase::jsonToCircleData(const Json::Value& j, CircleData& data) {
    data.type = getJsonString(j, "type", "circle");
    data.id = getJsonString(j, "id", "");
    data.planeType = getJsonString(j, "planeType", "");
    data.center_x = getJsonDoubleDefault(j, "center_x", 0.0);
    data.center_y = getJsonDoubleDefault(j, "center_y", 0.0);
    data.center_z = getJsonDoubleDefault(j, "center_z", 0.0);
    data.normal_x = getJsonDoubleDefault(j, "normal_x", 0.0);
    data.normal_y = getJsonDoubleDefault(j, "normal_y", 0.0);
    data.normal_z = getJsonDoubleDefault(j, "normal_z", 0.0);
    data.diameter = getJsonDoubleDefault(j, "diameter", 0.0);
    data.toolRadius = getJsonDoubleDefault(j, "toolRadius", 0.0);
    data.calculated = getJsonBoolDefault(j, "calculated", false);
    data.rms = getJsonDoubleDefault(j, "rms", 0.0);
    return true;
}

bool JsonDatabase::jsonToJobData(const Json::Value& j, JobData& data) {
    data.type = getJsonString(j, "type", "job"); data.object_key = getJsonIntDefault(j, "object_key", 1);
    data.id = getJsonString(j, "id", ""); data.date = getJsonString(j, "date", "");
    data.measurer = getJsonString(j, "measurer", ""); data.instrument_id = getJsonString(j, "instrument_id", "");
    data.units = getJsonString(j, "units", "mm"); data.active_change_station = getJsonString(j, "active_change_station", "");
    data.target = getJsonString(j, "target", ""); data.decimals = getJsonIntDefault(j, "decimals", 2);
    data.temperature = getJsonDoubleDefault(j, "temperature", 0.0); data.pressure = getJsonDoubleDefault(j, "pressure", 0.0); data.humidity = getJsonDoubleDefault(j, "humidity", 0.0);
    data.remote = getJsonString(j, "remote", "yes"); data.flow_time = getJsonDoubleDefault(j, "flow_time", 2.0); data.flow_move = getJsonDoubleDefault(j, "flow_move", 2.0);
    data.overwrite_info = getJsonString(j, "overwrite_info", "yes"); data.auto_increment = getJsonString(j, "auto_increment", "no");
    data.save_tool_settings = getJsonString(j, "save_tool_settings", "yes"); data.tool_info = getJsonString(j, "tool_info", "yes");
    data.inclination_threshold = getJsonDoubleDefault(j, "inclination_threshold", 0.015); data.default_folder = getJsonString(j, "default_folder", "");
    data.simulator = getJsonString(j, "simulator", "no");
    data.show_design_coords = getJsonBoolDefault(j, "show_design_coords", true);
    data.show_deviation_coords = getJsonBoolDefault(j, "show_deviation_coords", true);
    data.show_tool_offset = getJsonBoolDefault(j, "show_tool_offset", true);
    if (j.isMember("active_orientation")) {
        data.active_orientation.type = getJsonString(j["active_orientation"], "type", "?");
        data.active_orientation.id = getJsonString(j["active_orientation"], "id", "SCS");
        data.active_orientation.object_key = getJsonIntDefault(j["active_orientation"], "object_key", 0);
    }
    data.points.clear();
    if (j.isMember("points") && j["points"].isObject()) {
        const Json::Value& pts = j["points"];
        Json::Value::Members members = pts.getMemberNames();
        for (Json::Value::Members::const_iterator it = members.begin(); it != members.end(); ++it) {
            const std::string& key = *it;
            DCP_SHARED_PTR<PointData> pt(new PointData());
            if (jsonToPointData(pts[key], *pt))
                data.points[key] = pt;
        }
    }
    data.circlesData.clear();
    if (j.isMember("circles_data") && j["circles_data"].isObject()) {
        const Json::Value& circs = j["circles_data"];
        Json::Value::Members members = circs.getMemberNames();
        for (Json::Value::Members::const_iterator it = members.begin(); it != members.end(); ++it) {
            const std::string& key = *it;
            DCP_SHARED_PTR<CircleData> cd(new CircleData());
            if (jsonToCircleData(circs[key], *cd))
                data.circlesData[key] = cd;
        }
    }
    data.bestFitAlignments.clear();
    if (j.isMember("bestfit_alignments") && j["bestfit_alignments"].isObject()) {
        const Json::Value& bf = j["bestfit_alignments"];
        Json::Value::Members members = bf.getMemberNames();
        for (Json::Value::Members::const_iterator it = members.begin(); it != members.end(); ++it) {
            const std::string& key = *it;
            DCP_SHARED_PTR<BestFitAlignmentData> bfd(new BestFitAlignmentData());
            if (jsonToBestFitAlignmentData(bf[key], *bfd))
                data.bestFitAlignments[key] = bfd;
        }
    }
    data.changeStationsData.clear();
    if (j.isMember("change_stations_data") && j["change_stations_data"].isObject()) {
        const Json::Value& cs = j["change_stations_data"];
        Json::Value::Members members = cs.getMemberNames();
        for (Json::Value::Members::const_iterator it = members.begin(); it != members.end(); ++it) {
            const std::string& key = *it;
            DCP_SHARED_PTR<ChangeStationData> csd(new ChangeStationData());
            if (jsonToChangeStationData(cs[key], *csd))
                data.changeStationsData[key] = csd;
        }
    }
    data.alignments321.clear();
    if (j.isMember("alignments321") && j["alignments321"].isObject()) {
        const Json::Value& a321 = j["alignments321"];
        Json::Value::Members members = a321.getMemberNames();
        for (Json::Value::Members::const_iterator it = members.begin(); it != members.end(); ++it) {
            const std::string& key = *it;
            DCP_SHARED_PTR<Alignment321Data> a321d(new Alignment321Data());
            if (jsonToAlignment321Data(a321[key], *a321d))
                data.alignments321[key] = a321d;
        }
    }
    return true;
}

bool JsonDatabase::createJob(const std::string& jobId) {
    DCP06_TRACE_ENTER;
    m_currentJob = DCP_UNIQUE_PTR<JobData>(new JobData());
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
    DCP06_TRACE_POINT("jobId=%s", m_currentJob->id.c_str());
    DCP06_TRACE_EXIT;
    return true;
}

bool JsonDatabase::loadJob(const std::string& jobId) {
    DCP06_TRACE_ENTER;
    Json::Value j;
    if (!readJsonFromFile(getJobFilePath(jobId), j)) { DCP06_TRACE_EXIT; return false; }
    m_currentJob = DCP_UNIQUE_PTR<JobData>(new JobData());
    if (!jsonToJobData(j, *m_currentJob)) {
        m_currentJob.reset();
        DCP06_TRACE_EXIT; return false;
    }
    m_isJobLoaded = true;
    DCP06_TRACE_POINT("jobId=%s pts=%d", jobId.c_str(), (int)m_currentJob->points.size());
    DCP06_TRACE_EXIT;
    return true;
}

bool JsonDatabase::saveJob(const std::string& jobId) {
    DCP06_TRACE_ENTER;
    if (!m_isJobLoaded || !m_currentJob.get()) { DCP06_TRACE_EXIT; return false; }
    bool ok = writeJsonToFile(getJobFilePath(jobId), jobDataToJson(*m_currentJob));
    DCP06_TRACE_POINT("jobId=%s ok=%d", jobId.c_str(), ok);
    DCP06_TRACE_EXIT;
    return ok;
}

void JsonDatabase::closeJob() {
    DCP06_TRACE_ENTER;
    m_currentJob.reset();
    m_isJobLoaded = false;
    DCP06_TRACE_EXIT;
}

bool JsonDatabase::deleteJob(const std::string& jobId) {
    DCP06_TRACE_ENTER;
    std::string path = getJobFilePath(jobId);
    if (!boost::filesystem::exists(path)) { DCP06_TRACE_EXIT; return true; }
    boost::system::error_code ec;
    bool ok = boost::filesystem::remove(path, ec);
    DCP06_TRACE_POINT("jobId=%s ok=%d", jobId.c_str(), ok);
    DCP06_TRACE_EXIT;
    return ok;
}

bool JsonDatabase::copyJob(const std::string& sourceId, const std::string& targetId) {
    DCP06_TRACE_ENTER;
    Json::Value j;
    if (!readJsonFromFile(getJobFilePath(sourceId), j)) { DCP06_TRACE_EXIT; return false; }
    j["id"] = targetId;
    bool ok = writeJsonToFile(getJobFilePath(targetId), j);
    DCP06_TRACE_POINT("%s->%s ok=%d", sourceId.c_str(), targetId.c_str(), ok);
    DCP06_TRACE_EXIT;
    return ok;
}

bool JsonDatabase::swapJob(const std::string& jobId1, const std::string& jobId2) {
    DCP06_TRACE_ENTER;
    Json::Value j1, j2;
    if (!readJsonFromFile(getJobFilePath(jobId1), j1) || !readJsonFromFile(getJobFilePath(jobId2), j2))
        { DCP06_TRACE_EXIT; return false; }
    std::string t = j1["id"].asString();
    j1["id"] = jobId2;
    j2["id"] = t;
    bool ok = writeJsonToFile(getJobFilePath(jobId1), j2) && writeJsonToFile(getJobFilePath(jobId2), j1);
    DCP06_TRACE_EXIT;
    return ok;
}

bool JsonDatabase::addPoint(const std::string& pointId, const PointData& data) {
    if (!m_currentJob.get() || (int)m_currentJob->points.size() >= MAX_POINTS_PER_JOB) return false;
    DCP_SHARED_PTR<PointData> pt(new PointData(data));
    pt->id = pointId;
    if (pt->measTime.empty()) pt->measTime = getCurrentIsoTime();
    m_currentJob->points[pointId] = pt;
    return true;
}

bool JsonDatabase::updatePoint(const std::string& pointId, const PointData& data) {
    if (!m_currentJob.get()) return false;
    std::map<std::string, DCP_SHARED_PTR<PointData> >::iterator it = m_currentJob->points.find(pointId);
    if (it == m_currentJob->points.end()) return false;
    *it->second = data;
    it->second->id = pointId;
    return true;
}

bool JsonDatabase::deletePoint(const std::string& pointId) {
    if (!m_currentJob.get()) return false;
    return m_currentJob->points.erase(pointId) > 0;
}

bool JsonDatabase::getPoint(const std::string& pointId, PointData& data) const {
    if (!m_currentJob.get()) return false;
    std::map<std::string, DCP_SHARED_PTR<PointData> >::const_iterator it = m_currentJob->points.find(pointId);
    if (it == m_currentJob->points.end() || !it->second) return false;
    data = *it->second;
    return true;
}

bool JsonDatabase::addCircle(const std::string& circleId, const CircleData& data) {
    if (!m_currentJob.get()) return false;
    DCP_SHARED_PTR<CircleData> cd(new CircleData(data));
    cd->id = circleId;
    if (cd->measTime.empty()) cd->measTime = getCurrentIsoTime();
    m_currentJob->circlesData[circleId] = cd;
    return true;
}

bool JsonDatabase::updateCircle(const std::string& circleId, const CircleData& data) {
    if (!m_currentJob.get()) return false;
    std::map<std::string, DCP_SHARED_PTR<CircleData> >::iterator it = m_currentJob->circlesData.find(circleId);
    if (it == m_currentJob->circlesData.end()) return false;
    *it->second = data;
    it->second->id = circleId;
    return true;
}

bool JsonDatabase::deleteCircle(const std::string& circleId) {
    if (!m_currentJob.get()) return false;
    return m_currentJob->circlesData.erase(circleId) > 0;
}

bool JsonDatabase::getCircle(const std::string& circleId, CircleData& data) {
    if (!m_currentJob.get()) return false;
    std::map<std::string, DCP_SHARED_PTR<CircleData> >::const_iterator it = m_currentJob->circlesData.find(circleId);
    if (it == m_currentJob->circlesData.end() || !it->second) return false;
    data = *it->second;
    return true;
}

std::vector<std::string> JsonDatabase::getCircleIdsInJob() const {
    std::vector<std::string> out;
    if (!m_currentJob.get()) return out;
    for (std::map<std::string, DCP_SHARED_PTR<CircleData> >::const_iterator it = m_currentJob->circlesData.begin();
         it != m_currentJob->circlesData.end(); ++it)
        out.push_back(it->first);
    std::sort(out.begin(), out.end());
    return out;
}

bool JsonDatabase::addBestFitAlignment(const std::string& bestFitAlignmentId, const BestFitAlignmentData& data) {
    if (!m_currentJob.get()) return false;
    DCP_SHARED_PTR<BestFitAlignmentData> bfd(new BestFitAlignmentData(data));
    bfd->id = bestFitAlignmentId;
    if (bfd->measTime.empty()) bfd->measTime = getCurrentIsoTime();
    m_currentJob->bestFitAlignments[bestFitAlignmentId] = bfd;
    return true;
}

bool JsonDatabase::updateBestFitAlignment(const std::string& bestFitAlignmentId, const BestFitAlignmentData& data) {
    if (!m_currentJob.get()) return false;
    std::map<std::string, DCP_SHARED_PTR<BestFitAlignmentData> >::iterator it = m_currentJob->bestFitAlignments.find(bestFitAlignmentId);
    if (it == m_currentJob->bestFitAlignments.end()) return false;
    *it->second = data;
    it->second->id = bestFitAlignmentId;
    return true;
}

bool JsonDatabase::deleteBestFitAlignment(const std::string& bestFitAlignmentId) {
    if (!m_currentJob.get()) return false;
    return m_currentJob->bestFitAlignments.erase(bestFitAlignmentId) > 0;
}

bool JsonDatabase::getBestFitAlignment(const std::string& bestFitAlignmentId, BestFitAlignmentData& data) {
    if (!m_currentJob.get()) return false;
    std::map<std::string, DCP_SHARED_PTR<BestFitAlignmentData> >::const_iterator it = m_currentJob->bestFitAlignments.find(bestFitAlignmentId);
    if (it == m_currentJob->bestFitAlignments.end() || !it->second) return false;
    data = *it->second;
    return true;
}

bool JsonDatabase::addChangeStation(const std::string& changeStationId, const ChangeStationData& data) {
    if (!m_currentJob.get()) return false;
    DCP_SHARED_PTR<ChangeStationData> csd(new ChangeStationData(data));
    csd->id = changeStationId;
    if (csd->measTime.empty()) csd->measTime = getCurrentIsoTime();
    m_currentJob->changeStationsData[changeStationId] = csd;
    return true;
}

bool JsonDatabase::updateChangeStation(const std::string& changeStationId, const ChangeStationData& data) {
    if (!m_currentJob.get()) return false;
    std::map<std::string, DCP_SHARED_PTR<ChangeStationData> >::iterator it = m_currentJob->changeStationsData.find(changeStationId);
    if (it == m_currentJob->changeStationsData.end()) return false;
    *it->second = data;
    it->second->id = changeStationId;
    return true;
}

bool JsonDatabase::deleteChangeStation(const std::string& changeStationId) {
    if (!m_currentJob.get()) return false;
    return m_currentJob->changeStationsData.erase(changeStationId) > 0;
}

bool JsonDatabase::getChangeStation(const std::string& changeStationId, ChangeStationData& data) {
    if (!m_currentJob.get()) return false;
    std::map<std::string, DCP_SHARED_PTR<ChangeStationData> >::const_iterator it = m_currentJob->changeStationsData.find(changeStationId);
    if (it == m_currentJob->changeStationsData.end() || !it->second) return false;
    data = *it->second;
    return true;
}

bool JsonDatabase::add321Alignment(const std::string& alignment321Id, const Alignment321Data& data) {
    if (!m_currentJob.get()) return false;
    DCP_SHARED_PTR<Alignment321Data> a321(new Alignment321Data(data));
    a321->id = alignment321Id;
    if (a321->measTime.empty()) a321->measTime = getCurrentIsoTime();
    m_currentJob->alignments321[alignment321Id] = a321;
    return true;
}

bool JsonDatabase::update321Alignment(const std::string& alignment321Id, const Alignment321Data& data) {
    if (!m_currentJob.get()) return false;
    std::map<std::string, DCP_SHARED_PTR<Alignment321Data> >::iterator it = m_currentJob->alignments321.find(alignment321Id);
    if (it == m_currentJob->alignments321.end()) return false;
    *it->second = data;
    it->second->id = alignment321Id;
    return true;
}

bool JsonDatabase::delete321Alignment(const std::string& alignment321Id) {
    if (!m_currentJob.get()) return false;
    return m_currentJob->alignments321.erase(alignment321Id) > 0;
}

bool JsonDatabase::get321Alignment(const std::string& alignment321Id, Alignment321Data& data) {
    if (!m_currentJob.get()) return false;
    std::map<std::string, DCP_SHARED_PTR<Alignment321Data> >::const_iterator it = m_currentJob->alignments321.find(alignment321Id);
    if (it == m_currentJob->alignments321.end() || !it->second) return false;
    data = *it->second;
    return true;
}

std::vector<std::string> JsonDatabase::getAllAlignment321Ids() const {
    std::vector<std::string> out;
    if (!m_currentJob.get()) return out;
    for (std::map<std::string, DCP_SHARED_PTR<Alignment321Data> >::const_iterator it = m_currentJob->alignments321.begin();
         it != m_currentJob->alignments321.end(); ++it)
        out.push_back(it->first);
    return out;
}

std::vector<DCP_SHARED_PTR<PointData> > JsonDatabase::getAllPoints() const {
    std::vector<DCP_SHARED_PTR<PointData> > out;
    if (!m_currentJob.get()) return out;
    for (std::map<std::string, DCP_SHARED_PTR<PointData> >::const_iterator it = m_currentJob->points.begin();
         it != m_currentJob->points.end(); ++it) {
        if (it->second.get()) out.push_back(it->second);
    }
    return out;
}

std::vector<DCP_SHARED_PTR<PointData> > JsonDatabase::getAllPointsInJob() const {
    std::vector<DCP_SHARED_PTR<PointData> > out;
    if (!m_currentJob.get()) return out;
    std::vector<std::pair<int, DCP_SHARED_PTR<PointData> > > sorted;
    for (std::map<std::string, DCP_SHARED_PTR<PointData> >::const_iterator it = m_currentJob->points.begin();
         it != m_currentJob->points.end(); ++it) {
        if (!it->second.get()) continue;
        // Include all points (including new ones with no actual/design yet) so they appear in the list
        sorted.push_back(std::make_pair(pointIdSortKey(it->first), it->second));
    }
    std::sort(sorted.begin(), sorted.end(), SortByPointId());
    for (size_t i = 0; i < sorted.size(); ++i)
        out.push_back(sorted[i].second);
    return out;
}

short JsonDatabase::getPointListAsSelectPoints(S_SELECT_POINTS* pList, short iMaxPoints, short iDef) const {
    std::vector<DCP_SHARED_PTR<PointData> > pts = getAllPointsInJob();
    short iCount = 0;
    for (size_t i = 0; i < pts.size() && iCount < iMaxPoints; i++) {
        const DCP_SHARED_PTR<PointData>& pt = pts[i];
        if (!pt) continue;
        bool mea = pointHasActualValues(*pt);
        bool des = pointHasDesignValues(*pt);
        pList[iCount].iId = static_cast<short>(i + 1);
        std::string id = pt->id.size() <= (size_t)DCP_POINT_ID_LENGTH ? pt->id : pt->id.substr(0, DCP_POINT_ID_LENGTH);
        snprintf(pList[iCount].point_id, sizeof(pList[iCount].point_id), DCP_POINT_ID_FMT, id.c_str());
        pList[iCount].bActualDefined = mea;
        pList[iCount].bDesignDefined = des;
        pList[iCount].bPointSelected = false;
        if (iDef == BOTH) {
            pList[iCount].bDesignSelected = des;
            pList[iCount].bActualSelected = mea;
        } else if (iDef == ACTUAL) {
            pList[iCount].bDesignSelected = false;
            pList[iCount].bActualSelected = mea;
            if (!pList[iCount].bActualSelected)
                pList[iCount].bDesignSelected = des;
        } else {
            pList[iCount].bDesignSelected = des;
            pList[iCount].bActualSelected = false;
            if (!pList[iCount].bDesignSelected)
                pList[iCount].bActualSelected = mea;
        }
        iCount++;
    }
    return iCount;
}

short JsonDatabase::getPointListAsSelectPoint(S_SELECT_POINT* pList, short iMaxPoints) const {
    std::vector<DCP_SHARED_PTR<PointData> > pts = getAllPointsInJob();
    short iCount = 0;
    for (size_t i = 0; i < pts.size() && iCount < iMaxPoints; i++) {
        const DCP_SHARED_PTR<PointData>& pt = pts[i];
        if (!pt) continue;
        pList[iCount].no = static_cast<short>(i + 1);
        std::string id = pt->id.size() <= (size_t)DCP_POINT_ID_LENGTH ? pt->id : pt->id.substr(0, DCP_POINT_ID_LENGTH);
        snprintf(pList[iCount].point_id, sizeof(pList[iCount].point_id), DCP_POINT_ID_FMT, id.c_str());
        bool mea = pointHasActualValues(*pt);
        bool des = pointHasDesignValues(*pt);
        sprintf(pList[iCount].point_status, "%s/%s", mea ? "A" : "-", des ? "D" : "-");
        iCount++;
    }
    return iCount;
}

short JsonDatabase::getCircleListAsSelectCircle(S_SELECT_CIRCLE* pList, short iMaxCircles) const {
    if (!m_currentJob.get()) return 0;
    short iCount = 0;
    for (std::map<std::string, DCP_SHARED_PTR<CircleData> >::const_iterator it = m_currentJob->circlesData.begin();
         it != m_currentJob->circlesData.end() && iCount < iMaxCircles; ++it) {
        if (!it->second) continue;
        const CircleData& data = *it->second;
        pList[iCount].no = static_cast<short>(iCount + 1);
        std::string id = data.id.size() <= (size_t)(CIRCLE_ID_BUFF_LEN - 1) ? data.id : data.id.substr(0, CIRCLE_ID_BUFF_LEN - 1);
        snprintf(pList[iCount].circle_id, sizeof(pList[iCount].circle_id), "%.*s", (int)(CIRCLE_ID_BUFF_LEN - 1), id.c_str());
        if (DCP_ISNAN(data.center_x) || DCP_ISNAN(data.center_y) || DCP_ISNAN(data.center_z) || !data.calculated)
            snprintf(pList[iCount].center, sizeof(pList[iCount].center), "-");
        else
            snprintf(pList[iCount].center, sizeof(pList[iCount].center), "%.1f %.1f %.1f", data.center_x, data.center_y, data.center_z);
        if (DCP_ISNAN(data.diameter) || data.diameter == 0.0)
            snprintf(pList[iCount].diameter, sizeof(pList[iCount].diameter), "-");
        else
            snprintf(pList[iCount].diameter, sizeof(pList[iCount].diameter), "%.3f", data.diameter);
        iCount++;
    }
    return iCount;
}

static void fmtDouble(double v, char* buf) {
    if (buf) {
        if (DCP_ISNAN(v))
            buf[0] = '\0';  // empty string for "no value yet"
        else
            sprintf(buf, "%9.3f", v);
    }
}

bool JsonDatabase::getPointByIndex(int index1Based, bool useActual, char* pid,
    char* xact, char* xdes, char* yact, char* ydes, char* zact, char* zdes, char* note) const {
    (void)useActual;
    std::vector<DCP_SHARED_PTR<PointData> > pts = getAllPointsInJob();
    if (index1Based < 1 || index1Based > static_cast<int>(pts.size())) return false;
    const DCP_SHARED_PTR<PointData>& pt = pts[index1Based - 1];
    if (!pt) return false;
    if (pid) {
        snprintf(pid, POINT_ID_BUFF_LEN, DCP_POINT_ID_FMT, pt->id.c_str());
    }
    if (xact) fmtDouble(pt->x_mea, xact); if (xdes) fmtDouble(pt->x_dsg, xdes);
    if (yact) fmtDouble(pt->y_mea, yact); if (ydes) fmtDouble(pt->y_dsg, ydes);
    if (zact) fmtDouble(pt->z_mea, zact); if (zdes) fmtDouble(pt->z_dsg, zdes);
    if (note) {
        std::string n = pt->note.size() <= 6 ? pt->note : pt->note.substr(0, 6);
        sprintf(note, "%-6.6s", n.c_str());
    }
    return true;
}

std::string JsonDatabase::getJobDisplayName() const {
    if (!m_currentJob.get()) return "";
    return m_currentJob->id;
}

int JsonDatabase::getJobPointsCount() const {
    if (!m_currentJob.get()) return 0;
    return static_cast<int>(m_currentJob->points.size());
}

std::string JsonDatabase::getJobFileSizeString() const {
    if (!m_currentJob.get()) return "";
    std::string path = getJobFilePath(m_currentJob->id);
    boost::system::error_code ec;
    if (!boost::filesystem::exists(path, ec)) return "";
    boost::uintmax_t sz = boost::filesystem::file_size(path, ec);
    if (ec) return "";
    char buf[32];
    sprintf(buf, "%u", static_cast<unsigned>(sz));
    return buf;
}

std::string JsonDatabase::getJobModDateString() const {
    if (!m_currentJob.get()) return "";
    std::string path = getJobFilePath(m_currentJob->id);
    boost::system::error_code ec;
    if (!boost::filesystem::exists(path, ec)) return "";
    std::time_t t = boost::filesystem::last_write_time(path, ec);
    if (ec) return "";
    struct tm buf;
#ifdef _WIN32
    if (localtime_s(&buf, &t) != 0) return "";
#else
    if (!localtime_r(&t, &buf)) return "";
#endif
    char out[16];
    std::strftime(out, sizeof(out), "%Y-%m-%d", &buf);
    return std::string(out);
}

std::string JsonDatabase::getJobModTimeString() const {
    if (!m_currentJob.get()) return "";
    std::string path = getJobFilePath(m_currentJob->id);
    boost::system::error_code ec;
    if (!boost::filesystem::exists(path, ec)) return "";
    std::time_t t = boost::filesystem::last_write_time(path, ec);
    if (ec) return "";
    struct tm buf;
#ifdef _WIN32
    if (localtime_s(&buf, &t) != 0) return "";
#else
    if (!localtime_r(&t, &buf)) return "";
#endif
    char out[16];
    std::strftime(out, sizeof(out), "%H:%M:%S", &buf);
    return std::string(out);
}

bool JsonDatabase::swapMeasDesign() {
    if (!m_currentJob.get()) return false;
    for (std::map<std::string, DCP_SHARED_PTR<PointData> >::iterator it = m_currentJob->points.begin();
         it != m_currentJob->points.end(); ++it) {
        if (!it->second) continue;
        PointData& p = *it->second;
        std::swap(p.x_mea, p.x_dsg);
        std::swap(p.y_mea, p.y_dsg);
        std::swap(p.z_mea, p.z_dsg);
    }
    return true;
}

static std::string subStr(const std::string& line, size_t pos, size_t len) {
    if (pos + len > line.size()) return "";
    return trim(line.substr(pos, len));
}

/// Split line by separator. For "," split by comma; for " " or tab split by whitespace (DCP9-aligned).
static void splitLine(const std::string& line, const std::string& separator, std::vector<std::string>& fields) {
    fields.clear();
    if (separator == ",") {
        std::string::size_type start = 0;
        for (;;) {
            std::string::size_type end = line.find(',', start);
            if (end == std::string::npos) {
                fields.push_back(trim(line.substr(start)));
                break;
            }
            fields.push_back(trim(line.substr(start, end - start)));
            start = end + 1;
        }
    } else {
        std::string s = trim(line);
        std::string::size_type pos = 0;
        while (pos < s.size()) {
            std::string::size_type start = s.find_first_not_of(" \t", pos);
            if (start == std::string::npos) break;
            std::string::size_type end = s.find_first_of(" \t", start);
            if (end == std::string::npos)
                end = s.size();
            fields.push_back(s.substr(start, end - start));
            pos = end;
        }
    }
}

bool JsonDatabase::importFromTXT(const std::string& filename, const std::string& separator) {
    if (!m_currentJob.get()) return false;
    std::ifstream f(filename.c_str());
    if (!f.is_open()) return false;
    std::vector<std::string> allLines;
    std::string line;
    while (std::getline(f, line)) {
        std::string t = trim(line);
        if (!t.empty()) allLines.push_back(t);
    }
    f.close();
    if (allLines.empty()) return false;

    std::vector<std::string> fields;
    int typicalColumnCount = 0;
    int sampleCount = 0;
    for (size_t i = 0; i < allLines.size() && sampleCount < 3; ++i) {
        splitLine(allLines[i], separator, fields);
        if (fields.size() >= 3) {
            typicalColumnCount += static_cast<int>(fields.size());
            sampleCount++;
        }
    }
    if (sampleCount > 0)
        typicalColumnCount /= sampleCount;
    bool hasDesignColumns = (typicalColumnCount >= 7);

    int pointCount = 0;
    for (size_t i = 0; i < allLines.size(); ++i) {
        splitLine(allLines[i], separator, fields);
        if (fields.size() < 3) continue;

        PointData pt;
        pt.type = "point";
        pt.measTime = getCurrentIsoTime();

        std::string idStr = fields[0];
        if (!idStr.empty() && !std::isalpha(static_cast<unsigned char>(idStr[0])))
            idStr = "_" + idStr;
        if (idStr.empty()) {
            char buf[24];
            sprintf(buf, "P%d", ++pointCount);
            pt.id = buf;
        } else {
            pt.id = idStr;
        }

        if (hasDesignColumns && fields.size() >= 7) {
            pt.x_dsg = parseDouble(fields.size() > 1 ? fields[1] : "");
            pt.y_dsg = parseDouble(fields.size() > 2 ? fields[2] : "");
            pt.z_dsg = parseDouble(fields.size() > 3 ? fields[3] : "");
            pt.x_mea = parseDouble(fields.size() > 4 ? fields[4] : "");
            pt.y_mea = parseDouble(fields.size() > 5 ? fields[5] : "");
            pt.z_mea = parseDouble(fields.size() > 6 ? fields[6] : "");
        } else {
            pt.x_dsg = pt.y_dsg = pt.z_dsg = std::numeric_limits<double>::quiet_NaN();
            pt.x_mea = parseDouble(fields.size() > 1 ? fields[1] : "");
            pt.y_mea = parseDouble(fields.size() > 2 ? fields[2] : "");
            pt.z_mea = parseDouble(fields.size() > 3 ? fields[3] : "");
        }
        if (fields.size() > 7) pt.note = fields[7];

        bool hasActuals = !DCP_ISNAN(pt.x_mea) || !DCP_ISNAN(pt.y_mea) || !DCP_ISNAN(pt.z_mea);
        bool hasDesign = !DCP_ISNAN(pt.x_dsg) || !DCP_ISNAN(pt.y_dsg) || !DCP_ISNAN(pt.z_dsg);
        pt.status = hasActuals ? 2 : (hasDesign ? 1 : 0);
        pt.x_scs = pt.x_mea; pt.y_scs = pt.y_mea; pt.z_scs = pt.z_mea;
        pt.x_scs0 = pt.x_mea; pt.y_scs0 = pt.y_mea; pt.z_scs0 = pt.z_mea;
        addPoint(pt.id, pt);
        pointCount++;
    }
    return pointCount > 0;
}

static void writeTxtDouble(std::ostream& f, double v, const std::string& sep) {
    if (!DCP_ISNAN(v)) f << v;
    f << sep;
}

bool JsonDatabase::exportToTXT(const std::string& filename, const std::string& separator) {
    if (!m_currentJob.get()) return false;
    std::ofstream f(filename.c_str());
    if (!f.is_open()) return false;
    f << std::fixed << std::setprecision(6);
    std::vector<std::pair<int, DCP_SHARED_PTR<PointData> > > sorted;
    for (std::map<std::string, DCP_SHARED_PTR<PointData> >::const_iterator it = m_currentJob->points.begin();
         it != m_currentJob->points.end(); ++it) {
        if (it->second) sorted.push_back(std::make_pair(pointIdSortKey(it->first), it->second));
    }
    std::sort(sorted.begin(), sorted.end(), SortByPointId());
    for (size_t i = 0; i < sorted.size(); ++i) {
        const PointData& p = *sorted[i].second;
        f << p.id << separator;
        writeTxtDouble(f, p.x_dsg, separator);
        writeTxtDouble(f, p.y_dsg, separator);
        writeTxtDouble(f, p.z_dsg, separator);
        writeTxtDouble(f, p.x_mea, separator);
        writeTxtDouble(f, p.y_mea, separator);
        if (!DCP_ISNAN(p.z_mea)) f << p.z_mea;
        if (!p.note.empty()) f << separator << p.note;
        f << "\n";
    }
    return true;
}

bool JsonDatabase::importFromADF(const std::string& filename) {
    if (!m_currentJob.get()) return false;
    std::ifstream f(filename.c_str());
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
        pt.x_mea = parseDouble(subStr(line, 9, 9));  pt.x_dsg = parseDouble(subStr(line, 19, 9));
        pt.y_mea = parseDouble(subStr(line, 30, 10)); pt.y_dsg = parseDouble(subStr(line, 41, 9));
        pt.z_mea = parseDouble(subStr(line, 53, 9));  pt.z_dsg = parseDouble(subStr(line, 63, 9));
        if (line.size() > 73) pt.note = trim(line.substr(73));
        bool hasActuals = !DCP_ISNAN(pt.x_mea) || !DCP_ISNAN(pt.y_mea) || !DCP_ISNAN(pt.z_mea);
        bool hasDesign = !DCP_ISNAN(pt.x_dsg) || !DCP_ISNAN(pt.y_dsg) || !DCP_ISNAN(pt.z_dsg);
        pt.status = hasActuals ? 2 : (hasDesign ? 1 : 0);
        pt.x_scs = pt.x_mea; pt.y_scs = pt.y_mea; pt.z_scs = pt.z_mea;
        pt.x_scs0 = pt.x_mea; pt.y_scs0 = pt.y_mea; pt.z_scs0 = pt.z_mea;
        addPoint(pt.id, pt);
        pointCount++;
    }
    return pointCount > 0;
}

static void fmtNum(double v, char* out) {
    DCP_ISNAN(v) ? sprintf(out, "%9.9s", " ") : sprintf(out, "%9.3f", v);
}
static char fmtStatus(double v) {
    return (DCP_ISNAN(v) || v == 0) ? ' ' : 'X';
}

bool JsonDatabase::exportToADF(const std::string& filename) {
    if (!m_currentJob.get()) return false;
    std::ofstream f(filename.c_str());
    if (!f.is_open()) return false;
    f << "ADF File\nJob: " << m_currentJob->id << "\nDate: " << getCurrentIsoTime() << "\n\n";
    f << "PID X X_ACTUAL X_DESIGN Y Y_ACTUAL Y_DESIGN Z Z_ACTUAL Z_DESIGN\n";
    f << std::fixed << std::setprecision(3);
    for (std::map<std::string, DCP_SHARED_PTR<PointData> >::const_iterator it = m_currentJob->points.begin();
         it != m_currentJob->points.end(); ++it) {
        const DCP_SHARED_PTR<PointData>& pt = it->second;
        if (!pt) continue;
        char pid[8] = "      ";
        std::string id = pt->id.size() <= 6 ? pt->id : pt->id.substr(0, 6);
        for (size_t i = 0; i < id.size() && i < 6; i++) pid[i] = id[i];
        char xa[10], xd[10], ya[10], yd[10], za[10], zd[10];
        fmtNum(pt->x_mea, xa); fmtNum(pt->x_dsg, xd);
        fmtNum(pt->y_mea, ya); fmtNum(pt->y_dsg, yd);
        fmtNum(pt->z_mea, za); fmtNum(pt->z_dsg, zd);
        char xsta = fmtStatus(pt->x_mea), ysta = fmtStatus(pt->y_mea), zsta = fmtStatus(pt->z_mea);
        std::string note = pt->note.size() <= 6 ? pt->note : pt->note.substr(0, 6);
        char noteBuf[8] = "      ";
        for (size_t i = 0; i < note.size() && i < 6; i++) noteBuf[i] = note[i];
        char lineBuf[256];
        sprintf(lineBuf, "%-6.6s %c %9.9s %9.9s %c %9.9s %9.9s %c %9.9s %9.9s %-6.6s\n",
                pid, xsta, xa, xd, ysta, ya, yd, zsta, za, zd, noteBuf);
        f << lineBuf;
    }
    return true;
}

}  // namespace Database
}  // namespace DCP
