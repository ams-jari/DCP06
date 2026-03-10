// ================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: 
//
// $Workfile: HEW_HelloWorldGUI.cpp $
//
// Summary  : 
//
// ------------------------------------------------------------------------------------------------
//
// Copyright (c) AMS. Based on Leica Captivate plugin framework.
//
// ================================================================================================


// $Author: Hlar $
// $Date: 6.07.04 8:55 $
// $Revision: 1 $
// $Modtime: 5.07.04 14:55 $

/* $ History: $
*/
// $NoKeywords: $

#include "stdafx.h"
#include <dcp06/core/Model.hpp>
#include <dcp06/calculation/CalculationDist2Points.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/calculation/CalculationLine.hpp>
#include <dcp06/core/SelectMultiPoints.hpp>

#include <GUI_DeskTop.hpp>
#include <GUI_Layout.hpp>

// Detect memory leaks
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ================================================================================================
// ========================================  Declarations  ========================================
// ================================================================================================
// OBS_IMPLEMENT_EXECUTE(DCP::UnitDialog);

//================================================================================================
// =====================================  Static Functions  =======================================
// ================================================================================================


// ================================================================================================
// ======================================  Member Functions  ======================================
// ================================================================================================

// ================================================================================================
// Description: Constructor
// ================================================================================================
DCP::CalculationDist2PointsDialog::CalculationDist2PointsDialog(DCP::Model* pModel):m_pPoints(0),
		m_pHeaderDevAct(0), m_pHeaderDevDes(0), m_pHeaderDevDev(0), m_pXText(0), m_pYText(0),
		m_pZText(0), m_pTotalText(0), m_pXAct(0),m_pYAct(0),m_pZAct(0),m_pTotalAct(0),
		m_pXDsg(0),m_pYDsg(0),m_pZDsg(0),m_pTotalDsg(0),m_pXDev(0),m_pYDev(0),m_pZDev(0),m_pTotalDev(0),
		m_pModel(pModel),m_pCommon(0)
{
	//SetTxtApplicationId(AT_DCP06);
	
	m_pCommon = new Common(m_pModel);

	m_strDevActual.LoadTxt(AT_DCP06,P_DCP_DEV_ACTUAL_TOK);
	m_strDevDesign.LoadTxt(AT_DCP06,P_DCP_DEV_DESIGN_TOK);
	m_strDevDev.LoadTxt(AT_DCP06,P_DCP_DEV_DEVIATION_TOK);

	m_strX.LoadTxt(AT_DCP06, P_DCP_X_TOK);
	//m_strX += L":";

	m_strY.LoadTxt(AT_DCP06, P_DCP_Y_TOK);
	//m_strY += L":";

	m_strZ.LoadTxt(AT_DCP06, P_DCP_Z_TOK);
	//m_strZ += L":";

	m_strTotal.LoadTxt(AT_DCP06, P_DCP_TOTAL_DIST_TOK);
	//m_strTotal += L":";

	 bXmea[0] = '\0'; bYmea[0] = '\0'; bZmea[0] = '\0'; 
     bXdes[0] = '\0'; bYdes[0] = '\0'; bZdes[0] = '\0'; 
	 bXdif[0] = '\0'; bYdif[0] = '\0'; bZdif[0] = '\0';
	 bDmea[0] = '\0'; bDdes[0] = '\0'; bDdif[0] = '\0';

	sXline = L" ";
	sYline = L" ";
	sZline = L" ";
	sPoints = L" ";
	sTline = L" ";
}


// ================================================================================================
// Description: Destructor
// ================================================================================================
DCP::CalculationDist2PointsDialog::~CalculationDist2PointsDialog()
{
	if(m_pCommon)
	{
		delete m_pCommon;
		m_pCommon = 0;
	}
}

// ================================================================================================
// Description: OnInitDialog
// ================================================================================================
void DCP::CalculationDist2PointsDialog::OnInitDialog(void)
{
	GUI::BaseDialogC::OnInitDialog();
    
	m_pPoints = new GUI::ComboLineCtrlC(GUI::ComboLineCtrlC::IC_String);
	m_pPoints->SetId(ePoints);
	m_pPoints->SetText(StringC(AT_DCP06,P_DCP_POINTS_TOK));
	
	//m_pPoints->GetStringInputCtrl()->SetAlign(AlignmentT::AL_LEFT); CAPTIVATE
	
	void(m_pPoints->SetCtrlState(GUI::BaseCtrlC::CS_ReadOnly));
	void(m_pPoints->SetCtrlState(GUI::BaseCtrlC::CS_FocusUnable));
	//AddCtrl(m_pPoints);
	
	//InsertEmptyLine(); CAPTIVATE

	StringC sTemp;

	m_pHeaderDevAct = new GUI::TextCtrlC();
	m_pHeaderDevAct->SetText(m_strDevActual);

	m_pHeaderDevDes = new GUI::TextCtrlC();
	m_pHeaderDevDes->SetText(m_strDevDesign);

	m_pHeaderDevDev = new GUI::TextCtrlC();
	m_pHeaderDevDev->SetText(m_strDevDev);

	m_pXText = new GUI::TextCtrlC();
	m_pXText->SetText(StringC(m_strX));

	m_pYText = new GUI::TextCtrlC();
	m_pYText->SetText(StringC(m_strY));

	m_pZText = new GUI::TextCtrlC();
	m_pZText->SetText(StringC(m_strZ));

	m_pTotalText = new GUI::TextCtrlC();
	m_pTotalText->SetText(StringC(m_strTotal));

	m_pXAct = new GUI::TextCtrlC();
	m_pYAct = new GUI::TextCtrlC();
	m_pZAct = new GUI::TextCtrlC();
	m_pTotalAct = new GUI::TextCtrlC();

	m_pXDsg = new GUI::TextCtrlC();
	m_pYDsg = new GUI::TextCtrlC();
	m_pZDsg = new GUI::TextCtrlC();
	m_pTotalDsg = new GUI::TextCtrlC();

	m_pXDev = new GUI::TextCtrlC();
	m_pYDev = new GUI::TextCtrlC();
	m_pZDev = new GUI::TextCtrlC();
	m_pTotalDev = new GUI::TextCtrlC();

	GUI::LayoutC* pLayout = new GUI::LayoutC(GUI::LayoutC::LT_HorizontalBox, this);
	//GUI::LayoutC* pRow1 = new GUI::LayoutC(GUI::LayoutC::LT_HorizontalBox, this);
	GUI::LayoutC* pCol1 = new GUI::LayoutC(GUI::LayoutC::LT_VerticalBox, this);
    GUI::LayoutC* pCol2 = new GUI::LayoutC(GUI::LayoutC::LT_VerticalBox, this);
	GUI::LayoutC* pCol3 = new GUI::LayoutC(GUI::LayoutC::LT_VerticalBox, this);
	GUI::LayoutC* pCol4 = new GUI::LayoutC(GUI::LayoutC::LT_VerticalBox, this);

	//pLayout->AddLayout(pRow1);
	pLayout->AddLayout(pCol1);
	pLayout->AddLayout(pCol2);
	pLayout->AddLayout(pCol3);
	pLayout->AddLayout(pCol4);
	SetLayout(pLayout);

	pCol1->AddCtrl(m_pPoints);
	pCol1->AddCtrl(new GUI::TextCtrlC()); // empty
	pCol1->AddCtrl(m_pXText);
	pCol1->AddCtrl(m_pYText);
	pCol1->AddCtrl(m_pZText);
	pCol1->AddCtrl(m_pTotalText);

	pCol2->AddCtrl(new GUI::TextCtrlC()); // empty
	pCol2->AddCtrl(m_pHeaderDevAct);
	pCol2->AddCtrl(m_pXAct);
	pCol2->AddCtrl(m_pYAct);
	pCol2->AddCtrl(m_pZAct);
	pCol2->AddCtrl(m_pTotalAct);

	pCol3->AddCtrl(new GUI::TextCtrlC()); // empty
	pCol3->AddCtrl(m_pHeaderDevDes);
	pCol3->AddCtrl(m_pXDsg);
	pCol3->AddCtrl(m_pYDsg);
	pCol3->AddCtrl(m_pZDsg);
	pCol3->AddCtrl(m_pTotalDsg);

	pCol4->AddCtrl(new GUI::TextCtrlC()); // empty
	pCol4->AddCtrl(m_pHeaderDevDev);
	pCol4->AddCtrl(m_pXDev);
	pCol4->AddCtrl(m_pYDev);
	pCol4->AddCtrl(m_pZDev);
	pCol4->AddCtrl(m_pTotalDev);

	//SetHelpTok(H_DCP_CALC_2_POINTS_TOK,0);
}

// ================================================================================================
// Description: OnDialogActivated
// ================================================================================================
void DCP::CalculationDist2PointsDialog::OnDialogActivated()
{
	// Set title for dialog
	SetTitle(GetDataModel()->sTitle);

	// referesh all controls
	RefreshControls();
}

// ================================================================================================
// Description: refresh all controls
// ================================================================================================

void DCP::CalculationDist2PointsDialog::RefreshControls()
{	
	if(m_pPoints && m_pHeaderDevAct && m_pHeaderDevDes && m_pHeaderDevDev &&
		m_pXText && m_pYText && m_pZText && m_pTotalText && m_pXAct && m_pYAct && m_pZAct && m_pTotalAct &&
		 m_pXDsg && m_pYDsg && m_pZDsg && m_pTotalDsg &&  m_pXDev && m_pYDev && m_pZDev && m_pTotalDev) 
	{
		m_pPoints->GetStringInputCtrl()->SetString(sPoints);

		m_pXAct->SetText(bXmea);
		m_pXDsg->SetText(bXdes);
		m_pXDev->SetText(bXdif);
		
		m_pYAct->SetText(bYmea);
		m_pYDsg->SetText(bYdes);
		m_pYDev->SetText(bYdif);

		m_pZAct->SetText(bZmea);
		m_pZDsg->SetText(bZdes);
		m_pZDev->SetText(bZdif);

		m_pTotalAct->SetText(bDmea);
		m_pTotalDsg->SetText(bDdes);
		m_pTotalDev->SetText(bDdif);
		/*
		m_pX->SetString(1,StringC(bXmea));
		m_pX->SetString(2,StringC(bXdes));
		m_pX->SetString(3,StringC(bXdif));

		m_pY->SetString(1,StringC(bYmea));
		m_pY->SetString(2,StringC(bYdes));
		m_pY->SetString(3,StringC(bYdif));

		m_pZ->SetString(1,StringC(bZmea));
		m_pZ->SetString(2,StringC(bZdes));
		m_pZ->SetString(3,StringC(bZdif));

		m_pTotal->SetString(1,StringC(bDmea));
		m_pTotal->SetString(2,StringC(bDdes));
		m_pTotal->SetString(3,StringC(bDdif));
		*/
	}
}

// ================================================================================================
// Description: clac distances
// ================================================================================================
void DCP::CalculationDist2PointsDialog::calc_distances(
						short p1, char* pid1, char* bXmea1, char* bXdes1, char* bYmea1, char* bYdes1,char* bZmea1, char* bZdes1,
						short p2, char* pid2, char* bXmea2, char* bXdes2, char* bYmea2, char* bYdes2,char* bZmea2, char* bZdes2)
{
	/*
	char bXmea[15], bYmea[15], bZmea[15]; 
	char bXdes[15], bYdes[15], bZdes[15]; 
	char bXdif[15], bYdif[15], bZdif[15];
	char bDmea[15], bDdes[15], bDdif[15];
	*/
	m_pCommon->strbtrim(pid1);
	m_pCommon->strbtrim(pid2);

	m_pCommon->get_dist_len1(bXmea1,bXmea2,bXmea,10);
	m_pCommon->get_dist_len1(bYmea1,bYmea2,bYmea,10);
	m_pCommon->get_dist_len1(bZmea1,bZmea2,bZmea,10);

	// ********************************************
	m_pCommon->get_dist_len1(bXdes1,bXdes2,bXdes,10);
	m_pCommon->get_dist_len1(bYdes1,bYdes2,bYdes,10);
	m_pCommon->get_dist_len1(bZdes1,bZdes2,bZdes,10);

	// ********************************************
	m_pCommon->get_dist_len(bXmea,bXdes,bXdif,10);
	m_pCommon->get_dist_len(bYmea,bYdes,bYdif,10);
	m_pCommon->get_dist_len(bZmea,bZdes,bZdif,10);

	// ********************************************
	// xyz tot
	m_pCommon->calc_di(bDmea,bXmea1,bYmea1,bZmea1,bXmea2,bYmea2,bZmea2);
	m_pCommon->calc_di(bDdes,bXdes1,bYdes1,bZdes1,bXdes2,bYdes2,bZdes2);

	m_pCommon->get_dist_len(bDmea,bDdes,bDdif,10);

	// point ids
	sPoints = StringC(pid1);
	sPoints += L"-";
	sPoints += StringC(pid2);

	char coord_line_buf[100];
	// x
	sprintf(coord_line_buf,"%10s %10s %6s", bXmea, bXdes, bXdif);
	sXline = StringC(coord_line_buf);
	
	//y
	sprintf(coord_line_buf,"%10s %10s %6s", bYmea, bYdes, bYdif);
	sYline = StringC(coord_line_buf);

	//z
	sprintf(coord_line_buf,"%10s %10s %6s", bZmea, bZdes, bZdif);
	sZline = StringC(coord_line_buf);

	// total
	sprintf(coord_line_buf,"%10s %10s %6s", bDmea, bDdes, bDdif);
	sTline = StringC(coord_line_buf);
}

// ================================================================================================
// Description: UpdateData
// ================================================================================================
void DCP::CalculationDist2PointsDialog::UpdateData()
{
	// nothing to update
}



// ================================================================================================
// Description: only accept CalculationDist2PointsModel objects
// ================================================================================================
bool DCP::CalculationDist2PointsDialog::SetModel( GUI::ModelC* pModel )
{
    // Verify type
    DCP::CalculationDist2PointsModel* pDcpModel = dynamic_cast< DCP::CalculationDist2PointsModel* >( pModel );

    // Call base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    if ( pDcpModel != nullptr && /*GUI::*/ModelHandlerC::SetModel( pDcpModel ))
    {
        RefreshControls();
        return true;
    }
    USER_APP_VERIFY( false );
    return false;
}


// ================================================================================================
// Description: get CalculationDist2PointsModel 
// ================================================================================================
DCP::CalculationDist2PointsModel* DCP::CalculationDist2PointsDialog::GetDataModel() const
{
    return (DCP::CalculationDist2PointsModel*) GetModel(); //lint !e1774 Could use dynamic_cast to 
                                                //downcast polymorphic type
}

// ================================================================================================
// ====================================  CalcDist2PointsControllerC  ==============================
// ================================================================================================


// ================================================================================================
// Description: // constructor
// ================================================================================================
DCP::CalculationDist2PointsController::CalculationDist2PointsController(DCP::Model* pModel)
    : m_pDlg( nullptr ),m_pModel(pModel)
{
	// Set title token
    // The appropriate application ID has to be set because 'C_DCP_APPLICATION_NAME_TOK'
    // is a token from the text database 'DCP05.men'
    
	// do not set title here
	//SetTitleTok( AT_DCP06,xxx_TOK);

    // Create a dialog
    m_pDlg = new DCP::CalculationDist2PointsDialog(pModel);  //lint !e1524 new in constructor for class 
    (void)AddDialog( CALC_2_POINTS_DLG, m_pDlg, true );

    // Set the function key
	FKDef vDef;
	//vDef.nAppId = AT_DCP06;
	vDef.poOwner = this;
	vDef.strLable = StringC(AT_DCP06,K_DCP_PICK_TOK);
    SetFunctionKey( FK1, vDef );

    vDef.strLable = StringC(AT_DCP06,K_DCP_CONT_TOK);
    SetFunctionKey( FK6, vDef );

    // Hide quit
	FKDef vDef1;
	//vDef1.nAppId = AT_DCP06;
    vDef1.poOwner = this;
	vDef1.strLable = L" ";;
	SetFunctionKey( SHFK6, vDef1 );

} //lint !e818 Pointer parameter could be declared as pointing to const

// ===========================================================================================
// Description: destructor
// ===========================================================================================
DCP::CalculationDist2PointsController::~CalculationDist2PointsController()
{

}
// ===========================================================================================
// Description: Route model to everybody else
// ===========================================================================================
bool DCP::CalculationDist2PointsController::SetModel( GUI::ModelC* pModel )
{
    // Set it to base class
    // Removed namespace for eVC compability (WinCE Compiler) 
    (void)/*GUI::*/ControllerC::SetModel( pModel );

	// set title here bacause it depending on caller...
	DCP::CalculationDist2PointsModel* pMod = dynamic_cast< DCP::CalculationDist2PointsModel* >( pModel );
	SetTitle(pMod->sTitle);
	
    // Set it to hello world dialog
    return m_pDlg->SetModel( pModel );
}

// ================================================================================================
// Description: F1, Select 2 points
// ================================================================================================
void DCP::CalculationDist2PointsController::OnF1Pressed()
{	
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

	// SELECT MULTIPOINTS
	DCP::SelectMultiPointsModel* pModel = new SelectMultiPointsModel;
	memcpy(&pModel->sel_points[0],&m_pDlg->GetDataModel()->points[0], sizeof(S_SELECT_POINTS) * MAX_POINTS_IN_FILE);
	pModel->m_iPointsCount = m_pDlg->GetDataModel()->m_iCounts;
	
	pModel->m_iDef = BOTH;
	pModel->sSelectedFile = m_pDlg->GetDataModel()->sFile;
	pModel->m_iMinPoint = 2;
	pModel->m_iMaxPoint = 2;
	
	// set info text....
	StringC sInfo;
	sInfo.LoadTxt(AT_DCP06, T_DCP_SELECT_2_POINTS_TOK);
	sInfo += L"  ";
	sInfo += pModel->sSelectedFile;
	pModel->sInfo = sInfo;

	// set title
	pModel->sTitle = GetTitle();
	
	// set help token
	pModel->helpToken = H_DCP_SEL_MULTI_A_AND_D_TOK;

	if(GetController(SELECT_MULTIPOINTS_CONTROLLER) == nullptr)
	{
			(void)AddController( SELECT_MULTIPOINTS_CONTROLLER, new DCP::SelectMultiPointsController(m_pModel) );
	}

	(void)GetController( SELECT_MULTIPOINTS_CONTROLLER )->SetModel(pModel);
	SetActiveController(SELECT_MULTIPOINTS_CONTROLLER, true);
	
}

// ================================================================================================
// Description: F6, CONT
// ================================================================================================
void DCP::CalculationDist2PointsController::OnF6Pressed()
{
    if (m_pDlg == nullptr)
    {
        USER_APP_VERIFY( false );
        return;
    }

    // Update model
    // Set it to hello world dialog
    m_pDlg->UpdateData();

    // Remove the following statement if you don't want an exit
    // to the main menu
    (void)Close(EC_KEY_CONT);
}

// ================================================================================================
// Description: React on close of tabbed dialog
// ================================================================================================
void DCP::CalculationDist2PointsController::OnActiveDialogClosed( int lDlgID, int lExitCode )
{
}

// ================================================================================================
// Description: React on close of controller
// ================================================================================================
void DCP::CalculationDist2PointsController::OnActiveControllerClosed( int lCtrlID, int lExitCode )
{
	if(lCtrlID == SELECT_MULTIPOINTS_CONTROLLER && lExitCode == EC_KEY_CONT)
	{
		DCP::SelectMultiPointsModel* pModel = (DCP::SelectMultiPointsModel*) GetController( SELECT_MULTIPOINTS_CONTROLLER )->GetModel();		
		Common common(m_pModel);
		
		short p1 = 0, p2 = 0;

		p1 = pModel->nro_table[0][0];
		p2 = pModel->nro_table[1][0];

		if(p1 == 0 || p2 == 0)
		{
			MsgBox msgBox;
			StringC sMsg;
			sMsg.LoadTxt(AT_DCP06, M_DCP_2_POINTS_MUST_BE_DEFINED_TOK);
			msgBox.ShowMessageOk(sMsg);
		}
		else
		{
			char bXmea1[15], bYmea1[15], bZmea1[15];
			char bXdes1[15], bYdes1[15], bZdes1[15],pid1[7];
				
			char bXmea2[15], bYmea2[15], bZmea2[15];
			char bXdes2[15], bYdes2[15], bZdes2[15],pid2[7];
				
		
			m_pDlg->GetDataModel()->pAdfFile->form_pnt1((int) p1, pid1, nullptr, bXmea1, bXdes1, nullptr, bYmea1, bYdes1, nullptr, bZmea1, bZdes1, nullptr);
			m_pDlg->GetDataModel()->pAdfFile->form_pnt1((int) p2, pid2, nullptr, bXmea2, bXdes2, nullptr, bYmea2, bYdes2, nullptr, bZmea2, bZdes2, nullptr);
			
			m_pDlg->calc_distances(p1, pid1, bXmea1, bXdes1, bYmea1, bYdes1,bZmea1, bZdes1,
									p2, pid2, bXmea2, bXdes2, bYmea2, bYdes2,bZmea2, bZdes2);
		}
	}

	m_pDlg->RefreshControls();
	DestroyController( lCtrlID );
}

// ================================================================================================
// ====================================  CalculationDist2PointsModel  ===================================
// ================================================================================================


// ===========================================================================================
// Description: Constructor
// ===========================================================================================
DCP::CalculationDist2PointsModel::CalculationDist2PointsModel()
{
	memset(&points[0],0,sizeof(S_SELECT_POINTS) * MAX_POINTS_IN_FILE); 
}

// ===========================================================================================
// Description: Destructor
// ===========================================================================================
DCP::CalculationDist2PointsModel::~CalculationDist2PointsModel()
{
}
