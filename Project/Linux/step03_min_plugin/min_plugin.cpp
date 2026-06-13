// Step 3: minimal Captivate plugin — exports Start15751 (same App.Id as DCP06.sys).

#include "stdafx.h"

#include "min_plugin_tok.hpp"

#include <ABL_LoadableApps.hpp>
#include <GUI_Application.hpp>
#include <GUI_AppBase.hpp>
#include <GUI_GController.hpp>
#include <GUI_GModel.hpp>
#include <GUI_StandardDialog.hpp>
#include <Onboard_TOK.HPP>

namespace DCP06Stub
{
    enum ApplTypeT
    {
        AT_DCP06 = 15751
    };

    class StubModelC : public GUI::GModelC
    {
    public:
        StubModelC() {}
        virtual ~StubModelC() {}
    };

    class StubDialogC : public GUI::StandardDialogC
    {
    public:
        StubDialogC() {}
        virtual ~StubDialogC() {}
    };

    class StubControllerC : public GUI::GControllerC
    {
    public:
        enum { DLG_MAIN = 1, CTL_MAIN = 1 };

        explicit StubControllerC(StubModelC* pModel)
            : m_pModel(pModel)
        {
            USER_APP_ASSERT(m_pModel != NULL);
            USER_APP_VERIFY(SetModel(m_pModel));

            SetTitle(StringC(AT_DCP06, L_DCP_STUB_APP_NAME_TOK));

            StubDialogC* pDlg = new StubDialogC();
            (void)AddDialog(DLG_MAIN, pDlg, true);

            FKDef keyDef;
            keyDef.poOwner = this;
            keyDef.strLable = StringC(0, K_SET_TOK);
            SetFunctionKey(FK1, keyDef);
        }

        virtual void OnF1Pressed()
        {
            (void)Close(EC_KEY_CONT);
        }

    private:
        StubModelC* m_pModel;
    };

    class StubApplicationC : public GUI::ApplicationC, public GUI::ModelHandlerC
    {
    public:
        StubApplicationC()
        {
            StubModelC* pModel = new StubModelC;
            USER_APP_VERIFY(SetModel(pModel));
            (void)AddController(StubControllerC::CTL_MAIN, new StubControllerC(pModel));
            SetTxtApplicationId(AT_DCP06);
        }

        virtual ~StubApplicationC() {}

        virtual void Run()
        {
            (void)SetActiveController(StubControllerC::CTL_MAIN, false);
            GUI::ApplicationC::Run();
        }

        virtual void OnActiveControllerClosed(int /*lDlgID*/, int /*lExitCode*/)
        {
            (void)Close();
        }
    };

    APP_ENTRY_POINT(AT_DCP06, 15751, StubApplicationC)
}

using namespace DCP06Stub;

namespace ABL
{
    REGISTER_COMMAND_FACTORY_APPLICATION(
        StubApplicationC,
        AT_DCP06,
        C_DCP_STUB_APP_NAME_TOK,
        L_DCP_STUB_APP_NAME_TOK,
        AT_DCP06);
}
