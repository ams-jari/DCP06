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

// ========== DCP06EditCalcAngleDlgC ==========
DCP::DCP06EditCalcAngleDlgC::DCP06EditCalcAngleDlgC(DCP::DCP06ModelC* pDCP06Model)
    : GUI::ModelHandlerC(), GUI::StandardDialogC(), m_pDistId(0), m_pRefId(0), m_pTrgtId(0), m_pNote(0), m_pDCP06Model(pDCP06Model), m_pCommon(0) {}

DCP::DCP06EditCalcAngleDlgC::~DCP06EditCalcAngleDlgC()
{
    if (m_pCommon) { delete m_pCommon; m_pCommon = 0; }
}

void DCP::DCP06EditCalcAngleDlgC::OnInitDialog(void)
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

void DCP::DCP06EditCalcAngleDlgC::OnDialogActivated()
{
    m_pCommon = new DCP06CommonC(m_pDCP06Model);
    RefreshControls();
}

void DCP::DCP06EditCalcAngleDlgC::UpdateData()
{
    GetDataModel()->sDistId = m_pDistId->GetStringInputCtrl()->GetString();
    GetDataModel()->sRefId = m_pRefId->GetStringInputCtrl()->GetString();
    GetDataModel()->sTrgtId = m_pTrgtId->GetStringInputCtrl()->GetString();
    GetDataModel()->sNote = m_pNote->GetStringInputCtrl()->GetString();
}

void DCP::DCP06EditCalcAngleDlgC::RefreshControls()
{
    if (m_pDistId && m_pRefId && m_pNote && m_pTrgtId) {
        m_pDistId->GetStringInputCtrl()->SetString(GetDataModel()->sDistId);
        m_pRefId->GetStringInputCtrl()->SetString(GetDataModel()->sRefId);
        m_pTrgtId->GetStringInputCtrl()->SetString(GetDataModel()->sTrgtId);
        m_pNote->GetStringInputCtrl()->SetString(GetDataModel()->sNote);
    }
}

bool DCP::DCP06EditCalcAngleDlgC::SetModel(GUI::ModelC* pModel)
{
    DCP::DCP06EditCalcAngleModelC* p = dynamic_cast<DCP::DCP06EditCalcAngleModelC*>(pModel);
    if (p != NULL && ModelHandlerC::SetModel(pModel)) { RefreshControls(); return true; }
    USER_APP_VERIFY(false); return false;
}

DCP::DCP06EditCalcAngleModelC* DCP::DCP06EditCalcAngleDlgC::GetDataModel() const { return (DCP::DCP06EditCalcAngleModelC*)GetModel(); }

// ========== DCP06EditCalcAngleControllerC ==========
DCP::DCP06EditCalcAngleControllerC::DCP06EditCalcAngleControllerC(DCP::DCP06ModelC* pDCP06Model)
    : m_pDlg(NULL), m_pDCP06Model(pDCP06Model), m_pCommon(0)
{
    SetTitle(StringC(AT_DCP06, T_DCP_CALC_ANGLE_EDIT_TOK));
    m_pDlg = new DCP::DCP06EditCalcAngleDlgC(pDCP06Model);
    (void)AddDialog(XORYORZ_DLG, m_pDlg, true);
    m_pCommon = new DCP06CommonC(pDCP06Model);
    FKDef vDef; vDef.poOwner = this; vDef.strLable = StringC(AT_DCP06, K_DCP_CONT_TOK); SetFunctionKey(FK1, vDef);
    FKDef vDef1; vDef1.poOwner = this; vDef1.strLable = L" "; SetFunctionKey(SHFK6, vDef1);
}

DCP::DCP06EditCalcAngleControllerC::~DCP06EditCalcAngleControllerC() { if (m_pCommon) { delete m_pCommon; m_pCommon = 0; } }
bool DCP::DCP06EditCalcAngleControllerC::SetModel(GUI::ModelC* pModel) { (void)ControllerC::SetModel(pModel); return m_pDlg->SetModel(pModel); }
void DCP::DCP06EditCalcAngleControllerC::OnF1Pressed() { m_pDlg->UpdateData(); (void)Close(EC_KEY_CONT); }
void DCP::DCP06EditCalcAngleControllerC::OnActiveDialogClosed(int, int) {}
void DCP::DCP06EditCalcAngleControllerC::OnActiveControllerClosed(int lCtrlID, int lExitCode) { m_pDlg->RefreshControls(); DestroyController(lCtrlID); }

// ========== DCP06EditCalcDistDlgC ==========
DCP::DCP06EditCalcDistDlgC::DCP06EditCalcDistDlgC(DCP::DCP06ModelC* pDCP06Model)
    : GUI::ModelHandlerC(), GUI::StandardDialogC(), m_pDistId(0), m_pRefId(0), m_pNote(0), m_pDCP06Model(pDCP06Model), m_pCommon(0) {}

DCP::DCP06EditCalcDistDlgC::~DCP06EditCalcDistDlgC()
{
    if (m_pCommon) { delete m_pCommon; m_pCommon = 0; }
}

void DCP::DCP06EditCalcDistDlgC::OnInitDialog(void)
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

void DCP::DCP06EditCalcDistDlgC::OnDialogActivated()
{
    m_pCommon = new DCP06CommonC(m_pDCP06Model);
    RefreshControls();
}

void DCP::DCP06EditCalcDistDlgC::UpdateData()
{
    GetDataModel()->sDistId = m_pDistId->GetStringInputCtrl()->GetString();
    GetDataModel()->sRefId = m_pRefId->GetStringInputCtrl()->GetString();
    GetDataModel()->sNote = m_pNote->GetStringInputCtrl()->GetString();
}

void DCP::DCP06EditCalcDistDlgC::RefreshControls()
{
    if (m_pDistId && m_pRefId && m_pNote) {
        m_pDistId->GetStringInputCtrl()->SetString(GetDataModel()->sDistId);
        m_pRefId->GetStringInputCtrl()->SetString(GetDataModel()->sRefId);
        m_pNote->GetStringInputCtrl()->SetString(GetDataModel()->sNote);
    }
}

bool DCP::DCP06EditCalcDistDlgC::SetModel(GUI::ModelC* pModel)
{
    DCP::DCP06EditCalcDistModelC* p = dynamic_cast<DCP::DCP06EditCalcDistModelC*>(pModel);
    if (p != NULL && ModelHandlerC::SetModel(pModel)) { RefreshControls(); return true; }
    USER_APP_VERIFY(false); return false;
}

DCP::DCP06EditCalcDistModelC* DCP::DCP06EditCalcDistDlgC::GetDataModel() const { return (DCP::DCP06EditCalcDistModelC*)GetModel(); }

// ========== DCP06EditCalcDistControllerC ==========
DCP::DCP06EditCalcDistControllerC::DCP06EditCalcDistControllerC(DCP::DCP06ModelC* pDCP06Model)
    : m_pDlg(NULL), m_pDCP06Model(pDCP06Model), m_pCommon(0)
{
    SetTitle(StringC(AT_DCP06, T_DCP_CALC_DIST_EDIT_TOK));
    m_pDlg = new DCP::DCP06EditCalcDistDlgC(pDCP06Model);
    (void)AddDialog(XORYORZ_DLG, m_pDlg, true);
    m_pCommon = new DCP06CommonC(pDCP06Model);
    FKDef vDef; vDef.poOwner = this; vDef.strLable = StringC(AT_DCP06, K_DCP_CONT_TOK); SetFunctionKey(FK1, vDef);
    FKDef vDef1; vDef1.poOwner = this; vDef1.strLable = L" "; SetFunctionKey(SHFK6, vDef1);
}

DCP::DCP06EditCalcDistControllerC::~DCP06EditCalcDistControllerC() { if (m_pCommon) { delete m_pCommon; m_pCommon = 0; } }
bool DCP::DCP06EditCalcDistControllerC::SetModel(GUI::ModelC* pModel) { (void)ControllerC::SetModel(pModel); return m_pDlg->SetModel(pModel); }
void DCP::DCP06EditCalcDistControllerC::OnF1Pressed() { m_pDlg->UpdateData(); (void)Close(EC_KEY_CONT); }
void DCP::DCP06EditCalcDistControllerC::OnActiveDialogClosed(int, int) {}
void DCP::DCP06EditCalcDistControllerC::OnActiveControllerClosed(int lCtrlID, int lExitCode) { m_pDlg->RefreshControls(); DestroyController(lCtrlID); }
