// ================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: SelectMidpoint - midpoint list dialog
//
// ================================================================================================

#include "stdafx.h"
#include <dcp06/core/Model.hpp>
#include <dcp06/core/SelectMidpoint.hpp>
#include <dcp06/core/Defs.hpp>
#include <UTL_StringFunctions.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace DCP;

// ================================================================================================
// SelectMidpointDialog
// ================================================================================================

SelectMidpointDialog::SelectMidpointDialog() : poMultiColCtrl(nullptr)
{
    sCoordsLabel.LoadTxt(AT_DCP06, P_DCP_COORDS_TOK);
}

SelectMidpointDialog::~SelectMidpointDialog()
{
}

void SelectMidpointDialog::OnInitDialog(void)
{
    GUI::TableDialogC::OnInitDialog();

    poMultiColCtrl = new GUI::ListMultiColCtrlC();
    poMultiColCtrl->DisableSearch();
    poMultiColCtrl->Sort(GUI::ListMultiColCtrlC::SORT_Disabled);

    USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06, P_DCP_POINT_NO_TOK), MI_No));
    USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06, P_DCP_MIDPOINT_ID_TOK), MI_MidpointId));
    USER_APP_VERIFY(poMultiColCtrl->AddCol(sCoordsLabel, MI_Coords));

    poMultiColCtrl->AddColSelection(MI_No, MI_MidpointId, MI_Coords);
    USER_APP_VERIFY(poMultiColCtrl->SetColSelection(0));
    AddTable(poMultiColCtrl);
}

void SelectMidpointDialog::OnDialogActivated()
{
    GUI::TableDialogC::OnDialogActivated();

    DCP::SelectMidpointModel* pModel = GetDataModel();

    StringC sTitle;
    sTitle.LoadTxt(AT_DCP06, T_DCP_SELECT_MIDPOINT_TOK);
    char count_str[20];
    sprintf(count_str, "(%d)", pModel->m_iCounts);
    sTitle += StringC(count_str);
    SetTitle(sTitle);

    for (int i = 0; i < pModel->m_iCounts; i++)
    {
        char rowStr[32];
        sprintf(rowStr, "%d", i + 1);
        USER_APP_VERIFY(poMultiColCtrl->AddRow((short)(i + 1)));
        USER_APP_VERIFY(poMultiColCtrl->SetCellText(MI_No, (short)(i + 1), rowStr));
        USER_APP_VERIFY(poMultiColCtrl->SetCellText(MI_MidpointId, (short)(i + 1), pModel->midpoints[i].midpoint_id));
        USER_APP_VERIFY(poMultiColCtrl->SetCellText(MI_Coords, (short)(i + 1), pModel->midpoints[i].coords));
    }

    if (pModel->m_iSelectedId > 0 && pModel->m_iSelectedId <= pModel->m_iCounts)
        poMultiColCtrl->SetSelectedId(pModel->m_iSelectedId);
    else
        poMultiColCtrl->SetSelectedId(1);
}

void SelectMidpointDialog::UpdateData()
{
    short iSelected = poMultiColCtrl->GetSelectedId();
    StringC strSelectedId;
    poMultiColCtrl->GetCellText(MI_MidpointId, iSelected, strSelectedId);
    GetDataModel()->m_iSelectedId = iSelected;
    GetDataModel()->m_strSelectedMidpointId = strSelectedId;
}

bool SelectMidpointDialog::SetModel(GUI::ModelC* pModel)
{
    DCP::SelectMidpointModel* pDcpModel = dynamic_cast<DCP::SelectMidpointModel*>(pModel);
    if (pDcpModel != nullptr && ModelHandlerC::SetModel(pDcpModel))
    {
        RefreshControls();
        return true;
    }
    USER_APP_VERIFY(false);
    return false;
}

SelectMidpointModel* SelectMidpointDialog::GetDataModel() const
{
    return (SelectMidpointModel*)GetModel();
}

// ================================================================================================
// SelectMidpointController
// ================================================================================================

SelectMidpointController::SelectMidpointController() : m_pDlg(nullptr)
{
    SetTitle(StringC(AT_DCP06, T_DCP_SELECT_MIDPOINT_TOK));
    m_pDlg = new DCP::SelectMidpointDialog;
    (void)AddDialog(SELECT_MIDPOINT_DLG, m_pDlg, true);

    FKDef vDef;
    vDef.poOwner = this;
    vDef.strLable = StringC(AT_DCP06, K_DCP_CONT_TOK);
    SetFunctionKey(FK1, vDef);

    FKDef vDef1;
    vDef1.poOwner = this;
    vDef1.strLable = L" ";
    SetFunctionKey(SHFK6, vDef1);
}

SelectMidpointController::~SelectMidpointController()
{
}

bool SelectMidpointController::SetModel(GUI::ModelC* pModel)
{
    (void)ControllerC::SetModel(pModel);
    return m_pDlg->SetModel(pModel);
}

void SelectMidpointController::OnF1Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY(false);
        return;
    }
    m_pDlg->UpdateData();
    (void)Close(EC_KEY_CONT);
}

void SelectMidpointController::OnActiveDialogClosed(int /*lDlgID*/, int /*lExitCode*/)
{
}

void SelectMidpointController::OnActiveControllerClosed(int lCtrlID, int lExitCode)
{
    m_pDlg->RefreshControls();
    DestroyController(lCtrlID);
}

// ================================================================================================
// SelectMidpointModel
// ================================================================================================

SelectMidpointModel::SelectMidpointModel()
{
    m_iSelectedId = -1;
    m_strSelectedMidpointId = L"";
    memset(&midpoints[0], 0, sizeof(S_SELECT_MIDPOINT) * MAX_SELECT_MIDPOINTS);
}

SelectMidpointModel::~SelectMidpointModel()
{
}
