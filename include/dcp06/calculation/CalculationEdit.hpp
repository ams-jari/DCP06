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
    class Model;
    class EditCalculationAngleDialog;
    class EditCalculationAngleModel;
    class EditCalculationDistDialog;
    class EditCalculationDistModel;

    // --- EditCalcAngle ---
    class EditCalculationAngleController : public GUI::ControllerC
    {
    public:
        EditCalculationAngleController(Model* pModel);
        ~EditCalculationAngleController();
        virtual void OnF1Pressed();
        virtual bool SetModel(GUI::ModelC* pModel);
        virtual void OnActiveDialogClosed(int lDlgID, int lExitCode);
        virtual void OnActiveControllerClosed(int lCtrlID, int lExitCode);
    private:
        EditCalculationAngleController(const EditCalculationAngleController&) { USER_APP_VERIFY(false); }
        EditCalculationAngleController& operator=(const EditCalculationAngleController&) { USER_APP_VERIFY(false); return *this; }
        EditCalculationAngleDialog* m_pDlg;
        Model* m_pModel;
        Common* m_pCommon;
    };

    class EditCalculationAngleDialog : public GUI::StandardDialogC, public GUI::ModelHandlerC
    {
    public:
        enum eCtrlId { eDistId = 1, eRefId, eTrgtId, eNote };
        EditCalculationAngleDialog(Model* pModel);
        virtual ~EditCalculationAngleDialog();
        virtual void OnInitDialog(void);
        virtual void OnDialogActivated();
        virtual void UpdateData();
        virtual void RefreshControls();
        virtual bool SetModel(GUI::ModelC* pModel);
        EditCalculationAngleModel* GetDataModel() const;
    protected:
        GUI::ComboLineCtrlC* m_pDistId;
        GUI::ComboLineCtrlC* m_pRefId;
        GUI::ComboLineCtrlC* m_pTrgtId;
        GUI::ComboLineCtrlC* m_pNote;
        Model* m_pModel;
    private:
        Common* m_pCommon;
    };

    class EditCalculationAngleModel : public GUI::ModelC
    {
    public:
        StringC sDistId, sRefId, sTrgtId, sNote;
    };

    // --- EditCalcDist ---
    class EditCalculationDistController : public GUI::ControllerC
    {
    public:
        EditCalculationDistController(Model* pModel);
        ~EditCalculationDistController();
        virtual void OnF1Pressed();
        virtual bool SetModel(GUI::ModelC* pModel);
        virtual void OnActiveDialogClosed(int lDlgID, int lExitCode);
        virtual void OnActiveControllerClosed(int lCtrlID, int lExitCode);
    private:
        EditCalculationDistController(const EditCalculationDistController&) { USER_APP_VERIFY(false); }
        EditCalculationDistController& operator=(const EditCalculationDistController&) { USER_APP_VERIFY(false); return *this; }
        EditCalculationDistDialog* m_pDlg;
        Model* m_pModel;
        Common* m_pCommon;
    };

    class EditCalculationDistDialog : public GUI::StandardDialogC, public GUI::ModelHandlerC
    {
    public:
        enum eCtrlId { eDistId = 1, eRefId, eNote };
        EditCalculationDistDialog(Model* pModel);
        virtual ~EditCalculationDistDialog();
        virtual void OnInitDialog(void);
        virtual void OnDialogActivated();
        virtual void UpdateData();
        virtual void RefreshControls();
        virtual bool SetModel(GUI::ModelC* pModel);
        EditCalculationDistModel* GetDataModel() const;
    protected:
        GUI::ComboLineCtrlC* m_pDistId;
        GUI::ComboLineCtrlC* m_pRefId;
        GUI::ComboLineCtrlC* m_pNote;
        Model* m_pModel;
    private:
        Common* m_pCommon;
    };

    class EditCalculationDistModel : public GUI::ModelC
    {
    public:
        StringC sDistId, sRefId, sNote;
    };
}

#endif // DCP_CALCULATION_EDIT_HPP
