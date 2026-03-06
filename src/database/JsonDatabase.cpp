#include "stdafx.h"
#include <dcp06/database/JsonDatabase.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cctype>
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
    p /= ".work";
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
    return j;
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
    return true;
}

bool JsonDatabase::createJob(const std::string& jobId) {
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
    return true;
}

bool JsonDatabase::loadJob(const std::string& jobId) {
    Json::Value j;
    if (!readJsonFromFile(getJobFilePath(jobId), j)) return false;
    m_currentJob = DCP_UNIQUE_PTR<JobData>(new JobData());
    if (!jsonToJobData(j, *m_currentJob)) {
        m_currentJob.reset();
        return false;
    }
    m_isJobLoaded = true;
    return true;
}

bool JsonDatabase::saveJob(const std::string& jobId) {
    if (!m_isJobLoaded || !m_currentJob.get()) return false;
    return writeJsonToFile(getJobFilePath(jobId), jobDataToJson(*m_currentJob));
}

bool JsonDatabase::deleteJob(const std::string& jobId) {
    std::string path = getJobFilePath(jobId);
    if (!boost::filesystem::exists(path)) return true;
    boost::system::error_code ec;
    return boost::filesystem::remove(path, ec);
}

bool JsonDatabase::copyJob(const std::string& sourceId, const std::string& targetId) {
    Json::Value j;
    if (!readJsonFromFile(getJobFilePath(sourceId), j)) return false;
    j["id"] = targetId;
    return writeJsonToFile(getJobFilePath(targetId), j);
}

bool JsonDatabase::swapJob(const std::string& jobId1, const std::string& jobId2) {
    Json::Value j1, j2;
    if (!readJsonFromFile(getJobFilePath(jobId1), j1) || !readJsonFromFile(getJobFilePath(jobId2), j2))
        return false;
    std::string t = j1["id"].asString();
    j1["id"] = jobId2;
    j2["id"] = t;
    return writeJsonToFile(getJobFilePath(jobId1), j2) && writeJsonToFile(getJobFilePath(jobId2), j1);
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
        if (!pointHasActualValues(*it->second) && !pointHasDesignValues(*it->second)) continue;
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
        std::string id = pt->id.size() <= 6 ? pt->id : pt->id.substr(0, 6);
        sprintf(pList[iCount].point_id, "%-6.6s", id.c_str());
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
        std::string id = pt->id.size() <= 6 ? pt->id : pt->id.substr(0, 6);
        sprintf(pList[iCount].point_id, "%-6.6s", id.c_str());
        bool mea = pointHasActualValues(*pt);
        bool des = pointHasDesignValues(*pt);
        sprintf(pList[iCount].point_status, "%s/%s", mea ? "A" : "-", des ? "D" : "-");
        iCount++;
    }
    return iCount;
}

static void fmtDouble(double v, char* buf) {
    if (buf) DCP_ISNAN(v) ? sprintf(buf, "%9.9s", " ") : sprintf(buf, "%9.3f", v);
}

bool JsonDatabase::getPointByIndex(int index1Based, bool useActual, char* pid,
    char* xact, char* xdes, char* yact, char* ydes, char* zact, char* zdes, char* note) const {
    (void)useActual;
    std::vector<DCP_SHARED_PTR<PointData> > pts = getAllPointsInJob();
    if (index1Based < 1 || index1Based > static_cast<int>(pts.size())) return false;
    const DCP_SHARED_PTR<PointData>& pt = pts[index1Based - 1];
    if (!pt) return false;
    if (pid) {
        std::string id = pt->id.size() <= 6 ? pt->id : pt->id.substr(0, 6);
        sprintf(pid, "%-6.6s", id.c_str());
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

static std::string subStr(const std::string& line, size_t pos, size_t len) {
    if (pos + len > line.size()) return "";
    return trim(line.substr(pos, len));
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
