// ================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: SelectCircle - circle list dialog
//
// ================================================================================================

#include "stdafx.h"
#include <dcp06/core/Model.hpp>
#include <dcp06/core/SelectCircle.hpp>
#include <dcp06/core/Defs.hpp>
#include <UTL_StringFunctions.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace DCP;

// ================================================================================================
// SelectCircleDialog
// ================================================================================================

SelectCircleDialog::SelectCircleDialog() : poMultiColCtrl(nullptr)
{
    sCenterLabel.LoadTxt(AT_DCP06, P_DCP_CENTER_OF_CIRCLE_TOK);
    sDiameterLabel.LoadTxt(AT_DCP06, P_DCP_DIAMETER_TOK);
}

SelectCircleDialog::~SelectCircleDialog()
{
}

void SelectCircleDialog::OnInitDialog(void)
{
    GUI::TableDialogC::OnInitDialog();

    poMultiColCtrl = new GUI::ListMultiColCtrlC();
    poMultiColCtrl->DisableSearch();
    poMultiColCtrl->Sort(GUI::ListMultiColCtrlC::SORT_Disabled);

    USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06, P_DCP_POINT_NO_TOK), CI_No));
    USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06, P_DCP_CIRCLE_ID_TOK), CI_CircleId));
    USER_APP_VERIFY(poMultiColCtrl->AddCol(sCenterLabel, CI_Center));
    USER_APP_VERIFY(poMultiColCtrl->AddCol(sDiameterLabel, CI_Diameter));

    poMultiColCtrl->AddColSelection(CI_No, CI_CircleId, CI_Center, CI_Diameter);
    USER_APP_VERIFY(poMultiColCtrl->SetColSelection(0));
    AddTable(poMultiColCtrl);
}

void SelectCircleDialog::OnDialogActivated()
{
    GUI::TableDialogC::OnDialogActivated();

    DCP::SelectCircleModel* pModel = GetDataModel();

    StringC sTitle;
    sTitle.LoadTxt(AT_DCP06, T_DCP_SELECT_CIRCLE_TOK);
    char count_str[20];
    sprintf(count_str, "(%d)", pModel->m_iCounts);
    sTitle += StringC(count_str);
    SetTitle(sTitle);

    char rowStr[32];
    for (int i = 0; i < pModel->m_iCounts; i++)
    {
        sprintf(rowStr, "%d", i + 1);
        USER_APP_VERIFY(poMultiColCtrl->AddRow((short)(i + 1)));
        USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_No, (short)(i + 1), rowStr));
        USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_CircleId, (short)(i + 1), pModel->circles[i].circle_id));
        USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Center, (short)(i + 1), pModel->circles[i].center));
        USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Diameter, (short)(i + 1), pModel->circles[i].diameter));
    }

    if (pModel->m_iSelectedId > 0 && pModel->m_iSelectedId <= pModel->m_iCounts)
        poMultiColCtrl->SetSelectedId(pModel->m_iSelectedId);
    else
        poMultiColCtrl->SetSelectedId(1);
}

void SelectCircleDialog::UpdateData()
{
    short iSelected = poMultiColCtrl->GetSelectedId();
    StringC strSelectedId;
    poMultiColCtrl->GetCellText(CI_CircleId, iSelected, strSelectedId);
    GetDataModel()->m_iSelectedId = iSelected;
    GetDataModel()->m_strSelectedCircleId = strSelectedId;
}

bool SelectCircleDialog::SetModel(GUI::ModelC* pModel)
{
    DCP::SelectCircleModel* pDcpModel = dynamic_cast<DCP::SelectCircleModel*>(pModel);
    if (pDcpModel != nullptr && ModelHandlerC::SetModel(pDcpModel))
    {
        RefreshControls();
        return true;
    }
    USER_APP_VERIFY(false);
    return false;
}

SelectCircleModel* SelectCircleDialog::GetDataModel() const
{
    return (SelectCircleModel*)GetModel();
}

// ================================================================================================
// SelectCircleController
// ================================================================================================

SelectCircleController::SelectCircleController() : m_pDlg(nullptr)
{
    SetTitle(StringC(AT_DCP06, T_DCP_SELECT_CIRCLE_TOK));
    m_pDlg = new DCP::SelectCircleDialog;
    (void)AddDialog(SELECT_CIRCLE_DLG, m_pDlg, true);

    FKDef vDef;
    vDef.poOwner = this;
    vDef.strLable = StringC(AT_DCP06, K_DCP_CONT_TOK);
    SetFunctionKey(FK1, vDef);

    FKDef vDef1;
    vDef1.poOwner = this;
    vDef1.strLable = L" ";
    SetFunctionKey(SHFK6, vDef1);
}

SelectCircleController::~SelectCircleController()
{
}

bool SelectCircleController::SetModel(GUI::ModelC* pModel)
{
    (void)ControllerC::SetModel(pModel);
    return m_pDlg->SetModel(pModel);
}

void SelectCircleController::OnF1Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY(false);
        return;
    }
    m_pDlg->UpdateData();
    (void)Close(EC_KEY_CONT);
}

void SelectCircleController::OnActiveDialogClosed(int /*lDlgID*/, int /*lExitCode*/)
{
}

void SelectCircleController::OnActiveControllerClosed(int lCtrlID, int lExitCode)
{
    m_pDlg->RefreshControls();
    DestroyController(lCtrlID);
}

// ================================================================================================
// SelectCircleModel
// ================================================================================================

SelectCircleModel::SelectCircleModel()
{
    m_iSelectedId = -1;
    m_strSelectedCircleId = L"";
    memset(&circles[0], 0, sizeof(S_SELECT_CIRCLE) * MAX_SELECT_CIRCLES);
}

SelectCircleModel::~SelectCircleModel()
{
}
