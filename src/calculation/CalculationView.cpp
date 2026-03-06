// ================================================================================================
// CalculationView.cpp - View dialogs for Angle (AGF) and Distance (CDF) calculation files
// Consolidated from ViewAgf.cpp and ViewCdf.cpp
// ================================================================================================

#include "stdafx.h"
#include <dcp06/core/Model.hpp>
#include <dcp06/calculation/CalculationView.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/calculation/CalculationEdit.hpp>
#include <dcp06/core/Common.hpp>

#include <GUI_Types.hpp>
#include <GUI_DeskTop.hpp>
#include <GMAT_UnitConverter.hpp>
#include <UTL_StringFunctions.hpp>
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace DCP;

// ================================================================================================
// --- DCP06ViewAgfDlgC ---
// ================================================================================================
DCP06ViewAgfDlgC::DCP06ViewAgfDlgC(DCP::AgfFileFunc* pFileFunc, DCP06ModelC* pDCP06Model)
    : poMultiColCtrl(NULL), m_pFileFunc(pFileFunc), m_iSelectedCount(0)
{
    m_pCommon = new DCP06CommonC(pDCP06Model);
}

DCP06ViewAgfDlgC::~DCP06ViewAgfDlgC()
{
    if (m_pCommon) { delete m_pCommon; m_pCommon = 0; }
}

void DCP06ViewAgfDlgC::OnInitDialog(void)
{
    GUI::TableDialogC::OnInitDialog();
    poMultiColCtrl = new GUI::ListMultiColCtrlC();
    poMultiColCtrl->DisableSearch();
    poMultiColCtrl->Sort(GUI::ListMultiColCtrlC::SORT_Disabled);
    USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06, P_DCP_POINT_NUMBER_TOK), CI_No));
    USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06, P_DCP_ANGLE_ID_TOK), CI_AngleId));
    USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06, P_DCP_ANGLE_TOK), CI_Angle));
    USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06, P_DCP_NOTE_TOK), CI_Note));
    poMultiColCtrl->AddColSelection(CI_No, CI_AngleId, CI_Angle, CI_Note);
    USER_APP_VERIFY(poMultiColCtrl->SetColSelection(0));
    AddTable(poMultiColCtrl);
}

void DCP06ViewAgfDlgC::RefreshControls()
{
    if (poMultiColCtrl) {
        char temp[20];
        poMultiColCtrl->DestroyAll();
        for (short i = 0; i < m_pFileFunc->getPointsCount(); i++) {
            sprintf(temp, "%d", i + 1);
            m_pFileFunc->calcdist_pnt(i + 1);
            USER_APP_VERIFY(poMultiColCtrl->AddRow((short)i));
            USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_No, (short)i, StringC(temp)));
            USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_AngleId, (short)i, StringC(m_pFileFunc->id)));
            USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Angle, (short)i, StringC(m_pFileFunc->dist)));
            USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Note, (short)i, StringC(m_pFileFunc->note)));
        }
    }
}

void DCP06ViewAgfDlgC::OnDialogActivated() { GUI::TableDialogC::OnDialogActivated(); }
void DCP::DCP06ViewAgfDlgC::UpdateData() {}

bool DCP::DCP06ViewAgfDlgC::DeletePoint()
{
    short iSelectedId = poMultiColCtrl->GetSelectedId();
    if (m_pFileFunc->delete_point_from_cdf(iSelectedId)) { RefreshControls(); return true; }
    return false;
}

bool DCP::DCP06ViewAgfDlgC::GetSelectedData(StringC& sDistId, StringC& sRefId, StringC& sTrgtId, StringC& sNote)
{
    short iSelectedId = poMultiColCtrl->GetSelectedId();
    if (iSelectedId == -1) return false;
    m_pFileFunc->calcdist_pnt(iSelectedId + 1);
    UTL::Trim(m_pFileFunc->id);   sDistId = StringC(m_pFileFunc->id);
    UTL::Trim(m_pFileFunc->ref);  sRefId = StringC(m_pFileFunc->ref);
    UTL::Trim(m_pFileFunc->target); sTrgtId = StringC(m_pFileFunc->target);
    UTL::Trim(m_pFileFunc->note); sNote = StringC(m_pFileFunc->note);
    return true;
}

bool DCP::DCP06ViewAgfDlgC::SetModel(GUI::ModelC* pModel)
{
    DCP::DCP06ModelC* pDCP06Model = dynamic_cast<DCP::DCP06ModelC*>(pModel);
    if (pDCP06Model != NULL && ModelHandlerC::SetModel(pDCP06Model)) { RefreshControls(); return true; }
    USER_APP_VERIFY(false);
    return false;
}

DCP::DCP06ModelC* DCP::DCP06ViewAgfDlgC::GetDCP06Model() const { return (DCP::DCP06ModelC*)GetModel(); }

// --- DCP06ViewAgfControllerC ---
DCP::DCP06ViewAgfControllerC::DCP06ViewAgfControllerC(DCP::AgfFileFunc* pFileFunc, DCP06ModelC* pDCP06Model)
    : m_pDlg(NULL), m_pFileFunc(pFileFunc), m_pDCP06Model(pDCP06Model)
{
    StringC sTitle;
    sTitle.LoadTxt(AT_DCP06, T_DCP_CALC_ANGLE_VIEW_TOK);
    sTitle += L" ("; sTitle += StringC(m_pFileFunc->getFileName()); sTitle += L")";
    SetTitle(sTitle);
    m_pDlg = new DCP::DCP06ViewAgfDlgC(m_pFileFunc, pDCP06Model);
    (void)AddDialog(VIEWAGF_DLG, m_pDlg, true);
    FKDef vDef; vDef.poOwner = this;
    vDef.strLable = StringC(AT_DCP06, K_DCP_EDIT_TOK); SetFunctionKey(FK1, vDef);
    vDef.strLable = StringC(AT_DCP06, K_DCP_DEL_TOK);  SetFunctionKey(SHFK5, vDef);
    vDef.strLable = StringC(AT_DCP06, K_DCP_CONT_TOK); SetFunctionKey(FK6, vDef);
    FKDef vDef1; vDef1.poOwner = this; vDef1.strLable = L" "; SetFunctionKey(SHFK6, vDef1);
}

DCP::DCP06ViewAgfControllerC::~DCP06ViewAgfControllerC() {}

bool DCP::DCP06ViewAgfControllerC::SetModel(GUI::ModelC* pModel)
{
    (void)ControllerC::SetModel(pModel);
    return m_pDlg->SetModel(pModel);
}

void DCP::DCP06ViewAgfControllerC::OnF1Pressed()
{
    if (m_pDlg == NULL) { USER_APP_VERIFY(false); return; }
    StringC sDistId, sRefId, sNote, sTrgtId;
    if (m_pDlg->GetSelectedData(sDistId, sRefId, sTrgtId, sNote) == false) return;
    DCP::DCP06EditCalcAngleModelC* pModel = new DCP06EditCalcAngleModelC;
    pModel->sDistId = sDistId; pModel->sRefId = sRefId; pModel->sTrgtId = sTrgtId; pModel->sNote = sNote;
    if (GetController(VIEWAGF_EDIT_CONTROLLER) == NULL)
        (void)AddController(VIEWAGF_EDIT_CONTROLLER, new DCP::DCP06EditCalcAngleControllerC(m_pDlg->GetDCP06Model()));
    (void)GetController(VIEWAGF_EDIT_CONTROLLER)->SetModel(pModel);
    SetActiveController(VIEWAGF_EDIT_CONTROLLER, true);
}

void DCP::DCP06ViewAgfControllerC::OnSHF5Pressed() { if (m_pDlg) m_pDlg->DeletePoint(); }
void DCP::DCP06ViewAgfControllerC::OnF6Pressed() { if (m_pDlg) { m_pDlg->UpdateData(); (void)Close(EC_KEY_CONT); } }
void DCP::DCP06ViewAgfControllerC::OnActiveDialogClosed(int, int) {}

void DCP::DCP06ViewAgfControllerC::OnActiveControllerClosed(int lCtrlID, int lExitCode)
{
    if (lCtrlID == VIEWAGF_EDIT_CONTROLLER && lExitCode == EC_KEY_CONT) {
        DCP::DCP06EditCalcAngleModelC* pModel = (DCP::DCP06EditCalcAngleModelC*)GetController(VIEWAGF_EDIT_CONTROLLER)->GetModel();
        DCP06CommonC pCommon(m_pDCP06Model);
        char temp[100];
        BSS::UTI::BSS_UTI_WCharToAscii(pModel->sDistId, temp, 6); pCommon.strbtrim(temp); sprintf(m_pFileFunc->id, "%-s", temp);
        BSS::UTI::BSS_UTI_WCharToAscii(pModel->sRefId, temp, 6);  pCommon.strbtrim(temp); sprintf(m_pFileFunc->ref, "%-s", temp);
        BSS::UTI::BSS_UTI_WCharToAscii(pModel->sTrgtId, temp, 6); pCommon.strbtrim(temp); sprintf(m_pFileFunc->target, "%-s", temp);
        BSS::UTI::BSS_UTI_WCharToAscii(pModel->sNote, temp, 6);   pCommon.strbtrim(temp); sprintf(m_pFileFunc->note, "%-s", temp);
        m_pFileFunc->cdf_save_pnt();
    }
    m_pDlg->RefreshControls();
    DestroyController(lCtrlID);
}

// ================================================================================================
// --- DCP06ViewCdfDlgC ---
// ================================================================================================
DCP06ViewCdfDlgC::DCP06ViewCdfDlgC(DCP::CdfFileFunc* pFileFunc, DCP06ModelC* pDCP06Model)
    : poMultiColCtrl(NULL), m_pFileFunc(pFileFunc), m_iSelectedCount(0)
{
    sActualSelected.LoadTxt(AT_DCP06, P_DCP_ACTUAL_SELECTED_TOK);
    sActualNonSelected.LoadTxt(AT_DCP06, P_DCP_ACTUAL_NONSELECTED_TOK);
    sDesignSelected.LoadTxt(AT_DCP06, P_DCP_DESIGN_SELECTED_TOK);
    sDesignNonSelected.LoadTxt(AT_DCP06, P_DCP_DESIGN_NONSELECTED_TOK);
    m_strMaxPointSelected.LoadTxt(AT_DCP06, L_DCP_MAX_POINTS_SELECTED_TOK);
    m_pCommon = new DCP06CommonC(pDCP06Model);
}

DCP06ViewCdfDlgC::~DCP06ViewCdfDlgC()
{
    if (m_pCommon) { delete m_pCommon; m_pCommon = 0; }
}

void DCP06ViewCdfDlgC::OnInitDialog(void)
{
    GUI::TableDialogC::OnInitDialog();
    poMultiColCtrl = new GUI::ListMultiColCtrlC();
    poMultiColCtrl->DisableSearch();
    poMultiColCtrl->Sort(GUI::ListMultiColCtrlC::SORT_Disabled);
    USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06, P_DCP_POINT_NUMBER_TOK), CI_No));
    USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06, P_DCP_DISTANCE_ID_TOK), CI_DistId));
    USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06, P_DCP_DIST_TOK), CI_Dist));
    USER_APP_VERIFY(poMultiColCtrl->AddCol(StringC(AT_DCP06, P_DCP_NOTE_TOK), CI_Note));
    poMultiColCtrl->AddColSelection(CI_No, CI_DistId, CI_Dist, CI_Note);
    USER_APP_VERIFY(poMultiColCtrl->SetColSelection(0));
    AddTable(poMultiColCtrl);
}

void DCP06ViewCdfDlgC::RefreshControls()
{
    if (poMultiColCtrl) {
        char temp[20];
        poMultiColCtrl->DestroyAll();
        for (short i = 0; i < m_pFileFunc->getPointsCount(); i++) {
            sprintf(temp, "%d", i + 1);
            m_pFileFunc->calcdist_pnt(i + 1);
            USER_APP_VERIFY(poMultiColCtrl->AddRow((short)i));
            USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_No, (short)i, StringC(temp)));
            USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_DistId, (short)i, StringC(m_pFileFunc->id)));
            USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Dist, (short)i, StringC(m_pFileFunc->dist)));
            USER_APP_VERIFY(poMultiColCtrl->SetCellText(CI_Note, (short)i, StringC(m_pFileFunc->note)));
        }
    }
}

void DCP06ViewCdfDlgC::OnDialogActivated() { GUI::TableDialogC::OnDialogActivated(); }
void DCP::DCP06ViewCdfDlgC::UpdateData() {}

bool DCP::DCP06ViewCdfDlgC::DeletePoint()
{
    short iSelectedId = poMultiColCtrl->GetSelectedId();
    if (m_pFileFunc->delete_point_from_cdf(iSelectedId)) { RefreshControls(); return true; }
    return false;
}

bool DCP::DCP06ViewCdfDlgC::GetSelectedData(StringC& sDistId, StringC& sRefId, StringC& sNote)
{
    short iSelectedId = poMultiColCtrl->GetSelectedId();
    if (iSelectedId == -1) return false;
    m_pFileFunc->calcdist_pnt(iSelectedId + 1);
    UTL::Trim(m_pFileFunc->id);  sDistId = StringC(m_pFileFunc->id);
    UTL::Trim(m_pFileFunc->ref); sRefId = StringC(m_pFileFunc->ref);
    UTL::Trim(m_pFileFunc->note); sNote = StringC(m_pFileFunc->note);
    return true;
}

bool DCP::DCP06ViewCdfDlgC::SetModel(GUI::ModelC* pModel)
{
    DCP::DCP06ModelC* pDCP06Model = dynamic_cast<DCP::DCP06ModelC*>(pModel);
    if (pDCP06Model != NULL && ModelHandlerC::SetModel(pDCP06Model)) { RefreshControls(); return true; }
    USER_APP_VERIFY(false);
    return false;
}

DCP::DCP06ModelC* DCP::DCP06ViewCdfDlgC::GetDCP06Model() const { return (DCP::DCP06ModelC*)GetModel(); }

// --- DCP06ViewCdfControllerC ---
DCP::DCP06ViewCdfControllerC::DCP06ViewCdfControllerC(DCP::CdfFileFunc* pFileFunc, DCP06ModelC* pDCP06Model)
    : m_pDlg(NULL), m_pFileFunc(pFileFunc), m_pDCP06Model(pDCP06Model)
{
    StringC sTitle;
    sTitle.LoadTxt(AT_DCP06, T_DCP_CALC_DIST_VIEW_TOK);
    sTitle += L" ("; sTitle += StringC(m_pFileFunc->getFileName()); sTitle += L")";
    SetTitle(sTitle);
    m_pDlg = new DCP::DCP06ViewCdfDlgC(m_pFileFunc, m_pDCP06Model);
    (void)AddDialog(VIEWCDF_DLG, m_pDlg, true);
    FKDef vDef; vDef.poOwner = this;
    vDef.strLable = StringC(AT_DCP06, K_DCP_EDIT_TOK); SetFunctionKey(FK1, vDef);
    vDef.strLable = StringC(AT_DCP06, K_DCP_DEL_TOK);  SetFunctionKey(SHFK5, vDef);
    vDef.strLable = StringC(AT_DCP06, K_DCP_CONT_TOK); SetFunctionKey(FK6, vDef);
    FKDef vDef1; vDef1.poOwner = this; vDef1.strLable = L" "; SetFunctionKey(SHFK6, vDef1);
}

DCP::DCP06ViewCdfControllerC::~DCP06ViewCdfControllerC() {}

bool DCP::DCP06ViewCdfControllerC::SetModel(GUI::ModelC* pModel)
{
    (void)ControllerC::SetModel(pModel);
    return m_pDlg->SetModel(pModel);
}

void DCP::DCP06ViewCdfControllerC::OnF1Pressed()
{
    if (m_pDlg == NULL) { USER_APP_VERIFY(false); return; }
    StringC sDistId, sRefId, sNote;
    if (m_pDlg->GetSelectedData(sDistId, sRefId, sNote) == false) return;
    DCP::DCP06EditCalcDistModelC* pModel = new DCP06EditCalcDistModelC;
    pModel->sDistId = sDistId; pModel->sRefId = sRefId; pModel->sNote = sNote;
    if (GetController(VIEWCDF_EDIT_CONTROLLER) == NULL)
        (void)AddController(VIEWCDF_EDIT_CONTROLLER, new DCP::DCP06EditCalcDistControllerC(m_pDlg->GetDCP06Model()));
    (void)GetController(VIEWCDF_EDIT_CONTROLLER)->SetModel(pModel);
    SetActiveController(VIEWCDF_EDIT_CONTROLLER, true);
}

void DCP::DCP06ViewCdfControllerC::OnSHF5Pressed() { if (m_pDlg) m_pDlg->DeletePoint(); }
void DCP::DCP06ViewCdfControllerC::OnF6Pressed() { if (m_pDlg) { m_pDlg->UpdateData(); (void)Close(EC_KEY_CONT); } }
void DCP::DCP06ViewCdfControllerC::OnActiveDialogClosed(int, int) {}

void DCP::DCP06ViewCdfControllerC::OnActiveControllerClosed(int lCtrlID, int lExitCode)
{
    if (lCtrlID == VIEWCDF_EDIT_CONTROLLER && lExitCode == EC_KEY_CONT) {
        DCP::DCP06EditCalcDistModelC* pModel = (DCP::DCP06EditCalcDistModelC*)GetController(VIEWCDF_EDIT_CONTROLLER)->GetModel();
        DCP06CommonC pCommon(m_pDCP06Model);
        char temp[100];
        BSS::UTI::BSS_UTI_WCharToAscii(pModel->sDistId, temp, 6); pCommon.strbtrim(temp); sprintf(m_pFileFunc->id, "%-s", temp);
        BSS::UTI::BSS_UTI_WCharToAscii(pModel->sRefId, temp, 6);  pCommon.strbtrim(temp); sprintf(m_pFileFunc->ref, "%-s", temp);
        BSS::UTI::BSS_UTI_WCharToAscii(pModel->sNote, temp, 6);   pCommon.strbtrim(temp); sprintf(m_pFileFunc->note, "%-s", temp);
        m_pFileFunc->cdf_save_pnt();
    }
    m_pDlg->RefreshControls();
    DestroyController(lCtrlID);
}
