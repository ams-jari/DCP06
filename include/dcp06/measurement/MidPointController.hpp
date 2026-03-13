// ====================================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: 
//
// $Workfile: HEW_DCP05GUI.hpp $
//
// Summary  : 
//
// ------------------------------------------------------------------------------------------------
//
// Copyright (c) AMS. Based on Leica Captivate plugin framework.
//
// ================================================================================================


// $Author: Hlar $
// $Date: 6.07.04 8:55 $
// $Revision: 1 $
// $Modtime: 5.07.04 14:55 $

/* $ History: $
*/
// $NoKeywords: $

#ifndef DCP_CALCMIDPOINT_CONTROLLER_HPP
#define DCP_CALCMIDPOINT_CONTROLLER_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/core/PointBuffModel.hpp>
#ifdef DCP06_STORE_MIDPOINT_OBJECTS
#include <dcp06/measurement/Midpoint.hpp>
#endif
#include <GUI_AppBase.hpp>
#include <TBL_SurveyModel.hpp>
#include <TBL_Measurement.hpp>

namespace DCP
{
    class MidPointController : public GUI::ControllerC
    {
    public:
        MidPointController(Model *pModel);
        ~MidPointController();

        virtual void OnF1Pressed();
        virtual void OnF2Pressed();
        virtual void OnF3Pressed();
        virtual void OnF4Pressed();
        virtual void OnF5Pressed();
        virtual void OnF6Pressed();
        virtual void OnSHF2Pressed();

        virtual bool SetModel(GUI::ModelC* pModel);
        PointBuffModel* m_pPointBuffModel;
        PointBuffModel* GetDataModel() const;

        virtual void OnActiveDialogClosed(int lDlgID, int lExitCode);
        virtual void OnActiveControllerClosed(int lCtrlID, int lExitCode);
        virtual void OnControllerActivated(void);
        virtual void Run(void);

    private:
        MidPointController(const MidPointController&) { USER_APP_VERIFY(false); }
        MidPointController& operator=(const MidPointController&) {
            USER_APP_VERIFY(false); return *this;
        }

#ifdef DCP06_STORE_MIDPOINT_OBJECTS
        void set_function_keys();
        std::string getNextMidpointId() const;
        void ShowSelectMidpointDlg();
#endif
        void RunMeas();

        Common* m_pCommon;
        Model* m_pModel;
#ifdef DCP06_STORE_MIDPOINT_OBJECTS
        MidpointDialog* m_pDlg;
        MidpointModel* m_pDataModel;
#endif
    };
}

#endif // DCP_CALCMIDPOINT_CONTROLLER_HPP




