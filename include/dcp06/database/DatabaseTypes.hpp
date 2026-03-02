#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <limits>
#include <cmath>

namespace DCP {
namespace Database {

#define MAX_POINTS_PER_JOB 1000

// Base for measurement data
struct MeasurementData {
    std::string type;
    std::string id;
    std::string measTime;  // ISO format, replaces QDateTime
    std::string measurer;
    std::string instrumentId;
    bool calculated{false};
    double rms{0.0};
};

// Point measurement data (aligned with DCP9 PointData)
struct PointData : MeasurementData {
    double x_dsg{std::numeric_limits<double>::quiet_NaN()};
    double y_dsg{std::numeric_limits<double>::quiet_NaN()};
    double z_dsg{std::numeric_limits<double>::quiet_NaN()};
    double x_mea{0.0}, y_mea{0.0}, z_mea{0.0};
    double x_scs{0.0}, y_scs{0.0}, z_scs{0.0};
    double x_scs0{0.0}, y_scs0{0.0}, z_scs0{0.0};
    double hor_angle{0.0}, ver_angle{0.0}, distance{0.0};
    double precision{0.0}, dev_x{0.0}, dev_y{0.0}, dev_z{0.0};
    int status{0};
    std::string note;
    double offset_x{0.0}, offset_y{0.0}, offset_z{0.0};
    bool partial{false};
    std::string prism;
    double prism_constant{0.0};
    bool spherical_prism_correction{false};
    std::string orientation_type;
    std::string orientation_id;
    int orientation_object_key{0};
    int change_station_object_key{0};
};

// 4x3 transformation matrix (aligned with DCP9)
struct Matrix4x3 {
    double a00{1.0}, a01{0.0}, a02{0.0}, a03{0.0};
    double a10{0.0}, a11{1.0}, a12{0.0}, a13{0.0};
    double a20{0.0}, a21{0.0}, a22{1.0}, a23{0.0};
};

// Midpoint (Phase 1: minimal for compatibility)
struct MidpointData : MeasurementData {
    double x{0.0}, y{0.0}, z{0.0};
    double x_scs0{0.0}, y_scs0{0.0}, z_scs0{0.0};
    std::map<int, PointData> points;
};

// Circle (Phase 1: minimal)
struct CircleData : MeasurementData {
    std::string planeType;
    double center_x{0.0}, center_y{0.0}, center_z{0.0};
    double center_x_scs0{0.0}, center_y_scs0{0.0}, center_z_scs0{0.0};
    double normal_x{0.0}, normal_y{0.0}, normal_z{0.0};
    double normal_x_scs0{0.0}, normal_y_scs0{0.0}, normal_z_scs0{0.0};
    double diameter{0.0};
    int weakestPoint{0};
    double maxDeviation{0.0};
    double toolRadius{0.0};
    std::map<int, PointData> planePoints;
    std::map<int, PointData> points;
};

struct ScanPoint3D {
    double x{0.0}, y{0.0}, z{0.0};
};

struct SurfaceScanData {
    std::string type{"surface_scan"};
    std::string id;
    std::string measTime;
    int pointCount{0};
    double minX{0.0}, minY{0.0}, minZ{0.0};
    double maxX{0.0}, maxY{0.0}, maxZ{0.0};
    std::string pointsFile;
    std::string regionSummary;
};

struct ShaftAlignmentData : MeasurementData {
    std::string shaftType;
    double shaftDiameter{0.0}, shaftLength{0.0};
    std::string alignmentType;
    double tolerance{0.0}, maxDeviation{0.0};
    double angularDeviation{0.0}, parallelDeviation{0.0};
    std::map<std::string, PointData> points;
};

struct ChangeStationData : MeasurementData {
    std::map<std::string, PointData> points_prev;
    std::map<std::string, PointData> points_new;
    std::map<std::string, PointData> points_residuals;
    int chst_hz_plane{0};
    double rms{0.0};
    Matrix4x3 matrix;
};

struct Alignment321Data : MeasurementData {
    struct Plane {
        double px{0.0}, py{0.0}, pz{0.0};
        double nx{0.0}, ny{0.0}, nz{0.0};
        double rms{0.0};
        bool calculated{false};
        std::map<std::string, PointData> points;
    } plane;
    struct Line {
        double px{0.0}, py{0.0}, pz{0.0};
        double ux{0.0}, uy{0.0}, uz{0.0};
        double rms{0.0};
        bool calculated{false};
        std::map<std::string, PointData> points;
    } line;
    PointData offset_value, offset_value_tool, reference_point;
    PointData rotate_plane_buff, rotate_line_buff;
    struct Angle { double value{0.0}; bool calculated{false}; };
    std::map<std::string, Angle> rotate_plane;
    Angle rotate_line;
    int rotate_plane_order{0};
    Matrix4x3 matrix;
};

struct CylinderAlignmentData : MeasurementData {
    struct CenterLine {
        std::string method;
        double px{0.0}, py{0.0}, pz{0.0};
        double ux{0.0}, uy{0.0}, uz{0.0};
        double rms{0.0};
        bool calculated{false};
        std::map<std::string, PointData> points;
    } center_line;
    struct ReferenceAngle {
        std::string method;
        PointData point;
        bool defined{false};
    } reference_angle;
    struct Origin {
        std::string method;
        PointData point;
        bool defined{false};
    } origin;
    int com_hz_plane{0};
    Matrix4x3 matrix;
};

struct BestFitAlignmentData : MeasurementData {
    std::string bestFitAlignmentType;
    double position_x{0.0}, position_y{0.0}, position_z{0.0};
    double scsPosition0_x{0.0}, scsPosition0_y{0.0}, scsPosition0_z{0.0};
    double orientation_x{0.0}, orientation_y{0.0}, orientation_z{0.0};
    double scsOrientation0_x{0.0}, scsOrientation0_y{0.0}, scsOrientation0_z{0.0};
    double tolerance{0.0}, maxDeviation{0.0};
    std::map<std::string, PointData> points_scs;
    std::map<std::string, PointData> points_ocs;
    std::map<std::string, PointData> points_residuals;
    Matrix4x3 matrix;
};

struct ActiveOrientation {
    std::string type{"?"};
    std::string id{"SCS"};
    int object_key{0};
};

struct JobData {
    std::string type{"job"};
    int object_key{1};
    std::string id;
    std::string date;
    std::string measurer;
    std::string instrument_id;
    std::string units;
    std::string active_change_station;
    std::string target;
    int decimals{2};
    double temperature{0.0}, pressure{0.0}, humidity{0.0};
    std::string remote{"yes"};
    double flow_time{2.0}, flow_move{2.0};
    std::string overwrite_info{"yes"};
    std::string auto_increment{"no"};
    std::string save_tool_settings{"yes"};
    std::string tool_info{"yes"};
    double inclination_threshold{0.015};
    std::string default_folder;
    std::string simulator{"no"};
    bool show_design_coords{true};
    bool show_deviation_coords{true};
    bool show_tool_offset{true};
    ActiveOrientation active_orientation;

    std::map<std::string, std::shared_ptr<PointData>> points;
    std::map<std::string, std::shared_ptr<MidpointData>> midpointsData;
    std::map<std::string, std::shared_ptr<CircleData>> circlesData;
    std::map<std::string, std::shared_ptr<Alignment321Data>> alignments321;
    std::map<std::string, std::shared_ptr<BestFitAlignmentData>> bestFitAlignments;
    std::map<std::string, std::shared_ptr<CylinderAlignmentData>> cylinderAlignments;
    std::map<std::string, std::shared_ptr<ChangeStationData>> changeStationsData;
    std::map<std::string, std::shared_ptr<ShaftAlignmentData>> shaftAlignments;
    std::map<std::string, std::shared_ptr<SurfaceScanData>> surfaceScans;
};

}  // namespace Database
}  // namespace DCP
