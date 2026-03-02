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

#ifndef DCP_HIDDENPOINTDLG_HPP
#define DCP_HIDDENPOINTDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_PointBuffModel.hpp>

#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
//#include <GUI_Timer.hpp>

#include <cfa_Types.hpp>

#include <GSV_GeospatialViewDialog.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class DCP06ModelC;
	class DCP06HiddenPointDlgC;

    // Description: Tabbed controller for the Hello World application
    class DCP06HiddenPointControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06HiddenPointControllerC(DCP06ModelC *pDCP06Model);
			~DCP06HiddenPointControllerC();

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
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			void show_function_keys();
			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP06HiddenPointControllerC( const DCP06HiddenPointControllerC& oDCP06HiddenPointController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06HiddenPointControllerC& operator=( const DCP06HiddenPointControllerC& oDCP06HiddenPointController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06HiddenPointDlgC* m_pDlg;
			DCP06ModelC *m_pDCP06Model;
			DCP06CommonC* m_pCommon;
			bool m_bCamera;
			GSV::GeospatialViewDialogC* poVideoDlg;
			bool isATR;
    };


	class DCP06HiddenPointDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				ePointNo=1,
				ePointDist,
				eX,
				eY,
				eZ,
				eInfo
			};

		   DCP06HiddenPointDlgC(DCP06ModelC *pDCP06Model);

            // Description: Destructor
            virtual ~DCP06HiddenPointDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP06PointBuffModelC* GetDataModel() const;


			virtual void PointNext();
			virtual void PointPrev();
			virtual void PointDelete();
			virtual void update_meas_values(double x, double y, double z, short /*DCP_POINT_STATUS*/ status);
			virtual short calc_hidden_point();

			short get_current_point();

	protected:
  			
			GUI::ComboLineCtrlC* m_pPointNo;
			GUI::ComboLineCtrlC* m_pPointDist;
			GUI::ComboLineCtrlC* m_pX;
			GUI::ComboLineCtrlC* m_pY;
			GUI::ComboLineCtrlC* m_pZ;
			//GUI::TextCtrlC* m_pInfo;
			DCP06ModelC *m_pDCP06Model;
	private:
			S_POINT_BUFF measured_points[MAX_POINTS_IN_HIDDENPOINT_BAR];
			short m_iCurrentPoint;
			
			S_POINT_BUFF hiddenpoint[1];
			
			short iInfoInd;
			StringC strInfoText;
			//virtual void OnTimer(void);
			//GUI::TimerC m_pTimer;
			DCP06CommonC* m_pCommon;
	};

	class DCP06HiddenPointDlgBaseC: public GUI::ControllerC
	{
	public:
			DCP06HiddenPointDlgBaseC(int iCtrlId, DCP06ModelC* pDCP06Model ,DCP06PointBuffModelC* pDCPPointBuffModel);
			
	};
};

#endif // DCP_BASEGUI_HPP




