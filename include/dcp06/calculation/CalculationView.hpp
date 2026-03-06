// ====================================================================================================================
// CalculationView.hpp - View dialogs for Angle (AGF) and Distance (CDF) calculation files
// Consolidated from ViewAgf.hpp and ViewCdf.hpp
// ====================================================================================================================

#ifndef DCP_CALCULATION_VIEW_HPP
#define DCP_CALCULATION_VIEW_HPP

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/calculation/CalculationAngle.hpp>
#include <dcp06/calculation/CalculationDist.hpp>

#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_TableDialog.hpp>
#include <GUI_ListMultiColCtrl.hpp>
#include <GUI_AppBase.hpp>

namespace DCP
{
    class Model;
    class ViewAgfDialog;
    class ViewAgfModel;
    class ViewCdfDialog;
    class ViewCdfModel;

    // --- ViewAgf (Angle file view) ---
    class ViewAgfController : public GUI::ControllerC
    {
    public:
        ViewAgfController(AgfFileFunc* pFileFunc, Model* pModel);
        ~ViewAgfController();
        virtual void OnF1Pressed();
        virtual void OnSHF5Pressed();
        virtual void OnF6Pressed();
        virtual bool SetModel(GUI::ModelC* pModel);
        virtual void OnActiveDialogClosed(int lDlgID, int lExitCode);
        void OnActiveControllerClosed(int lCtrlID, int lExitCode);
    private:
        ViewAgfController(const ViewAgfController&) { USER_APP_VERIFY(false); }
        ViewAgfController& operator=(const ViewAgfController&) { USER_APP_VERIFY(false); return *this; }
        ViewAgfDialog* m_pDlg;
        AgfFileFunc* m_pFileFunc;
        Model* m_pModel;
    };

    class ViewAgfDialog : public GUI::TableDialogC, public GUI::ModelHandlerC
    {
    public:
        enum CI_ColumnId { CI_No = 1, CI_AngleId, CI_Angle, CI_Note };
        ViewAgfDialog(AgfFileFunc* pFileFunc, Model* pModel);
        virtual ~ViewAgfDialog();
        virtual void OnInitDialog(void);
        virtual void OnDialogActivated();
        virtual void UpdateData();
        virtual bool SetModel(GUI::ModelC* pModel);
        Model* GetModel() const;
        virtual void RefreshControls();
        bool DeletePoint();
        bool GetSelectedData(StringC& dDistId, StringC& sRefId, StringC& sTrgtId, StringC& sNote);
    protected:
        GUI::ListMultiColCtrlC* poMultiColCtrl;
        Model* m_pModel;
    private:
        short m_iSelectedCount;
        StringC m_strMaxPointSelected;
        Common* m_pCommon;
        AgfFileFunc* m_pFileFunc;
    };

    // --- ViewCdf (Distance file view) ---
    class ViewCdfController : public GUI::ControllerC
    {
    public:
        ViewCdfController(CdfFileFunc* pFileFunc, Model* pModel);
        ~ViewCdfController();
        virtual void OnF1Pressed();
        virtual void OnSHF5Pressed();
        virtual void OnF6Pressed();
        virtual bool SetModel(GUI::ModelC* pModel);
        virtual void OnActiveDialogClosed(int lDlgID, int lExitCode);
        void OnActiveControllerClosed(int lCtrlID, int lExitCode);
    private:
        ViewCdfController(const ViewCdfController&) { USER_APP_VERIFY(false); }
        ViewCdfController& operator=(const ViewCdfController&) { USER_APP_VERIFY(false); return *this; }
        ViewCdfDialog* m_pDlg;
        CdfFileFunc* m_pFileFunc;
        Model* m_pModel;
    };

    class ViewCdfDialog : public GUI::TableDialogC, public GUI::ModelHandlerC
    {
    public:
        enum CI_ColumnId { CI_No = 1, CI_DistId, CI_Dist, CI_Note };
        ViewCdfDialog(CdfFileFunc* pFileFunc, Model* pModel);
        virtual ~ViewCdfDialog();
        virtual void OnInitDialog(void);
        virtual void OnDialogActivated();
        virtual void UpdateData();
        virtual bool SetModel(GUI::ModelC* pModel);
        Model* GetModel() const;
        virtual void RefreshControls();
        bool DeletePoint();
        bool GetSelectedData(StringC& dDistId, StringC& sRefId, StringC& sNote);
    protected:
        GUI::ListMultiColCtrlC* poMultiColCtrl;
        Model* m_pModel;
    private:
        StringC sActualSelected;
        StringC sActualNonSelected;
        StringC sDesignSelected;
        StringC sDesignNonSelected;
        short m_iSelectedCount;
        StringC m_strMaxPointSelected;
        Common* m_pCommon;
        CdfFileFunc* m_pFileFunc;
    };
}

#endif // DCP_CALCULATION_VIEW_HPP
