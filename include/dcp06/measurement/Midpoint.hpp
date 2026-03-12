// ====================================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: Midpoint setup dialog (like Circle)
//
// ================================================================================================

#ifndef DCP_MIDPOINT_DLG_HPP
#define DCP_MIDPOINT_DLG_HPP

#include <dcp06/core/Types.hpp>
#include <dcp06/core/PointBuffModel.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/core/Common.hpp>

#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
#include <OBS_Observer.hpp>

namespace DCP
{
    class Model;
    class MidpointDialog;
    class MidpointModel;

    class MidpointDialog : public GUI::StandardDialogC, public GUI::ModelHandlerC, public OBS::CommandC
    {
    public:
        enum eCtrlId
        {
            eMidpointId = 1
        };

        MidpointDialog(Model *pModel, MidpointModel* pMidpointModel);

        virtual ~MidpointDialog();

        virtual void OnInitDialog(void);
        virtual void OnDialogActivated();
        virtual void UpdateData();
        virtual void RefreshControls();
        bool LoadMidpointFromDb(const std::string& midpointId);

        virtual bool SetModel(GUI::ModelC* pModel);
        PointBuffModel* GetPointBuffModelModel() const;
        bool GetMidpointIdString(char* buf, size_t bufLen) const;

    protected:
        Model * m_pModel;
        GUI::ComboLineCtrlC* m_pMidpointId;
        MidpointModel* m_pDataModel;

    private:
        OBS_DECLARE_EXECUTE(MidpointDialog);
        OBS::ObserverC m_pMidpointIdEditObserver;
        virtual void OnValueChanged(int unNotifyCode, int ulParam2);
    };

    class MidpointModel : public GUI::ModelC
    {
    public:
        MidpointModel(Model* pModel);
        virtual ~MidpointModel();

        void clear_midpoint();
        void delete_midpoint();
        Model* m_pModel;
        Common* pCommon;
        MsgBox* pMsgBox;
        char midpoint_id[POINT_ID_BUFF_LEN];
    };
}

#endif // DCP_MIDPOINT_DLG_HPP
