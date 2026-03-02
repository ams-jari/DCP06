
#include "stdafx.h"
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_Model.hpp>
#include <dcp06/core/UserDefStatus.hpp>

	DCP::DCP05UserDefStatusDlgC::DCP05UserDefStatusDlgC(DCP05ModelC* pDCP05Model, bool defined_):
		m_pText1(0),m_pDCP05Model(pDCP05Model),Defined(defined_)
	{
		//SetTxtApplicationId(AT_DCP05);
	}

	DCP::DCP05UserDefStatusDlgC::~DCP05UserDefStatusDlgC()
	{

	}

	void DCP::DCP05UserDefStatusDlgC::OnInitDialog(void)
	{
		ResetFunctionKeys();

		FKDef vDef;
		//vDef.nAppId = AT_DCP05;
		vDef.poOwner = this;
		
		if(Defined)
		{
			vDef.strLable = StringC(AT_DCP05,K_DCP_CONFIG_TOK);
			SetFunctionKey( FK1, vDef );
		}
		vDef.strLable = StringC(AT_DCP05,K_DCP_MEAS_TOK);
		SetFunctionKey( FK3, vDef );

	  // Hide quit 
		FKDef vDef1;
		//vDef1.nAppId = AT_DCP05;
		vDef1.poOwner = this;
		vDef1.strLable = L" ";;
		SetFunctionKey( SHFK6, vDef1 );
		SetFunctionKey( SHFK2, vDef1 );

		//m_pText = new GUI::TextBoxCtrlC(0,10,300,120);
		m_pText1 = new GUI::TextCtrlC();
		m_pText1->SetId(1);
		AddCtrl(m_pText1);
		
		SetTitle(StringC(AT_DCP05,T_DCP_USERDEF_TOK));

		// TODO VIVA
		/*
		if(Defined)
			SetHelpTok(H_DCP_USERDEF_DEFINED_TOK,0);
		else
			SetHelpTok(H_DCP_USERDEF_NOT_DEFINED_TOK,0);
		*/
	}

	void DCP::DCP05UserDefStatusDlgC::OnDialogActivated()
	{
		RefreshControls();
	} 

	void DCP::DCP05UserDefStatusDlgC::OnF1Pressed()
	{
		Close(100);
	}

	void DCP::DCP05UserDefStatusDlgC::OnF3Pressed()
	{
		Close(200);
	}
	void DCP::DCP05UserDefStatusDlgC::RefreshControls()
	{
		if(m_pText1)
		{	
			if(!Defined)
			{
				m_pText1->SetText(StringC(AT_DCP05,M_DCP_USERDEF_NOT_DEF_TOK));
			}
			else
			{
				m_pText1->SetText(StringC(AT_DCP05,M_DCP_USERDEF_DEFINED_TOK));
			}
		}
	}