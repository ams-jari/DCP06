// ====================================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: SelectCircle - circle list dialog (like SelectPoint)
//
// ================================================================================================

#ifndef DCP_SELECTCIRCLE_DLG_HPP
#define DCP_SELECTCIRCLE_DLG_HPP

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Defs.hpp>

#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_TableDialog.hpp>
#include <GUI_ListMultiColCtrl.hpp>
#include <GUI_AppBase.hpp>

namespace DCP
{

    class Model;
    class SelectCircleDialog;
    class SelectCircleModel;

    class SelectCircleController : public GUI::ControllerC
    {
    public:
        SelectCircleController();
        ~SelectCircleController();

        virtual void OnF1Pressed();

        virtual bool SetModel( GUI::ModelC* pModel );

        virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
        void OnActiveControllerClosed( int lCtrlID, int lExitCode );

    private:
        SelectCircleController( const SelectCircleController& ) { USER_APP_VERIFY( false ); }
        SelectCircleController& operator=( const SelectCircleController& ) {
            USER_APP_VERIFY( false ); return *this;
        }

        SelectCircleDialog* m_pDlg;
    };


    class SelectCircleDialog : public GUI::TableDialogC, public GUI::ModelHandlerC
    {
    public:
        enum CI_ColumnId
        {
            CI_No = 1,
            CI_CircleId,
            CI_Center,
            CI_Diameter
        };

        SelectCircleDialog();
        virtual ~SelectCircleDialog();

        virtual void OnInitDialog(void);
        virtual void OnDialogActivated();
        virtual void UpdateData();
        virtual bool SetModel( GUI::ModelC* pModel );

        SelectCircleModel* GetDataModel() const;

    protected:
        GUI::ListMultiColCtrlC* poMultiColCtrl;

    private:
        StringC sCenterLabel;
        StringC sDiameterLabel;
    };

    class SelectCircleModel : public GUI::ModelC
    {
    public:
        SelectCircleModel();
        virtual ~SelectCircleModel();

        S_SELECT_CIRCLE circles[MAX_SELECT_CIRCLES];
        short m_iCounts;
        StringC m_strSelectedCircleId;
        short m_iSelectedId;
    };
}

#endif // DCP_SELECTCIRCLE_DLG_HPP
