// ================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: MidPoint controller with setup dialog (like Circle)
//
// ================================================================================================

#include "stdafx.h"
#include "calc.h"
#include <math.h>
#include <dcp06/core/Model.hpp>
#include <dcp06/core/Logger.hpp>
#include <dcp06/core/Measure.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/measurement/MidPointController.hpp>
#include <dcp06/measurement/Midpoint.hpp>
#include <dcp06/core/SelectMidpoint.hpp>
#include <dcp06/database/JsonDatabase.hpp>
#include <dcp06/database/DatabaseTypes.hpp>

#include <GUI_Types.hpp>
#include <GUI_DeskTop.hpp>
#include <GUI_MessageDialog.hpp>
#include <ABL_MsgDef.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace DCP;

// ================================================================================================
// Constructor
// ================================================================================================
DCP::MidPointController::MidPointController(DCP::Model *pModel)
    : m_pModel(pModel)
    , m_pCommon(0)
    , m_pDlg(nullptr)
    , m_pDataModel(0)
{
    DCP06_LOG_DEBUG("-- MidPointController::MidPointController: constructing");
    SetTitle(StringC(AT_DCP06, T_DCP_MID_POINT_TOK));

    m_pDataModel = new MidpointModel(pModel);
    m_pDlg = new MidpointDialog(pModel, m_pDataModel);
    (void)AddDialog(MIDPOINT_DLG, m_pDlg, true);
    DCP06_LOG_DEBUG("-- MidPointController::MidPointController: AddDialog(MIDPOINT_DLG) done");

    set_function_keys();
}

// ================================================================================================
// Destructor
// ================================================================================================
DCP::MidPointController::~MidPointController()
{
    if (m_pCommon)
    {
        delete m_pCommon;
        m_pCommon = 0;
    }
    if (m_pDataModel)
    {
        delete m_pDataModel;
        m_pDataModel = 0;
    }
}

// ================================================================================================
// set_function_keys: ADD, LIST, DEL, (empty), MEAS, CONT
// ================================================================================================
void DCP::MidPointController::set_function_keys()
{
    ResetFunctionKeys();
    FKDef vDef;
    vDef.poOwner = this;
    vDef.strLable = StringC(AT_DCP06, K_DCP_ADD_TOK);
    SetFunctionKey(FK1, vDef);
    vDef.strLable = StringC(AT_DCP06, K_DCP_LIST_TOK);
    SetFunctionKey(FK2, vDef);
    vDef.strLable = StringC(AT_DCP06, K_DCP_DEL_TOK);
    SetFunctionKey(FK3, vDef);
    vDef.strLable = L" ";
    SetFunctionKey(FK4, vDef);
    vDef.strLable = StringC(AT_DCP06, K_DCP_MEAS_TOK);
    SetFunctionKey(FK5, vDef);
    vDef.strLable = StringC(AT_DCP06, K_DCP_CONT_TOK);
    SetFunctionKey(FK6, vDef);

    FKDef vDef1;
    vDef1.poOwner = this;
    vDef1.strLable = L" ";
    SetFunctionKey(SHFK6, vDef1);

    GUI::DesktopC::Instance()->UpdateFunctionKeys();
}

// ================================================================================================
// getNextMidpointId: Mp1->Mp2, midpoint_2->midpoint_3
// ================================================================================================
std::string DCP::MidPointController::getNextMidpointId() const
{
    char buf[POINT_ID_BUFF_LEN];
    buf[0] = '\0';
    if (m_pDlg && m_pDlg->GetMidpointIdString(buf, sizeof(buf)))
        m_pDataModel->pCommon->strbtrim(buf);
    std::string current(buf);
    if (current.empty()) return "Mp1";

    size_t numStart = std::string::npos;
    for (size_t i = current.length(); i > 0; )
    {
        --i;
        if (current[i] >= '0' && current[i] <= '9')
            numStart = i;
        else
            break;
    }

    if (numStart != std::string::npos && numStart < current.length())
    {
        std::string prefix = current.substr(0, numStart);
        int num = 0;
        for (size_t i = numStart; i < current.length(); i++)
            num = num * 10 + (current[i] - '0');
        num++;
        char out[64];
        snprintf(out, sizeof(out), "%s%d", prefix.c_str(), num);
        return std::string(out);
    }
    return current + "_1";
}

// ================================================================================================
// ShowSelectMidpointDlg
// ================================================================================================
void DCP::MidPointController::ShowSelectMidpointDlg()
{
    DCP::Database::JsonDatabase* jdb = m_pModel->GetDatabase() ?
        dynamic_cast<DCP::Database::JsonDatabase*>(m_pModel->GetDatabase()) : 0;
    if (!jdb || !jdb->isJobLoaded()) return;

    DCP::SelectMidpointModel* pModel = new DCP::SelectMidpointModel;
    int iCount = jdb->getMidpointListAsSelectMidpoint(&pModel->midpoints[0], MAX_SELECT_MIDPOINTS);
    pModel->m_iCounts = static_cast<short>(iCount);

    char buf[POINT_ID_BUFF_LEN];
    buf[0] = '\0';
    if (m_pDlg && m_pDlg->GetMidpointIdString(buf, sizeof(buf)))
        m_pDataModel->pCommon->strbtrim(buf);
    std::string currentId(buf);
    int selIdx = 1;
    for (int i = 0; i < iCount; i++)
    {
        if (strcmp(pModel->midpoints[i].midpoint_id, currentId.c_str()) == 0)
        {
            selIdx = i + 1;
            break;
        }
    }
    pModel->m_iSelectedId = static_cast<short>(selIdx);

    if (GetController(SELECT_MIDPOINT_CONTROLLER) == nullptr)
        (void)AddController(SELECT_MIDPOINT_CONTROLLER, new DCP::SelectMidpointController);
    (void)GetController(SELECT_MIDPOINT_CONTROLLER)->SetModel(pModel);
    SetActiveController(SELECT_MIDPOINT_CONTROLLER, true);
}

// ================================================================================================
// RunMeas: open measure controller (original Run logic)
// ================================================================================================
void DCP::MidPointController::RunMeas()
{
    if (!m_pDataModel->pCommon->check_edm_mode())
        return;

    DCP::MeasureModel* pModel = new MeasureModel;
    pModel->m_iPointsCount = MAX_MID_POINTS;
    pModel->m_iMaxPoint = MAX_MID_POINTS;
    pModel->m_iMinPoint = MAX_MID_POINTS;

    memset(&pModel->point_table[0], 0, sizeof(S_POINT_BUFF) * MAX_MID_POINTS);
    memcpy(&pModel->point_table[0], &m_pModel->mid_points[0], sizeof(S_POINT_BUFF) * MAX_MID_POINTS);

    if (GetController(MEAS_MID_POINT_CONTROLLER) == nullptr)
        (void)AddController(MEAS_MID_POINT_CONTROLLER, new DCP::MeasureController(m_pModel));

    (void)GetController(MEAS_MID_POINT_CONTROLLER)->SetTitle(StringC(AT_DCP06, T_DCP_MID_POINT_TOK));
    (void)GetController(MEAS_MID_POINT_CONTROLLER)->SetModel(pModel);
    SetActiveController(MEAS_MID_POINT_CONTROLLER, true);
}

// ================================================================================================
// Run: delegate to base - let ControllerC show the default dialog (like CircleController does)
// ================================================================================================
void DCP::MidPointController::Run(void)
{
    ControllerC::Run();
}

// ================================================================================================
// OnControllerActivated
// ================================================================================================
void DCP::MidPointController::OnControllerActivated(void)
{
    DCP06_LOG_DEBUG("-- MidPointController::OnControllerActivated: called");
}

// ================================================================================================
// SetModel
// ================================================================================================
bool DCP::MidPointController::SetModel(GUI::ModelC* pModel)
{
    DCP06_LOG_DEBUG("-- MidPointController::SetModel: called");
    m_pCommon = new Common(m_pModel);
    m_pPointBuffModel = (PointBuffModel*)pModel;
    (void)ControllerC::SetModel(pModel);
    bool ret = m_pDlg ? m_pDlg->SetModel(pModel) : true;
    // Force framework to show this controller's dialog (Circle works without this; Midpoint needs it)
    SetActiveDialog(MIDPOINT_DLG, true);
    DCP06_LOG_DEBUG("-- MidPointController::SetModel: done");
    return ret;
}

// ================================================================================================
// OnF1Pressed ADD
// ================================================================================================
void DCP::MidPointController::OnF1Pressed()
{
    if (!m_pDlg || !m_pDataModel) return;

    m_pDataModel->clear_midpoint();
    std::string nextId = getNextMidpointId();
    snprintf(m_pDataModel->midpoint_id, POINT_ID_BUFF_LEN, "%-s", nextId.c_str());

    if (m_pModel)
    {
        DCP::Database::JsonDatabase* jdb = m_pModel->GetDatabase() ?
            dynamic_cast<DCP::Database::JsonDatabase*>(m_pModel->GetDatabase()) : 0;
        if (jdb && jdb->isJobLoaded() && !m_pModel->m_currentJobId.empty())
        {
            DCP::Database::MidpointData existing;
            if (!jdb->getMidpoint(nextId, existing))
            {
                DCP::Database::MidpointData md;
                md.type = "midpoint";
                md.id = nextId;
                md.calculated = false;
                jdb->addMidpoint(nextId, md);
                jdb->saveJob(m_pModel->m_currentJobId);
            }
        }
    }
    m_pDlg->RefreshControls();
}

// ================================================================================================
// OnF2Pressed LIST
// ================================================================================================
void DCP::MidPointController::OnF2Pressed()
{
    if (!m_pDlg) return;
    ShowSelectMidpointDlg();
}

// ================================================================================================
// OnF3Pressed DEL
// ================================================================================================
void DCP::MidPointController::OnF3Pressed()
{
    OnSHF2Pressed();
}

// ================================================================================================
// OnF4Pressed (empty)
// ================================================================================================
void DCP::MidPointController::OnF4Pressed()
{
}

// ================================================================================================
// OnF5Pressed MEAS
// ================================================================================================
void DCP::MidPointController::OnF5Pressed()
{
    if (!m_pDlg || !m_pDataModel) return;
    RunMeas();
}

// ================================================================================================
// OnF6Pressed CONT
// ================================================================================================
void DCP::MidPointController::OnF6Pressed()
{
    if (!m_pDlg || !m_pDataModel) return;

    m_pDlg->UpdateData();

    char buf[POINT_ID_BUFF_LEN];
    buf[0] = '\0';
    if (m_pDlg->GetMidpointIdString(buf, sizeof(buf)))
        m_pDataModel->pCommon->strbtrim(buf);
    std::string midpointId(buf[0] ? buf : "Mp1");

    if (GetDataModel() && midpointId.size() > 0)
    {
        DCP::Database::JsonDatabase* jdb = m_pModel->GetDatabase() ?
            dynamic_cast<DCP::Database::JsonDatabase*>(m_pModel->GetDatabase()) : 0;
        if (jdb && jdb->isJobLoaded() && !m_pModel->m_currentJobId.empty())
        {
            DCP::Database::MidpointData md;
            md.type = "midpoint";
            md.id = midpointId;
            md.x = GetDataModel()->m_pPointBuff[0].x;
            md.y = GetDataModel()->m_pPointBuff[0].y;
            md.z = GetDataModel()->m_pPointBuff[0].z;
            md.calculated = (GetDataModel()->m_pPointBuff[0].sta == POINT_MEASURED);
            DCP::Database::MidpointData existing;
            if (jdb->getMidpoint(midpointId, existing))
                jdb->updateMidpoint(midpointId, md);
            else
                jdb->addMidpoint(midpointId, md);
            jdb->saveJob(m_pModel->m_currentJobId);
        }
    }

    if (GetDataModel() && GetDataModel()->m_pPointBuff[0].sta == POINT_MEASURED)
        (void)Close(EC_KEY_CONT);
    else
        (void)Close(EC_KEY_ESC);
}

// ================================================================================================
// OnSHF2Pressed DEL - delete from DB and clear
// ================================================================================================
void DCP::MidPointController::OnSHF2Pressed()
{
    if (!m_pDlg || !m_pDataModel) return;

    char buf[POINT_ID_BUFF_LEN];
    buf[0] = '\0';
    if (m_pDlg->GetMidpointIdString(buf, sizeof(buf)))
    {
        m_pDataModel->pCommon->strbtrim(buf);
        std::string midpointId(buf);
        if (!midpointId.empty())
        {
            StringC msg, strMidpointText;
            strMidpointText.LoadTxt(AT_DCP06, L_DCP_MID_POINT_TOK);
            msg.LoadTxt(AT_DCP06, M_DCP_DELETE_ALL_TOK);
            msg.Format(msg, (const wchar_t*)strMidpointText);
            if (m_pDataModel->pMsgBox->ShowMessageYesNo(msg))
            {
                if (m_pModel)
                {
                    DCP::Database::JsonDatabase* jdb = m_pModel->GetDatabase() ?
                        dynamic_cast<DCP::Database::JsonDatabase*>(m_pModel->GetDatabase()) : 0;
                    if (jdb && jdb->isJobLoaded() && !m_pModel->m_currentJobId.empty())
                    {
                        jdb->deleteMidpoint(midpointId);
                        jdb->saveJob(m_pModel->m_currentJobId);
                    }
                }
                m_pDataModel->clear_midpoint();
            }
        }
        else
            m_pDataModel->delete_midpoint();
    }
    else
        m_pDataModel->delete_midpoint();

    m_pDlg->RefreshControls();
}

// ================================================================================================
// OnActiveDialogClosed
// ================================================================================================
void DCP::MidPointController::OnActiveDialogClosed(int /*lDlgID*/, int /*lExitCode*/)
{
}

// ================================================================================================
// OnActiveControllerClosed
// ================================================================================================
void DCP::MidPointController::OnActiveControllerClosed(int lCtrlID, int lExitCode)
{
    if (lCtrlID == MEAS_MID_POINT_CONTROLLER && lExitCode == EC_KEY_CONT)
    {
        DCP::MeasureModel* pModel = (DCP::MeasureModel*)GetController(MEAS_MID_POINT_CONTROLLER)->GetModel();

        double x_mid = 0.0, y_mid = 0.0, z_mid = 0.0;

        memcpy(&m_pModel->mid_points[0], &pModel->point_table[0], sizeof(S_POINT_BUFF) * MAX_MID_POINTS);

        if (m_pCommon && m_pCommon->calc_mid_point(&pModel->point_table[0], &x_mid, &y_mid, &z_mid) == 0)
        {
            GetDataModel()->m_pPointBuff[0].x = x_mid;
            GetDataModel()->m_pPointBuff[0].y = y_mid;
            GetDataModel()->m_pPointBuff[0].z = z_mid;
            GetDataModel()->m_pPointBuff[0].sta = POINT_MEASURED;

            m_pDlg->UpdateData();
            char buf[POINT_ID_BUFF_LEN];
            buf[0] = '\0';
            if (m_pDlg->GetMidpointIdString(buf, sizeof(buf)))
                m_pDataModel->pCommon->strbtrim(buf);
            std::string midpointId(buf[0] ? buf : "Mp1");
            if (m_pModel && !midpointId.empty())
            {
                DCP::Database::JsonDatabase* jdb = m_pModel->GetDatabase() ?
                    dynamic_cast<DCP::Database::JsonDatabase*>(m_pModel->GetDatabase()) : 0;
                if (jdb && jdb->isJobLoaded() && !m_pModel->m_currentJobId.empty())
                {
                    DCP::Database::MidpointData md;
                    md.type = "midpoint";
                    md.id = midpointId;
                    md.x = x_mid;
                    md.y = y_mid;
                    md.z = z_mid;
                    md.calculated = true;
                    DCP::Database::MidpointData existing;
                    if (jdb->getMidpoint(midpointId, existing))
                        jdb->updateMidpoint(midpointId, md);
                    else
                        jdb->addMidpoint(midpointId, md);
                    jdb->saveJob(m_pModel->m_currentJobId);
                }
            }
            Close(EC_KEY_CONT);
        }
        else
        {
            GUI::DesktopC::Instance()->MessageShow(L"Cannot calculate mid point!");
            Close(EC_KEY_ESC);
        }
    }

    if (lCtrlID == SELECT_MIDPOINT_CONTROLLER && lExitCode == EC_KEY_CONT)
    {
        DCP::SelectMidpointModel* pModel = (DCP::SelectMidpointModel*)GetController(SELECT_MIDPOINT_CONTROLLER)->GetModel();
        if (pModel && !pModel->m_strSelectedMidpointId.IsEmpty())
        {
            char buf[POINT_ID_BUFF_LEN];
            m_pDataModel->pCommon->convert_to_ascii(pModel->m_strSelectedMidpointId, buf, (short)sizeof(buf));
            m_pDataModel->pCommon->strbtrim(buf);
            std::string midpointId(buf);
            if (!midpointId.empty() && m_pDlg->LoadMidpointFromDb(midpointId))
                snprintf(m_pDataModel->midpoint_id, POINT_ID_BUFF_LEN, "%-s", midpointId.c_str());
        }
        if (m_pDlg)
            m_pDlg->RefreshControls();
    }

    DestroyController(lCtrlID);
}

// ================================================================================================
// GetDataModel
// ================================================================================================
DCP::PointBuffModel* DCP::MidPointController::GetDataModel() const
{
    return (PointBuffModel*)GetModel();
}
