#pragma once

#include <string>
#include <map>
#include <vector>
#include <limits>
#include <cmath>
#if defined(_MSC_VER) && _MSC_VER < 1600
#include <memory>
#define DCP_SHARED_PTR std::tr1::shared_ptr
#define DCP_UNIQUE_PTR std::auto_ptr
#define DCP_OVERRIDE
#else
#include <memory>
#define DCP_SHARED_PTR std::shared_ptr
#define DCP_UNIQUE_PTR std::unique_ptr
#define DCP_OVERRIDE override
#endif

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
    bool calculated;
    double rms;
    MeasurementData() : calculated(false), rms(0.0) {}
};

// Point measurement data (aligned with DCP9 PointData)
struct PointData : MeasurementData {
    double x_dsg, y_dsg, z_dsg;
    double x_mea, y_mea, z_mea;
    double x_scs, y_scs, z_scs;
    double x_scs0, y_scs0, z_scs0;
    double hor_angle, ver_angle, distance;
    double precision, dev_x, dev_y, dev_z;
    int status;
    std::string note;
    double offset_x, offset_y, offset_z;
    bool partial;
    std::string prism;
    double prism_constant;
    bool spherical_prism_correction;
    std::string orientation_type;
    std::string orientation_id;
    int orientation_object_key;
    int change_station_object_key;
    PointData() :
        x_dsg(std::numeric_limits<double>::quiet_NaN()),
        y_dsg(std::numeric_limits<double>::quiet_NaN()),
        z_dsg(std::numeric_limits<double>::quiet_NaN()),
        x_mea(0.0), y_mea(0.0), z_mea(0.0),
        x_scs(0.0), y_scs(0.0), z_scs(0.0),
        x_scs0(0.0), y_scs0(0.0), z_scs0(0.0),
        hor_angle(0.0), ver_angle(0.0), distance(0.0),
        precision(0.0), dev_x(0.0), dev_y(0.0), dev_z(0.0),
        status(0), offset_x(0.0), offset_y(0.0), offset_z(0.0),
        partial(false), prism_constant(0.0), spherical_prism_correction(false),
        orientation_object_key(0), change_station_object_key(0) {}
};

// 4x3 transformation matrix (aligned with DCP9)
struct Matrix4x3 {
    double a00, a01, a02, a03;
    double a10, a11, a12, a13;
    double a20, a21, a22, a23;
    Matrix4x3() :
        a00(1.0), a01(0.0), a02(0.0), a03(0.0),
        a10(0.0), a11(1.0), a12(0.0), a13(0.0),
        a20(0.0), a21(0.0), a22(1.0), a23(0.0) {}
};

// Midpoint (Phase 1: minimal for compatibility)
struct MidpointData : MeasurementData {
    double x, y, z;
    double x_scs0, y_scs0, z_scs0;
    std::map<int, PointData> points;
    MidpointData() : x(0.0), y(0.0), z(0.0), x_scs0(0.0), y_scs0(0.0), z_scs0(0.0) {}
};

// Circle (Phase 1: minimal)
struct CircleData : MeasurementData {
    std::string planeType;
    double center_x, center_y, center_z;
    double center_x_scs0, center_y_scs0, center_z_scs0;
    double normal_x, normal_y, normal_z;
    double normal_x_scs0, normal_y_scs0, normal_z_scs0;
    double diameter;
    int weakestPoint;
    double maxDeviation;
    double toolRadius;
    std::map<int, PointData> planePoints;
    std::map<int, PointData> points;
    CircleData() :
        center_x(0.0), center_y(0.0), center_z(0.0),
        center_x_scs0(0.0), center_y_scs0(0.0), center_z_scs0(0.0),
        normal_x(0.0), normal_y(0.0), normal_z(0.0),
        normal_x_scs0(0.0), normal_y_scs0(0.0), normal_z_scs0(0.0),
        diameter(0.0), weakestPoint(0), maxDeviation(0.0), toolRadius(0.0) {}
};

struct ScanPoint3D {
    double x, y, z;
    ScanPoint3D() : x(0.0), y(0.0), z(0.0) {}
};

struct SurfaceScanData {
    std::string type;
    std::string id;
    std::string measTime;
    int pointCount;
    double minX, minY, minZ;
    double maxX, maxY, maxZ;
    std::string pointsFile;
    std::string regionSummary;
    SurfaceScanData() : type("surface_scan"), pointCount(0),
        minX(0.0), minY(0.0), minZ(0.0), maxX(0.0), maxY(0.0), maxZ(0.0) {}
};

struct ShaftAlignmentData : MeasurementData {
    std::string shaftType;
    double shaftDiameter, shaftLength;
    std::string alignmentType;
    double tolerance, maxDeviation;
    double angularDeviation, parallelDeviation;
    std::map<std::string, PointData> points;
    ShaftAlignmentData() :
        shaftDiameter(0.0), shaftLength(0.0),
        tolerance(0.0), maxDeviation(0.0),
        angularDeviation(0.0), parallelDeviation(0.0) {}
};

struct ChangeStationData : MeasurementData {
    std::map<std::string, PointData> points_prev;
    std::map<std::string, PointData> points_new;
    std::map<std::string, PointData> points_residuals;
    int chst_hz_plane;
    double rms;
    Matrix4x3 matrix;
    ChangeStationData() : chst_hz_plane(0), rms(0.0) {}
};

struct Alignment321Data : MeasurementData {
    struct Plane {
        double px, py, pz;
        double nx, ny, nz;
        double rms;
        bool calculated;
        std::map<std::string, PointData> points;
        Plane() : px(0.0), py(0.0), pz(0.0), nx(0.0), ny(0.0), nz(0.0), rms(0.0), calculated(false) {}
    } plane;
    struct Line {
        double px, py, pz;
        double ux, uy, uz;
        double rms;
        bool calculated;
        std::map<std::string, PointData> points;
        Line() : px(0.0), py(0.0), pz(0.0), ux(0.0), uy(0.0), uz(0.0), rms(0.0), calculated(false) {}
    } line;
    PointData offset_value, offset_value_tool, reference_point;
    PointData rotate_plane_buff, rotate_line_buff;
    struct Angle { double value; bool calculated; Angle() : value(0.0), calculated(false) {} };
    std::map<std::string, Angle> rotate_plane;
    Angle rotate_line;
    int rotate_plane_order;
    Matrix4x3 matrix;
    Alignment321Data() : rotate_plane_order(0) {}
};

struct CylinderAlignmentData : MeasurementData {
    struct CenterLine {
        std::string method;
        double px, py, pz;
        double ux, uy, uz;
        double rms;
        bool calculated;
        std::map<std::string, PointData> points;
        CenterLine() : px(0.0), py(0.0), pz(0.0), ux(0.0), uy(0.0), uz(0.0), rms(0.0), calculated(false) {}
    } center_line;
    struct ReferenceAngle {
        std::string method;
        PointData point;
        bool defined;
        ReferenceAngle() : defined(false) {}
    } reference_angle;
    struct Origin {
        std::string method;
        PointData point;
        bool defined;
        Origin() : defined(false) {}
    } origin;
    int com_hz_plane;
    Matrix4x3 matrix;
    CylinderAlignmentData() : com_hz_plane(0) {}
};

struct BestFitAlignmentData : MeasurementData {
    std::string bestFitAlignmentType;
    double position_x, position_y, position_z;
    double scsPosition0_x, scsPosition0_y, scsPosition0_z;
    double orientation_x, orientation_y, orientation_z;
    double scsOrientation0_x, scsOrientation0_y, scsOrientation0_z;
    double tolerance, maxDeviation;
    std::map<std::string, PointData> points_scs;
    std::map<std::string, PointData> points_ocs;
    std::map<std::string, PointData> points_residuals;
    Matrix4x3 matrix;
    BestFitAlignmentData() :
        position_x(0.0), position_y(0.0), position_z(0.0),
        scsPosition0_x(0.0), scsPosition0_y(0.0), scsPosition0_z(0.0),
        orientation_x(0.0), orientation_y(0.0), orientation_z(0.0),
        scsOrientation0_x(0.0), scsOrientation0_y(0.0), scsOrientation0_z(0.0),
        tolerance(0.0), maxDeviation(0.0) {}
};

struct ActiveOrientation {
    std::string type;
    std::string id;
    int object_key;
    ActiveOrientation() : type("?"), id("SCS"), object_key(0) {}
};

struct JobData {
    std::string type;
    int object_key;
    std::string id;
    std::string date;
    std::string measurer;
    std::string instrument_id;
    std::string units;
    std::string active_change_station;
    std::string target;
    int decimals;
    double temperature, pressure, humidity;
    std::string remote;
    double flow_time, flow_move;
    std::string overwrite_info;
    std::string auto_increment;
    std::string save_tool_settings;
    std::string tool_info;
    double inclination_threshold;
    std::string default_folder;
    std::string simulator;
    bool show_design_coords;
    bool show_deviation_coords;
    bool show_tool_offset;
    ActiveOrientation active_orientation;

    std::map<std::string, DCP_SHARED_PTR<PointData> > points;
    std::map<std::string, DCP_SHARED_PTR<MidpointData> > midpointsData;
    std::map<std::string, DCP_SHARED_PTR<CircleData> > circlesData;
    std::map<std::string, DCP_SHARED_PTR<Alignment321Data> > alignments321;
    std::map<std::string, DCP_SHARED_PTR<BestFitAlignmentData> > bestFitAlignments;
    std::map<std::string, DCP_SHARED_PTR<CylinderAlignmentData> > cylinderAlignments;
    std::map<std::string, DCP_SHARED_PTR<ChangeStationData> > changeStationsData;
    std::map<std::string, DCP_SHARED_PTR<ShaftAlignmentData> > shaftAlignments;
    std::map<std::string, DCP_SHARED_PTR<SurfaceScanData> > surfaceScans;

    JobData() :
        type("job"), object_key(1), decimals(2),
        temperature(0.0), pressure(0.0), humidity(0.0),
        remote("yes"), flow_time(2.0), flow_move(2.0),
        overwrite_info("yes"), auto_increment("no"),
        save_tool_settings("yes"), tool_info("yes"),
        inclination_threshold(0.015), simulator("no"),
        show_design_coords(true), show_deviation_coords(true), show_tool_offset(true) {}
};

}  // namespace Database
}  // namespace DCP
