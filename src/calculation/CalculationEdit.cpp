// ================================================================================================
// CalculationEdit.cpp - Edit dialogs for Angle and Distance calculation
// Consolidated from EditCalcAngle.cpp and EditCalcDist.cpp
// ================================================================================================

#include "stdafx.h"
#include <dcp06/core/Model.hpp>
#include <dcp06/init/Initialization.hpp>
#include <dcp06/calculation/CalculationEdit.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/core/Tool.hpp>
#include <dcp06/core/Common.hpp>
#include <GUI_Types.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ========== EditCalculationAngleDialog ==========
DCP::EditCalculationAngleDialog::EditCalculationAngleDialog(DCP::Model* pModel)
    : GUI::ModelHandlerC(), GUI::StandardDialogC(), m_pDistId(0), m_pRefId(0), m_pTrgtId(0), m_pNote(0), m_pModel(pModel), m_pCommon(0) {}

DCP::EditCalculationAngleDialog::~EditCalculationAngleDialog()
{
    if (m_pCommon) { delete m_pCommon; m_pCommon = 0; }
}

void DCP::EditCalculationAngleDialog::OnInitDialog(void)
{
    GUI::BaseDialogC::OnInitDialog();
    m_pDistId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
    m_pDistId->SetId(eDistId); m_pDistId->SetText(StringC(AT_DCP06, P_DCP_ANGLE_ID_TOK));
    m_pDistId->GetStringInputCtrl()->SetCharsCountMax(6); m_pDistId->SetEmptyAllowed(true); AddCtrl(m_pDistId);
    m_pRefId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
    m_pRefId->SetId(eRefId); m_pRefId->SetText(StringC(AT_DCP06, P_DCP_REF_ID_TOK));
    m_pRefId->GetStringInputCtrl()->SetCharsCountMax(6); m_pRefId->SetEmptyAllowed(true); AddCtrl(m_pRefId);
    m_pTrgtId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
    m_pTrgtId->SetId(eTrgtId); m_pTrgtId->SetText(StringC(AT_DCP06, P_DCP_TARGET_ID_TOK));
    m_pTrgtId->GetStringInputCtrl()->SetCharsCountMax(6); m_pTrgtId->SetEmptyAllowed(true); AddCtrl(m_pTrgtId);
    m_pNote = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
    m_pNote->SetId(eNote); m_pNote->SetText(StringC(AT_DCP06, P_DCP_NOTE_TOK));
    m_pNote->GetStringInputCtrl()->SetCharsCountMax(8); m_pNote->SetEmptyAllowed(true); AddCtrl(m_pNote);
}

void DCP::EditCalculationAngleDialog::OnDialogActivated()
{
    m_pCommon = new Common(m_pModel);
    RefreshControls();
}

void DCP::EditCalculationAngleDialog::UpdateData()
{
    GetDataModel()->sDistId = m_pDistId->GetStringInputCtrl()->GetString();
    GetDataModel()->sRefId = m_pRefId->GetStringInputCtrl()->GetString();
    GetDataModel()->sTrgtId = m_pTrgtId->GetStringInputCtrl()->GetString();
    GetDataModel()->sNote = m_pNote->GetStringInputCtrl()->GetString();
}

void DCP::EditCalculationAngleDialog::RefreshControls()
{
    if (m_pDistId && m_pRefId && m_pNote && m_pTrgtId) {
        m_pDistId->GetStringInputCtrl()->SetString(GetDataModel()->sDistId);
        m_pRefId->GetStringInputCtrl()->SetString(GetDataModel()->sRefId);
        m_pTrgtId->GetStringInputCtrl()->SetString(GetDataModel()->sTrgtId);
        m_pNote->GetStringInputCtrl()->SetString(GetDataModel()->sNote);
    }
}

bool DCP::EditCalculationAngleDialog::SetModel(GUI::ModelC* pModel)
{
    DCP::EditCalculationAngleModel* p = dynamic_cast<DCP::EditCalculationAngleModel*>(pModel);
    if (p != nullptr && ModelHandlerC::SetModel(pModel)) { RefreshControls(); return true; }
    USER_APP_VERIFY(false); return false;
}

DCP::EditCalculationAngleModel* DCP::EditCalculationAngleDialog::GetDataModel() const { return (DCP::EditCalculationAngleModel*)GetModel(); }

// ========== EditCalculationAngleController ==========
DCP::EditCalculationAngleController::EditCalculationAngleController(DCP::Model* pModel)
    : m_pDlg(nullptr), m_pModel(pModel), m_pCommon(0)
{
    SetTitle(StringC(AT_DCP06, T_DCP_CALC_ANGLE_EDIT_TOK));
    m_pDlg = new DCP::EditCalculationAngleDialog(pModel);
    (void)AddDialog(XORYORZ_DLG, m_pDlg, true);
    m_pCommon = new Common(pModel);
    FKDef vDef; vDef.poOwner = this; vDef.strLable = StringC(AT_DCP06, K_DCP_CONT_TOK); SetFunctionKey(FK1, vDef);
    FKDef vDef1; vDef1.poOwner = this; vDef1.strLable = L" "; SetFunctionKey(SHFK6, vDef1);
}

DCP::EditCalculationAngleController::~EditCalculationAngleController() { if (m_pCommon) { delete m_pCommon; m_pCommon = 0; } }
bool DCP::EditCalculationAngleController::SetModel(GUI::ModelC* pModel) { (void)ControllerC::SetModel(pModel); return m_pDlg->SetModel(pModel); }
void DCP::EditCalculationAngleController::OnF1Pressed() { m_pDlg->UpdateData(); (void)Close(EC_KEY_CONT); }
void DCP::EditCalculationAngleController::OnActiveDialogClosed(int, int) {}
void DCP::EditCalculationAngleController::OnActiveControllerClosed(int lCtrlID, int lExitCode) { m_pDlg->RefreshControls(); DestroyController(lCtrlID); }

// ========== EditCalculationDistDialog ==========
DCP::EditCalculationDistDialog::EditCalculationDistDialog(DCP::Model* pModel)
    : GUI::ModelHandlerC(), GUI::StandardDialogC(), m_pDistId(0), m_pRefId(0), m_pNote(0), m_pModel(pModel), m_pCommon(0) {}

DCP::EditCalculationDistDialog::~EditCalculationDistDialog()
{
    if (m_pCommon) { delete m_pCommon; m_pCommon = 0; }
}

void DCP::EditCalculationDistDialog::OnInitDialog(void)
{
    GUI::BaseDialogC::OnInitDialog();
    m_pDistId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
    m_pDistId->SetId(eDistId); m_pDistId->SetText(StringC(AT_DCP06, P_DCP_DISTANCE_ID_TOK));
    m_pDistId->GetStringInputCtrl()->SetCharsCountMax(6); m_pDistId->SetEmptyAllowed(true); AddCtrl(m_pDistId);
    m_pRefId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
    m_pRefId->SetId(eRefId); m_pRefId->SetText(StringC(AT_DCP06, P_DCP_REF_ID_TOK));
    m_pRefId->GetStringInputCtrl()->SetCharsCountMax(6); m_pRefId->SetEmptyAllowed(true); AddCtrl(m_pRefId);
    m_pNote = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
    m_pNote->SetId(eNote); m_pNote->SetText(StringC(AT_DCP06, P_DCP_NOTE_TOK));
    m_pNote->GetStringInputCtrl()->SetCharsCountMax(8); m_pNote->SetEmptyAllowed(true); AddCtrl(m_pNote);
}

void DCP::EditCalculationDistDialog::OnDialogActivated()
{
    m_pCommon = new Common(m_pModel);
    RefreshControls();
}

void DCP::EditCalculationDistDialog::UpdateData()
{
    GetDataModel()->sDistId = m_pDistId->GetStringInputCtrl()->GetString();
    GetDataModel()->sRefId = m_pRefId->GetStringInputCtrl()->GetString();
    GetDataModel()->sNote = m_pNote->GetStringInputCtrl()->GetString();
}

void DCP::EditCalculationDistDialog::RefreshControls()
{
    if (m_pDistId && m_pRefId && m_pNote) {
        m_pDistId->GetStringInputCtrl()->SetString(GetDataModel()->sDistId);
        m_pRefId->GetStringInputCtrl()->SetString(GetDataModel()->sRefId);
        m_pNote->GetStringInputCtrl()->SetString(GetDataModel()->sNote);
    }
}

bool DCP::EditCalculationDistDialog::SetModel(GUI::ModelC* pModel)
{
    DCP::EditCalculationDistModel* p = dynamic_cast<DCP::EditCalculationDistModel*>(pModel);
    if (p != nullptr && ModelHandlerC::SetModel(pModel)) { RefreshControls(); return true; }
    USER_APP_VERIFY(false); return false;
}

DCP::EditCalculationDistModel* DCP::EditCalculationDistDialog::GetDataModel() const { return (DCP::EditCalculationDistModel*)GetModel(); }

// ========== EditCalculationDistController ==========
DCP::EditCalculationDistController::EditCalculationDistController(DCP::Model* pModel)
    : m_pDlg(nullptr), m_pModel(pModel), m_pCommon(0)
{
    SetTitle(StringC(AT_DCP06, T_DCP_CALC_DIST_EDIT_TOK));
    m_pDlg = new DCP::EditCalculationDistDialog(pModel);
    (void)AddDialog(XORYORZ_DLG, m_pDlg, true);
    m_pCommon = new Common(pModel);
    FKDef vDef; vDef.poOwner = this; vDef.strLable = StringC(AT_DCP06, K_DCP_CONT_TOK); SetFunctionKey(FK1, vDef);
    FKDef vDef1; vDef1.poOwner = this; vDef1.strLable = L" "; SetFunctionKey(SHFK6, vDef1);
}

DCP::EditCalculationDistController::~EditCalculationDistController() { if (m_pCommon) { delete m_pCommon; m_pCommon = 0; } }
bool DCP::EditCalculationDistController::SetModel(GUI::ModelC* pModel) { (void)ControllerC::SetModel(pModel); return m_pDlg->SetModel(pModel); }
void DCP::EditCalculationDistController::OnF1Pressed() { m_pDlg->UpdateData(); (void)Close(EC_KEY_CONT); }
void DCP::EditCalculationDistController::OnActiveDialogClosed(int, int) {}
void DCP::EditCalculationDistController::OnActiveControllerClosed(int lCtrlID, int lExitCode) { m_pDlg->RefreshControls(); DestroyController(lCtrlID); }
