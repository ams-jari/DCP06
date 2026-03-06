// ====================================================================================================================
// CalculationCircleCore.hpp - Circle calculation core and controller
// Consolidated from CalcCircle.hpp and CalcCircleController.hpp
// ====================================================================================================================

#ifndef DCP_CALCULATION_CIRCLE_CORE_HPP
#define DCP_CALCULATION_CIRCLE_CORE_HPP

#include <dcp06/core/Types.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/measurement/Circle.hpp>

#include <GUI_AppBase.hpp>
#include <TBL_SurveyModel.hpp>
#include <TBL_Measurement.hpp>

namespace DCP
{
    class CalcCircle
    {
    public:
        CalcCircle(short plane_type, S_CIRCLE_BUFF* circle_points, S_PLANE_BUFF* planes,
            S_CIRCLE_BUFF* circle_points_in_plane, double dToolRadius, Model* pModel);
        ~CalcCircle();
        short calc_center_of_circle();
        double diameter, cx, cy, cz, vi, vj, vk;
        void get_results(double* dCx, double* dCy, double* dCz, double* dVi, double* dVj, double* dVk, double* dDiameter, double* rms, short* iPno);
        double get_max_dist_and_rms_circle(S_CIRCLE_BUFF* circle_, short* pno, double* rms_diameter);
    private:
        Common* m_pCommon;
        MsgBox* m_pMsgBox;
        short m_iPlaneType;
        S_CIRCLE_BUFF* m_pCircle_points;
        S_CIRCLE_BUFF* m_pCircle_points_in_plane;
        S_PLANE_BUFF* m_pPlanes;
        double m_dToolRadius;
        double ccs_matrix[4][4];
        double ccs_inv_matrix[4][4];
        S_PLANE_BUFF temp_plane_table[1];
        short convert_points_to_plane(S_CIRCLE_BUFF* circle_points, S_CIRCLE_BUFF* circle_points_in_plane, short count);
        short calc_matrix(S_POINT_BUFF* points);
        short calc_circle(S_CIRCLE_BUFF* points, int count, double* cxtod, double* cytod, double* diameter);
        short pns_ymp(S_CIRCLE_BUFF* points, int count, double* cxtod, double* cytod, double* diameter);
    };

    class CalcCircleontrollerC : public GUI::ControllerC
    {
    public:
        CalcCircleontrollerC(CircleModel* pCircleModel, short iDisplay = 0);
        ~CalcCircleontrollerC();
        virtual bool SetModel(GUI::ModelC* pModel);
        virtual void OnActiveDialogClosed(int lDlgID, int lExitCode);
        virtual void OnActiveControllerClosed(int lCtrlID, int lExitCode);
        virtual void OnControllerActivated(void);
        virtual void Run(void);
    private:
        CalcCircleontrollerC(const CalcCircleontrollerC&) { USER_APP_VERIFY(false); }
        CalcCircleontrollerC& operator=(const CalcCircleontrollerC&) { USER_APP_VERIFY(false); return *this; }
        Common* m_pCommon;
        Model* m_pModel;
        CircleModel* m_pDataModel;
        short m_iDisplay;
    };
}

#endif // DCP_CALCULATION_CIRCLE_CORE_HPP
