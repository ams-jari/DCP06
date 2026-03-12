// ====================================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: SelectMidpoint - midpoint list dialog (like SelectCircle)
//
// ================================================================================================

#ifndef DCP_SELECTMIDPOINT_DLG_HPP
#define DCP_SELECTMIDPOINT_DLG_HPP

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
    class SelectMidpointDialog;
    class SelectMidpointModel;

    class SelectMidpointController : public GUI::ControllerC
    {
    public:
        SelectMidpointController();
        ~SelectMidpointController();

        virtual void OnF1Pressed();

        virtual bool SetModel( GUI::ModelC* pModel );

        virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
        void OnActiveControllerClosed( int lCtrlID, int lExitCode );

    private:
        SelectMidpointController( const SelectMidpointController& ) { USER_APP_VERIFY( false ); }
        SelectMidpointController& operator=( const SelectMidpointController& ) {
            USER_APP_VERIFY( false ); return *this;
        }

        SelectMidpointDialog* m_pDlg;
    };


    class SelectMidpointDialog : public GUI::TableDialogC, public GUI::ModelHandlerC
    {
    public:
        enum MI_ColumnId
        {
            MI_No = 1,
            MI_MidpointId,
            MI_Coords
        };

        SelectMidpointDialog();
        virtual ~SelectMidpointDialog();

        virtual void OnInitDialog(void);
        virtual void OnDialogActivated();
        virtual void UpdateData();
        virtual bool SetModel( GUI::ModelC* pModel );

        SelectMidpointModel* GetDataModel() const;

    protected:
        GUI::ListMultiColCtrlC* poMultiColCtrl;

    private:
        StringC sCoordsLabel;
    };

    class SelectMidpointModel : public GUI::ModelC
    {
    public:
        SelectMidpointModel();
        virtual ~SelectMidpointModel();

        S_SELECT_MIDPOINT midpoints[MAX_SELECT_MIDPOINTS];
        short m_iCounts;
        StringC m_strSelectedMidpointId;
        short m_iSelectedId;
    };
}

#endif // DCP_SELECTMIDPOINT_DLG_HPP
