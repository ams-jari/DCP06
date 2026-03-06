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

#ifndef DCP_MEASDLG_HPP
#define DCP_MEASDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/Common.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
//#include <GUI_Timer.hpp>
#include <TBL_Measurement.hpp>

#include <cfa_Types.hpp>
#include <GSV_GeospatialViewDialog.hpp>

namespace ABL
{
    // Description: Forward declaration
    class SurveyModelC;
}

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class DCP06ModelC;
	class DCP06MeasDlgC;
	class DCP06MeasModelC;
	//class DCPSurveyModelC; 

    // Description: Tabbed controller for the Hello World application
	class DCP06MeasControllerC : public GUI::ControllerC, public TBL::MeasurementC//, public GUI::StandardDialogC
    {
        public:

            // Description: Constructor
            DCP06MeasControllerC(DCP06ModelC *pDCP06Model);
			~DCP06MeasControllerC();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			virtual void OnF2Pressed();
			virtual void OnF3Pressed();
			virtual void OnF4Pressed();
			virtual void OnF5Pressed();
			virtual void OnF6Pressed();
			virtual void OnSHF1Pressed();
			virtual void OnSHF2Pressed();
			virtual void OnSHF3Pressed();
			virtual void OnSHF4Pressed();
			virtual void OnSHF5Pressed();
			virtual void OnSHF6Pressed();
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );

			virtual void OnControllerActivated(void);
            virtual void OnControllerClosed(int lExitCode);

			void show_function_keys();

			//DCPSurveyModelC*   poSurveyModel;
			
		protected:
			//virtual void OnOperationDistEvent(int unNotifyCode, int ulOperationId);
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP06MeasControllerC( const DCP06MeasControllerC& oDCP05MeasController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06MeasControllerC& operator=( const DCP06MeasControllerC& oDCP05MeasController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06MeasDlgC* m_pDlg;
			DCP06ModelC *m_pDCP06Model;
			DCP06CommonC* m_pCommon;
			bool m_bCamera;
			GSV::GeospatialViewDialogC* poVideoDlg;
			bool isATR;
    };


	class DCP06MeasDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC, public OBS::CommandC
	{
		 public:

			enum eCtrlId
			{
				ePointNo=1,
				ePointId,
				eX,
				eY,
				eZ,
				eInfo
			};

		   DCP06MeasDlgC(DCP06ModelC *pDCP06Model);

            // Description: Destructor
            virtual ~DCP06MeasDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP06MeasModelC* GetDataModel() const;

			virtual void delete_point();
			virtual void next_point();
			virtual void prev_point();
			virtual void add_point();
			virtual void update_meas_values(double x, double y, double z, short/*DCP_POINT_STATUS*/ status);
			virtual void SelectPoint(StringC strSelectedPoint,short iSelectedPointId);

			
	protected:
  			
			GUI::ComboLineCtrlC* m_pPointNo;
			GUI::ComboLineCtrlC* m_pPointId;
			GUI::ComboLineCtrlC* m_pX;
			GUI::ComboLineCtrlC* m_pY;
			GUI::ComboLineCtrlC* m_pZ;
			//GUI::TextCtrlC* m_pInfo;

			OBS_DECLARE_EXECUTE(DCP06MeasDlgC);

			


	private:
		OBS::ObserverC m_pPointIdObserver;
		virtual void OnPointIdChanged( int unNotifyCode,  int ulParam2);

		/*virtual void OnTimer(void);
		GUI::TimerC m_pTimer;*/

		DCP06ModelC *m_pDCP06Model;

		//S_POINT_BUFF temp_point_table[20];
		//S_POINT_BUFF temp_point_table2[20];

		bool CheckDistance(double x, double y, double z);
		double to_mm(double dist);
		short iInfoInd;
		StringC strInfoText;
		DCP06CommonC* m_pCommon;
		
	};


   class DCP06MeasModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06MeasModelC();

            // Description: Destructor
            //
            virtual ~DCP06MeasModelC();
						
			short m_iMinPoint;
			short m_iMaxPoint;
			short m_iPointsCount;
			short m_iCurrentPoint;

			S_POINT_BUFF point_table[20];
			S_POINT_BUFF point_table2[20];

			char default_pid[10];
			bool disable_point_editing;
    };
	

};

#endif // DCP_BASEGUI_HPP




