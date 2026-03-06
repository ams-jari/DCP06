// ====================================================================================================================
// CalculationPlane.hpp - Plane calculation core and controller
// Consolidated from CalcPlane.hpp and CalcPlaneController.hpp
// ====================================================================================================================

#ifndef DCP_CALCULATION_PLANE_HPP
#define DCP_CALCULATION_PLANE_HPP

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/core/Defs.hpp>
#include <GUI_AppBase.hpp>
#include <TBL_SurveyModel.hpp>
#include <TBL_Measurement.hpp>

namespace DCP
{
    class CalcPlane
    {
    public:
        CalcPlane();
        ~CalcPlane();
        short calc(S_PLANE_BUFF* plane, short actdes);
        short defined_points_count_in_plane(S_PLANE_BUFF* plane, short* lastpoint);
    private:
    };

    class CalcPlaneontrollerC : public GUI::ControllerC
    {
    public:
        CalcPlaneontrollerC(S_PLANE_BUFF* oLine, short actdes, short iAskId);
        ~CalcPlaneontrollerC();
        virtual bool SetModel(GUI::ModelC* pModel);
        virtual void OnActiveDialogClosed(int lDlgID, int lExitCode);
        virtual void OnActiveControllerClosed(int lCtrlID, int lExitCode);
        virtual void OnControllerActivated(void);
        virtual void Run(void);
    private:
        CalcPlaneontrollerC(const CalcPlaneontrollerC&) { USER_APP_VERIFY(false); }
        CalcPlaneontrollerC& operator=(const CalcPlaneontrollerC&) { USER_APP_VERIFY(false); return *this; }
        Common* m_pCommon;
        Model* m_pModel;
        S_PLANE_BUFF* m_pPlaneBuff;
        short m_iActDes;
        short m_iAskId;
    };
}

#endif // DCP_CALCULATION_PLANE_HPP
