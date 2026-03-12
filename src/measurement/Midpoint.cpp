// ================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: Midpoint setup dialog
//
// ================================================================================================

#include "stdafx.h"
#include <dcp06/core/Model.hpp>
#include <dcp06/core/Logger.hpp>
#include <dcp06/measurement/Midpoint.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/database/JsonDatabase.hpp>
#include <dcp06/database/DatabaseTypes.hpp>

#include <GUI_Types.hpp>
#include <UTL_String.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

OBS_IMPLEMENT_EXECUTE(DCP::MidpointDialog);

using namespace DCP;

// ================================================================================================
// MidpointDialog
// ================================================================================================

DCP::MidpointDialog::MidpointDialog(DCP::Model * pModel, MidpointModel* pMidpointModel)
    : GUI::ModelHandlerC()
    , GUI::StandardDialogC()
    , m_pModel(pModel)
    , m_pMidpointId(0)
    , m_pDataModel(pMidpointModel)
    , m_pMidpointIdEditObserver(OBS_METHOD_TO_PARAM0(MidpointDialog, OnValueChanged), this)
{
}

DCP::MidpointDialog::~MidpointDialog()
{
}

void DCP::MidpointDialog::OnInitDialog(void)
{
    DCP06_LOG_DEBUG(">> MidpointDialog::OnInitDialog");
    GUI::BaseDialogC::OnInitDialog();

    m_pMidpointId = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
    m_pMidpointId->SetId(eMidpointId);
    m_pMidpointId->SetText(StringC(AT_DCP06, L_DCP_ENTER_MIDPOINT_ID_TOK));
    m_pMidpointId->GetStringInputCtrl()->SetCharsCountMax(DCP_POINT_ID_LENGTH);
    m_pMidpointId->SetEmptyAllowed(true);
    AddCtrl(m_pMidpointId);

    m_pMidpointIdEditObserver.Attach(m_pMidpointId->GetSubject());

    DCP06_LOG_DEBUG("<< MidpointDialog::OnInitDialog");
}

void DCP::MidpointDialog::OnDialogActivated()
{
    DCP06_LOG_DEBUG("-- MidpointDialog::OnDialogActivated: called");
    RefreshControls();
}

void DCP::MidpointDialog::UpdateData()
{
    if (m_pDataModel && m_pMidpointId && m_pMidpointId->GetStringInputCtrl())
    {
        StringC s = m_pMidpointId->GetStringInputCtrl()->GetString();
        if (m_pDataModel->pCommon)
        {
            char buf[POINT_ID_BUFF_LEN];
            m_pDataModel->pCommon->convert_to_ascii(s, buf, (short)sizeof(buf));
            m_pDataModel->pCommon->strbtrim(buf);
            snprintf(m_pDataModel->midpoint_id, POINT_ID_BUFF_LEN, "%-s", buf[0] ? buf : "Mp1");
        }
    }
}

void DCP::MidpointDialog::RefreshControls()
{
    if (m_pMidpointId && m_pDataModel)
    {
        char buf[POINT_ID_BUFF_LEN];
        snprintf(buf, sizeof(buf), "%-s", m_pDataModel->midpoint_id[0] ? m_pDataModel->midpoint_id : "Mp1");
        if (m_pDataModel->pCommon) m_pDataModel->pCommon->strbtrim(buf);
        m_pMidpointId->GetStringInputCtrl()->SetString(StringC(buf[0] ? buf : "Mp1"));
    }
}

bool DCP::MidpointDialog::LoadMidpointFromDb(const std::string& midpointId)
{
    if (!m_pModel || midpointId.empty()) return false;
    DCP::Database::JsonDatabase* jdb = m_pModel->GetDatabase() ?
        dynamic_cast<DCP::Database::JsonDatabase*>(m_pModel->GetDatabase()) : 0;
    if (!jdb) return false;

    DCP::Database::MidpointData md;
    if (!jdb->getMidpoint(midpointId, md)) return false;

    snprintf(m_pDataModel->midpoint_id, POINT_ID_BUFF_LEN, "%-s", midpointId.c_str());

    PointBuffModel* pPointBuff = GetPointBuffModelModel();
    if (pPointBuff && md.calculated)
    {
        pPointBuff->m_pPointBuff[0].x = md.x;
        pPointBuff->m_pPointBuff[0].y = md.y;
        pPointBuff->m_pPointBuff[0].z = md.z;
        pPointBuff->m_pPointBuff[0].sta = POINT_MEASURED;
    }

    RefreshControls();
    return true;
}

bool DCP::MidpointDialog::SetModel(GUI::ModelC* pModel)
{
    DCP::PointBuffModel* pDcpModel = dynamic_cast<DCP::PointBuffModel*>(pModel);
    if (pDcpModel != nullptr && ModelHandlerC::SetModel(pDcpModel))
    {
        RefreshControls();
        return true;
    }
    USER_APP_VERIFY(false);
    return false;
}

DCP::PointBuffModel* DCP::MidpointDialog::GetPointBuffModelModel() const
{
    return (PointBuffModel*)GetModel();
}

bool DCP::MidpointDialog::GetMidpointIdString(char* buf, size_t bufLen) const
{
    if (!m_pMidpointId || !m_pDataModel || !m_pDataModel->pCommon || bufLen == 0) return false;
    StringC s = m_pMidpointId->GetStringInputCtrl()->GetString();
    if (s.IsEmpty()) s = StringC(L"Mp1");
    m_pDataModel->pCommon->convert_to_ascii(s, buf, (short)bufLen);
    m_pDataModel->pCommon->strbtrim(buf);
    return true;
}

void DCP::MidpointDialog::OnValueChanged(int unNotifyCode, int ulParam2)
{
    if (unNotifyCode == GUI::NC_ONEDITMODE_LEFT && ulParam2 == eMidpointId && m_pMidpointId && m_pMidpointId->GetStringInputCtrl())
    {
        StringC s = m_pMidpointId->GetStringInputCtrl()->GetString();
        if (m_pDataModel && m_pDataModel->pCommon)
        {
            char buf[POINT_ID_BUFF_LEN];
            m_pDataModel->pCommon->convert_to_ascii(s, buf, (short)sizeof(buf));
            m_pDataModel->pCommon->strbtrim(buf);
            snprintf(m_pDataModel->midpoint_id, POINT_ID_BUFF_LEN, "%-s", buf[0] ? buf : "Mp1");
            std::string midpointId(buf[0] ? buf : "Mp1");
            if (m_pModel && !midpointId.empty())
            {
                DCP::Database::JsonDatabase* jdb = m_pModel->GetDatabase() ?
                    dynamic_cast<DCP::Database::JsonDatabase*>(m_pModel->GetDatabase()) : 0;
                if (jdb && jdb->isJobLoaded() && !m_pModel->m_currentJobId.empty())
                {
                    DCP::Database::MidpointData existing;
                    if (!jdb->getMidpoint(midpointId, existing))
                    {
                        DCP::Database::MidpointData md;
                        md.type = "midpoint";
                        md.id = midpointId;
                        md.calculated = false;
                        jdb->addMidpoint(midpointId, md);
                        jdb->saveJob(m_pModel->m_currentJobId);
                    }
                }
            }
        }
    }
}

// ================================================================================================
// MidpointModel
// ================================================================================================

DCP::MidpointModel::MidpointModel(Model* pModel) : m_pModel(pModel)
{
    memset(midpoint_id, 0, sizeof(midpoint_id));
    snprintf(midpoint_id, POINT_ID_BUFF_LEN, "Mp1");
    pCommon = new Common(pModel);
    pMsgBox = new MsgBox;
}

DCP::MidpointModel::~MidpointModel()
{
    if (pCommon) { delete pCommon; pCommon = 0; }
    if (pMsgBox) { delete pMsgBox; pMsgBox = 0; }
}

void DCP::MidpointModel::clear_midpoint()
{
    memset(midpoint_id, 0, sizeof(midpoint_id));
    snprintf(midpoint_id, POINT_ID_BUFF_LEN, "Mp1");
}

void DCP::MidpointModel::delete_midpoint()
{
    StringC msg, strMidpointText;
    strMidpointText.LoadTxt(AT_DCP06, L_DCP_MID_POINT_TOK);
    msg.LoadTxt(AT_DCP06, M_DCP_DELETE_ALL_TOK);
    msg.Format(msg, (const wchar_t*)strMidpointText);
    if (pMsgBox->ShowMessageYesNo(msg))
        clear_midpoint();
}
