// ====================================================================================================================
//
// Project  : Pluto/Venus Onboard Applications SW
//
// Component: 
//
// $Workfile: HEW_DCP05GUI.hpp $
//
// Summary  : 
//
// ------------------------------------------------------------------------------------------------
//
// Copyright by Leica Geosystems AG, Heerbrugg 2002
//
// ================================================================================================


// $Author: Hlar $
// $Date: 6.07.04 8:55 $
// $Revision: 1 $
// $Modtime: 5.07.04 14:55 $

/* $ History: $
*/
// $NoKeywords: $

#ifndef DCP_RES_LINE_DLG_HPP
#define DCP_RES_LINE_DLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/orientation/DCP_DefineLine.hpp>
#include <dcp06/core/DCP_Common.hpp>

#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_TableDialog.hpp>
#include <GUI_ListMultiColCtrl.hpp>
#include <GUI_AppBase.hpp>
//#include <GUI_Timer.hpp>
// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class DCP06ModelC;
	class DCP06ResLineDlgC;
	//class DCP06SelectOnePointModelC;

    // Description: Tabbed controller for the Hello World application
	
    class DCP06ResLineControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06ResLineControllerC(DCP06ModelC *pDCP06Model);
			~DCP06ResLineControllerC();

            // Description: Handle change of position values
            //virtual void OnF1Pressed();
			//virtual void OnF2Pressed();
			//virtual void OnF5Pressed();
			virtual void OnF6Pressed();
			//virtual void OnSHF1Pressed();
			//virtual void OnSHF2Pressed();
			//virtual void OnSHF5Pressed();

            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			void OnActiveControllerClosed( int lCtrlID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP06ResLineControllerC( const DCP06ResLineControllerC& oDCP06ResLineController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06ResLineControllerC& operator=( const DCP06ResLineControllerC& oDCP06ResLineController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06ResLineDlgC* m_pDlg;
			DCP06ModelC* m_pDCP06Model;
    };
	

	class DCP06ResLineDlgC: public GUI::TableDialogC,public GUI::ModelHandlerC//, public OBS::CommandC
	{
	 public:

			 enum eCtrlId
			{
				eInfo=1,
				eMulti=1
			 };

			/*
			enum CW_ColumnWidth
			{
				CW_No = GUI::ListConfigColsC::CW_5,
				CW_Point = GUI::ListConfigColsC::CW_10,
				CW_Deviation = GUI::ListConfigColsC::CW_5
				
			};
			*/
			enum CI_ColumnId
			{
				CI_No	=1,
				CI_Point,
				CI_Deviation
			};

		   DCP06ResLineDlgC(DCP06ModelC *pDCP06Model);

            // Description: Destructor
            virtual ~DCP06ResLineDlgC();

			virtual void OnInitDialog(void);

			virtual void OnF1Pressed(void);
			//virtual void OnF6Pressed();

			virtual void OnDialogActivated();
			virtual void RefreshControls();

			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP06DefineLineModelC* GetDataModel() const;

			//OBS::ObserverC m_pMultiColCtrlObserver;
			//virtual void OnChanged(int unNotifyCode, int ulParam2);

		protected:
			GUI::TextCtrlC* m_pInfo1;
			GUI::ListMultiColCtrlC* poMultiColCtrl;	
			DCP06ModelC* m_pDCP06Model;
			// Description: add all controls
       

			//OBS_DECLARE_EXECUTE(DCP06SelectOnePointDlgC);
		
		private:
				//S_SELECT_POINTS sel_points[MAX_POINTS_IN_FILE];
				StringC sActualSelected;
				StringC sActualNonSelected;
				StringC sDesignSelected;
				StringC sDesignNonSelected;
				//virtual void OnTimer(void);

				DCP06CommonC* m_pCommon;
				double calc_pdist(S_LINE_BUFF *line, short pno);
				double get_max_dist_and_rms_line(S_LINE_BUFF *line, short *pno, double *rms/*, short ACT*/);

				StringC sTitle;
				

				//void DCP::DCP06SelectMultiPointsDlgC::remove_point_table(short sel);
				//void DCP::DCP06SelectMultiPointsDlgC::add_point_table(short sel, bool bActual);
 	};


	
	
	class DCP06ResLineModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06ResLineModelC();

            // Description: Destructor
            //
            virtual ~DCP06ResLineModelC();
			S_LINE_BUFF line[1];
			StringC sCaption;
			/*			
			short m_iPointsCount;
			short m_iCurrentPoint;
			short m_iDef;
		
			S_SELECT_POINTS points[MAX_POINTS_IN_FILE];
			//short nro_table[MAX_POINTS_IN_FILE][2];
			short iActual_or_design;
			short iSelectedNo;
			StringC sSelectedFile;
			*/
    };
};

#endif // DCP_BASEGUI_HPP




