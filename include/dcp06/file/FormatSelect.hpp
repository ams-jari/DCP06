// ====================================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: Format selection for Import/Export (ADF, CSV, TXT, ASC)
//
// ------------------------------------------------------------------------------------------------
//
// Copyright (c) AMS. Based on Leica Captivate plugin framework.
//
// ================================================================================================

#ifndef DCP_FORMATSELECT_HPP
#define DCP_FORMATSELECT_HPP

#include <dcp06/core/Types.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>

namespace DCP {

class Model;

class FormatSelectController;
class FormatSelectDialog;

class FormatSelectModel : public GUI::ModelC {
public:
    FormatSelectModel();
    virtual ~FormatSelectModel();
    short m_iFormat;   // ONLY_ADF, ONLY_CSV, ONLY_TXT, ONLY_ASC
    bool m_bForImport; // true=Import, false=Export
};

class FormatSelectController : public GUI::ControllerC {
public:
    FormatSelectController(const wchar_t* sTitle, bool bImport);
    ~FormatSelectController();
    virtual void OnF1Pressed();
    virtual void OnF2Pressed();
    virtual void OnF3Pressed();
    virtual void OnF4Pressed();
    virtual bool SetModel(GUI::ModelC* pModel);
    short GetFormat() const;
private:
    FormatSelectController(const FormatSelectController&);
    FormatSelectController& operator=(const FormatSelectController&);
    FormatSelectDialog* m_pDlg;
    bool m_bImport;
};

class FormatSelectDialog : public GUI::StandardDialogC, public GUI::ModelHandlerC {
public:
    FormatSelectDialog(const wchar_t* sTitle, bool bImport);
    virtual ~FormatSelectDialog();
    virtual void OnInitDialog();
    virtual void OnDialogActivated();
    virtual void RefreshControls();
    virtual bool SetModel(GUI::ModelC* pModel);
    FormatSelectModel* GetDataModel() const;
protected:
    GUI::TextCtrlC* m_pInfo;
private:
    bool m_bImport;
};

}  // namespace DCP

#endif
