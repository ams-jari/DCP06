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

#ifndef DCP_XORYORZ_HPP
#define DCP_XORYORZ_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_PointBuffModel.hpp>
#include <dcp06/core/DCP_Common.hpp>

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
	class DCP05ModelC;
	class DCP05XYZDlgC;


    // Description: Tabbed controller for the Hello World application
    class DCP05XYZControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05XYZControllerC(DCP05ModelC *pDCP05Model);
			~DCP05XYZControllerC();

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
            DCP05XYZControllerC( const DCP05XYZControllerC& oDCP05XYZController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05XYZControllerC& operator=( const DCP05XYZControllerC& oDCP05XYZController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05XYZDlgC* m_pDlg;
			DCP05ModelC * m_pDCP05Model;
			DCP05CommonC* m_pCommon;
			bool m_bCamera;
			GSV::GeospatialViewDialogC* poVideoDlg;
			bool isATR;
    };


	class DCP05XYZDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				ePointId=1,
				eX,
				eY,
				eZ,
				eInfo
			};

		   DCP05XYZDlgC(DCP05ModelC *pDCP05Model);

            // Description: Destructor
            virtual ~DCP05XYZDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP05PointBuffModelC* GetDataModel() const;

			virtual void delete_point();

	protected:
  			
			GUI::ComboLineCtrlC* m_pPointId;
			GUI::ComboLineCtrlC* m_pX;
			GUI::ComboLineCtrlC* m_pY;
			GUI::ComboLineCtrlC* m_pZ;
			//GUI::TextCtrlC* m_pInfo;

			DCP05ModelC * m_pDCP05Model;
	private:
			short iInfoInd;
			StringC strInfoText;
			//virtual void OnTimer(void);
			//GUI::TimerC m_pTimer;
			DCP05CommonC* m_pCommon;
			
	};
};

#endif // DCP_BASEGUI_HPP




