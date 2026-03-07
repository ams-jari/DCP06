// ====================================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: 
//
// $Workfile: HEW_DCP05GUI.hpp $
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

#ifndef DCP_LINEFITMEASDLG_HPP
#define DCP_LINEFITMEASDLG_HPP

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
#include <dcp06/core/Model.hpp>
#include <dcp06/calculation/CalculationLineFit.hpp>
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
	class Model;
	class LineFitMeasDialog;
	class LineFitMeasModel;
	//class DCPSurveyModelC; 

    // Description: Tabbed controller for the Hello World application
	class LineFitMeasController : public GUI::ControllerC, public TBL::MeasurementC//, public GUI::StandardDialogC
    {
        public:

            // Description: Constructor
            LineFitMeasController(Model *pModel);
			~LineFitMeasController();

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
			
		//protected:
			//virtual void OnOperationDistEvent(int unNotifyCode, int ulOperationId);
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            LineFitMeasController( const LineFitMeasController& oLineFitMeasController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            LineFitMeasController& operator=( const LineFitMeasController& oLineFitMeasController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            LineFitMeasDialog* m_pDlg;
			Model *m_pModel;
			Common* m_pCommon;
			bool m_bCamera;
			GSV::GeospatialViewDialogC* poVideoDlg;
			bool isATR;
			void calculate_linefit(short iCurrPoint);
			CalcLineFit* calcLineFit;
    };


	class LineFitMeasDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC, public OBS::CommandC
	{
		 public:

			enum eCtrlId
			{
				ePointNo=1,
				ePointId,
				eHeight,
				eDistFromLine, // interpolation
				eDistAlongLine, // distance from first point
				eLineOffset,

				/*eX,
				eY,
				eZ,*/
				eInfo
			};

		   LineFitMeasDialog(Model *pModel);

            // Description: Destructor
            virtual ~LineFitMeasDialog();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			LineFitMeasModel* GetDataModel() const;

			virtual void delete_point();
			virtual void next_point();
			virtual void prev_point();
			virtual void add_point();
			virtual void update_meas_values(double x, double y, double z, short/*DCP_POINT_STATUS*/ status);
			virtual void SelectPoint(StringC strSelectedPoint,short iSelectedPointId);

			
	protected:
  			
			GUI::ComboLineCtrlC* m_pPointNo;
			GUI::ComboLineCtrlC* m_pPointId;
			/*GUI::ComboLineCtrlC* m_pX;
			GUI::ComboLineCtrlC* m_pY;
			GUI::ComboLineCtrlC* m_pZ;*/
			GUI::ComboLineCtrlC* m_pHeight;
			GUI::ComboLineCtrlC* m_pDistFromLine;
			GUI::ComboLineCtrlC* m_pDistAlongLine;
			GUI::ComboLineCtrlC* m_pLineOffset;
			//GUI::TextCtrlC* m_pInfo;

			OBS_DECLARE_EXECUTE(LineFitMeasDialog);

			


	private:
		OBS::ObserverC m_pPointIdObserver;
		virtual void OnPointIdChanged( int unNotifyCode,  int ulParam2);

		//virtual void OnTimer(void);
		//GUI::TimerC m_pTimer;

		Model *m_pModel;

		//S_POINT_BUFF temp_point_table[20];
		//S_POINT_BUFF temp_point_table2[20];

		bool CheckDistance(double x, double y, double z);
		double to_mm(double dist);
		short iInfoInd;
		StringC strInfoText;
		Common* m_pCommon;
		
	};


   class LineFitMeasModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            LineFitMeasModel();

            // Description: Destructor
            //
            virtual ~LineFitMeasModel();
						
			short m_iMinPoint;
			short m_iMaxPoint;
			short m_iPointsCount;
			short m_iCurrentPoint;

			S_POINT_BUFF point_table[MAX_LINEFIT_POINTS];
			S_POINT_BUFF points_in_line[MAX_LINEFIT_POINTS];

			//S_POINT_BUFF point_table2[20];

			char default_pid[10];
			bool disable_point_editing;

			S_LINE_BUFF* pLline_buff;
			S_LINE_BUFF* pLline_buff_ocs;
			Alignment321Model* align321Model;
			//S_POINT_BUFF points_buff[MAX_LINEFIT_POINTS];	
			S_LINE_FITTING_RESULTS linefit_results[MAX_LINEFIT_POINTS];
			double manualHeight;
			double shiftValue;
			double rotateAngle;

			int selectedHeight;
			int selectedShift;
			int selectedRotate;
			int selectedRefLine;


    };
	

};

#endif // DCP_BASEGUI_HPP




