// ====================================================================================================================
// CalculationEdit.hpp - Edit dialogs for Angle and Distance calculation
// Consolidated from EditCalcAngle.hpp and EditCalcDist.hpp
// ====================================================================================================================

#ifndef DCP_CALCULATION_EDIT_HPP
#define DCP_CALCULATION_EDIT_HPP

#include <dcp06/core/Types.hpp>
#include <dcp06/core/PointBuffModel.hpp>
#include <dcp06/core/Common.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>

namespace DCP
{
    class DCP06ModelC;
    class DCP06EditCalcAngleDlgC;
    class DCP06EditCalcAngleModelC;
    class DCP06EditCalcDistDlgC;
    class DCP06EditCalcDistModelC;

    // --- EditCalcAngle ---
    class DCP06EditCalcAngleControllerC : public GUI::ControllerC
    {
    public:
        DCP06EditCalcAngleControllerC(DCP06ModelC* pDCP06Model);
        ~DCP06EditCalcAngleControllerC();
        virtual void OnF1Pressed();
        virtual bool SetModel(GUI::ModelC* pModel);
        virtual void OnActiveDialogClosed(int lDlgID, int lExitCode);
        virtual void OnActiveControllerClosed(int lCtrlID, int lExitCode);
    private:
        DCP06EditCalcAngleControllerC(const DCP06EditCalcAngleControllerC&) { USER_APP_VERIFY(false); }
        DCP06EditCalcAngleControllerC& operator=(const DCP06EditCalcAngleControllerC&) { USER_APP_VERIFY(false); return *this; }
        DCP06EditCalcAngleDlgC* m_pDlg;
        DCP06ModelC* m_pDCP06Model;
        DCP06CommonC* m_pCommon;
    };

    class DCP06EditCalcAngleDlgC : public GUI::StandardDialogC, public GUI::ModelHandlerC
    {
    public:
        enum eCtrlId { eDistId = 1, eRefId, eTrgtId, eNote };
        DCP06EditCalcAngleDlgC(DCP06ModelC* pDCP06Model);
        virtual ~DCP06EditCalcAngleDlgC();
        virtual void OnInitDialog(void);
        virtual void OnDialogActivated();
        virtual void UpdateData();
        virtual void RefreshControls();
        virtual bool SetModel(GUI::ModelC* pModel);
        DCP06EditCalcAngleModelC* GetDataModel() const;
    protected:
        GUI::ComboLineCtrlC* m_pDistId;
        GUI::ComboLineCtrlC* m_pRefId;
        GUI::ComboLineCtrlC* m_pTrgtId;
        GUI::ComboLineCtrlC* m_pNote;
        DCP06ModelC* m_pDCP06Model;
    private:
        DCP06CommonC* m_pCommon;
    };

    class DCP06EditCalcAngleModelC : public GUI::ModelC
    {
    public:
        StringC sDistId, sRefId, sTrgtId, sNote;
    };

    // --- EditCalcDist ---
    class DCP06EditCalcDistControllerC : public GUI::ControllerC
    {
    public:
        DCP06EditCalcDistControllerC(DCP06ModelC* pDCP06Model);
        ~DCP06EditCalcDistControllerC();
        virtual void OnF1Pressed();
        virtual bool SetModel(GUI::ModelC* pModel);
        virtual void OnActiveDialogClosed(int lDlgID, int lExitCode);
        virtual void OnActiveControllerClosed(int lCtrlID, int lExitCode);
    private:
        DCP06EditCalcDistControllerC(const DCP06EditCalcDistControllerC&) { USER_APP_VERIFY(false); }
        DCP06EditCalcDistControllerC& operator=(const DCP06EditCalcDistControllerC&) { USER_APP_VERIFY(false); return *this; }
        DCP06EditCalcDistDlgC* m_pDlg;
        DCP06ModelC* m_pDCP06Model;
        DCP06CommonC* m_pCommon;
    };

    class DCP06EditCalcDistDlgC : public GUI::StandardDialogC, public GUI::ModelHandlerC
    {
    public:
        enum eCtrlId { eDistId = 1, eRefId, eNote };
        DCP06EditCalcDistDlgC(DCP06ModelC* pDCP06Model);
        virtual ~DCP06EditCalcDistDlgC();
        virtual void OnInitDialog(void);
        virtual void OnDialogActivated();
        virtual void UpdateData();
        virtual void RefreshControls();
        virtual bool SetModel(GUI::ModelC* pModel);
        DCP06EditCalcDistModelC* GetDataModel() const;
    protected:
        GUI::ComboLineCtrlC* m_pDistId;
        GUI::ComboLineCtrlC* m_pRefId;
        GUI::ComboLineCtrlC* m_pNote;
        DCP06ModelC* m_pDCP06Model;
    private:
        DCP06CommonC* m_pCommon;
    };

    class DCP06EditCalcDistModelC : public GUI::ModelC
    {
    public:
        StringC sDistId, sRefId, sNote;
    };
}

#endif // DCP_CALCULATION_EDIT_HPP
