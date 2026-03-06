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
    class DCP06ModelC;
    class DCP06ViewAgfDlgC;
    class DCP06ViewAgfModelC;
    class DCP06ViewCdfDlgC;
    class DCP06ViewCdfModelC;

    // --- ViewAgf (Angle file view) ---
    class DCP06ViewAgfControllerC : public GUI::ControllerC
    {
    public:
        DCP06ViewAgfControllerC(AgfFileFunc* pFileFunc, DCP06ModelC* pDCP06Model);
        ~DCP06ViewAgfControllerC();
        virtual void OnF1Pressed();
        virtual void OnSHF5Pressed();
        virtual void OnF6Pressed();
        virtual bool SetModel(GUI::ModelC* pModel);
        virtual void OnActiveDialogClosed(int lDlgID, int lExitCode);
        void OnActiveControllerClosed(int lCtrlID, int lExitCode);
    private:
        DCP06ViewAgfControllerC(const DCP06ViewAgfControllerC&) { USER_APP_VERIFY(false); }
        DCP06ViewAgfControllerC& operator=(const DCP06ViewAgfControllerC&) { USER_APP_VERIFY(false); return *this; }
        DCP06ViewAgfDlgC* m_pDlg;
        AgfFileFunc* m_pFileFunc;
        DCP06ModelC* m_pDCP06Model;
    };

    class DCP06ViewAgfDlgC : public GUI::TableDialogC, public GUI::ModelHandlerC
    {
    public:
        enum CI_ColumnId { CI_No = 1, CI_AngleId, CI_Angle, CI_Note };
        DCP06ViewAgfDlgC(AgfFileFunc* pFileFunc, DCP06ModelC* pDCP06Model);
        virtual ~DCP06ViewAgfDlgC();
        virtual void OnInitDialog(void);
        virtual void OnDialogActivated();
        virtual void UpdateData();
        virtual bool SetModel(GUI::ModelC* pModel);
        DCP06ModelC* GetDCP06Model() const;
        virtual void RefreshControls();
        bool DeletePoint();
        bool GetSelectedData(StringC& dDistId, StringC& sRefId, StringC& sTrgtId, StringC& sNote);
    protected:
        GUI::ListMultiColCtrlC* poMultiColCtrl;
        DCP06ModelC* m_pDCP06Model;
    private:
        short m_iSelectedCount;
        StringC m_strMaxPointSelected;
        DCP06CommonC* m_pCommon;
        AgfFileFunc* m_pFileFunc;
    };

    // --- ViewCdf (Distance file view) ---
    class DCP06ViewCdfControllerC : public GUI::ControllerC
    {
    public:
        DCP06ViewCdfControllerC(CdfFileFunc* pFileFunc, DCP06ModelC* pDCP06Model);
        ~DCP06ViewCdfControllerC();
        virtual void OnF1Pressed();
        virtual void OnSHF5Pressed();
        virtual void OnF6Pressed();
        virtual bool SetModel(GUI::ModelC* pModel);
        virtual void OnActiveDialogClosed(int lDlgID, int lExitCode);
        void OnActiveControllerClosed(int lCtrlID, int lExitCode);
    private:
        DCP06ViewCdfControllerC(const DCP06ViewCdfControllerC&) { USER_APP_VERIFY(false); }
        DCP06ViewCdfControllerC& operator=(const DCP06ViewCdfControllerC&) { USER_APP_VERIFY(false); return *this; }
        DCP06ViewCdfDlgC* m_pDlg;
        CdfFileFunc* m_pFileFunc;
        DCP06ModelC* m_pDCP06Model;
    };

    class DCP06ViewCdfDlgC : public GUI::TableDialogC, public GUI::ModelHandlerC
    {
    public:
        enum CI_ColumnId { CI_No = 1, CI_DistId, CI_Dist, CI_Note };
        DCP06ViewCdfDlgC(CdfFileFunc* pFileFunc, DCP06ModelC* pDCP06Model);
        virtual ~DCP06ViewCdfDlgC();
        virtual void OnInitDialog(void);
        virtual void OnDialogActivated();
        virtual void UpdateData();
        virtual bool SetModel(GUI::ModelC* pModel);
        DCP06ModelC* GetDCP06Model() const;
        virtual void RefreshControls();
        bool DeletePoint();
        bool GetSelectedData(StringC& dDistId, StringC& sRefId, StringC& sNote);
    protected:
        GUI::ListMultiColCtrlC* poMultiColCtrl;
        DCP06ModelC* m_pDCP06Model;
    private:
        StringC sActualSelected;
        StringC sActualNonSelected;
        StringC sDesignSelected;
        StringC sDesignNonSelected;
        short m_iSelectedCount;
        StringC m_strMaxPointSelected;
        DCP06CommonC* m_pCommon;
        CdfFileFunc* m_pFileFunc;
    };
}

#endif // DCP_CALCULATION_VIEW_HPP
