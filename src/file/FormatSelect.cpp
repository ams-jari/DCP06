// ================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: Format selection for Import/Export
//
// ================================================================================================

#include "stdafx.h"
#include <dcp06/file/FormatSelect.hpp>
#include <dcp06/core/Defs.hpp>

using namespace DCP;

// ------------------------------------------------------------------------------------------------
// FormatSelectModel
// ------------------------------------------------------------------------------------------------

FormatSelectModel::FormatSelectModel() : m_iFormat(ONLY_ADF), m_bForImport(true) {}
FormatSelectModel::~FormatSelectModel() {}

// ------------------------------------------------------------------------------------------------
// FormatSelectDialog
// ------------------------------------------------------------------------------------------------

FormatSelectDialog::FormatSelectDialog(const wchar_t* sTitle, bool bImport)
    : m_pInfo(0), m_bImport(bImport) {
    SetTitle(StringC(sTitle));
}

FormatSelectDialog::~FormatSelectDialog() {}

void FormatSelectDialog::OnInitDialog() {
    GUI::BaseDialogC::OnInitDialog();
    m_pInfo = new GUI::TextCtrlC();
    m_pInfo->SetId(1);
    m_pInfo->SetText(StringC(L"F1: ADF\nF2: CSV\nF3: TXT\nF4: ASC"));
    AddCtrl(m_pInfo);
}

void FormatSelectDialog::OnDialogActivated() {
    RefreshControls();
}

bool FormatSelectDialog::SetModel(GUI::ModelC* pModel) {
    return GUI::ModelHandlerC::SetModel(pModel);
}

FormatSelectModel* FormatSelectDialog::GetDataModel() const {
    return (FormatSelectModel*)GetModel();
}

// ------------------------------------------------------------------------------------------------
// FormatSelectController
// ------------------------------------------------------------------------------------------------

FormatSelectController::FormatSelectController(const wchar_t* sTitle, bool bImport)
    : m_pDlg(0), m_bImport(bImport) {
    m_pDlg = new FormatSelectDialog(sTitle, bImport);
    (void)AddDialog(FORMAT_SELECT_DLG, m_pDlg, true);

    FKDef vDef;
    vDef.poOwner = this;
    vDef.strLable = L"ADF";
    SetFunctionKey(FK1, vDef);

    vDef.strLable = L"CSV";
    SetFunctionKey(FK2, vDef);

    vDef.strLable = L"TXT";
    SetFunctionKey(FK3, vDef);

    vDef.strLable = L"ASC";
    SetFunctionKey(FK4, vDef);

    FKDef vDef1;
    vDef1.poOwner = this;
    vDef1.strLable = L" ";
    SetFunctionKey(SHFK6, vDef1);
}

FormatSelectController::~FormatSelectController() {}

bool FormatSelectController::SetModel(GUI::ModelC* pModel) {
    (void)ControllerC::SetModel(pModel);
    return m_pDlg ? m_pDlg->SetModel(pModel) : false;
}

short FormatSelectController::GetFormat() const {
    FormatSelectModel* p = (FormatSelectModel*)GetModel();
    return p ? p->m_iFormat : ONLY_ADF;
}

static void doFormatChoice(FormatSelectController* self, short format) {
    FormatSelectModel* p = (FormatSelectModel*)self->GetModel();
    if (p) p->m_iFormat = format;
    (void)self->Close(EC_KEY_CONT);
}

void FormatSelectController::OnF1Pressed() { doFormatChoice(this, ONLY_ADF); }
void FormatSelectController::OnF2Pressed() { doFormatChoice(this, ONLY_CSV); }
void FormatSelectController::OnF3Pressed() { doFormatChoice(this, ONLY_TXT); }
void FormatSelectController::OnF4Pressed() { doFormatChoice(this, ONLY_ASC); }
