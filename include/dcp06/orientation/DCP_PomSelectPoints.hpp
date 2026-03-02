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

#ifndef DCP_POMSELECTPOINTS_DLG_HPP
#define DCP_POMSELECTPOINTS_DLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Common.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>

#include <dcp06/file/DCP_AdfFileFunc.hpp>
// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class DCP05ModelC;
	class DCP05PomSelectPointsDlgC;
	class DCP05PomSelectPointsModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP05PomSelectPointsControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05PomSelectPointsControllerC(DCP05ModelC* pDCP05Model);
			~DCP05PomSelectPointsControllerC();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			virtual void OnF2Pressed();
			virtual void OnF3Pressed();
			virtual void OnF4Pressed();
			virtual void OnF5Pressed();
			virtual void OnF6Pressed();
			virtual void OnSHF2Pressed();
			//virtual void OnSHF3Pressed();
			//virtual void OnSHF4Pressed();
			//virtual void OnSHF5Pressed();
			//virtual void OnSHF6Pressed();
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			
			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP05PomSelectPointsControllerC( const DCP05PomSelectPointsControllerC& oDCP05PomSelectPointsController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05PomSelectPointsControllerC& operator=( const DCP05PomSelectPointsControllerC& oDCP05PomSelectPointsController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05PomSelectPointsDlgC* m_pDlg;
			DCP05ModelC* m_pDCP05Model;
			//AdfFileFunc* adf;
			void save_current_point();
    };


	class DCP05PomSelectPointsDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC, public OBS::CommandC
	{
		 public:

			enum eCtrlId
			{
				ePointNo=1,
				ePointId,
				eX,
				eY,
				eZ
			};

		   DCP05PomSelectPointsDlgC(DCP05ModelC* pDCP05Model);

            // Description: Destructor
            virtual ~DCP05PomSelectPointsDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP05PomSelectPointsModelC* GetDataModel() const;

			virtual void PointNext();
			virtual void PointPrev();
			virtual void SelectPoint(short iId);
			void SetFocusToPoint();

	protected:
  			
			GUI::ComboLineCtrlC* m_pPointNo;
			GUI::ComboLineCtrlC* m_pPointId;
			GUI::ComboLineCtrlC* m_pX;
			GUI::ComboLineCtrlC* m_pY;
			GUI::ComboLineCtrlC* m_pZ;

			DCP05ModelC* m_pDCP05Model;

			OBS_DECLARE_EXECUTE(DCP05PomSelectPointsDlgC);
	private:
		OBS::ObserverC m_pPointIdObserver;
		OBS::ObserverC m_pXObserver;
		OBS::ObserverC m_pYObserver;
		OBS::ObserverC m_pZObserver;
		virtual void OnPointIdChanged( int unNotifyCode, int ulParam2);
		DCP05CommonC* m_pCommon;
	};

	 class DCP05PomSelectPointsModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP05PomSelectPointsModelC();

            // Description: Destructor
            //
            virtual ~DCP05PomSelectPointsModelC();
			
			S_POINT_BUFF points[MAX_SELECT_POINTS];
			S_POINT_BUFF points1[MAX_SELECT_POINTS];
			
			short iMaxPoint;
			short iMinPoint;
			short iDisplay;
			short iCurrentPoint;
			short m_iPointsCount;

			char default_pid[10];
			bool disable_point_editing;

    };
};

#endif // DCP_BASEGUI_HPP




