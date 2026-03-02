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

#ifndef DCP_PLANEESCANSETUPDLG_HPP
#define DCP_PLANESCANSETUPDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
#include "calc.h"
#include <dcp06/measurement/DCP_ScanFileFunc.hpp>
#include <dcp06/file/DCP_File.hpp>
#include <dcp06/measurement/DCP_MeasXYZ.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class DCP05ModelC;
	class DCP05PlaneScanDlgC;
	class DCP05PlaneScanModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP05PlaneScanControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05PlaneScanControllerC(DCP05ModelC* pDCP05Model);
			~DCP05PlaneScanControllerC();

            // Description: Handle change of position values
           	virtual void OnF1Pressed(void);
			//virtual void OnF2Pressed(void);
			//virtual void OnF3Pressed(void);
			virtual void OnF4Pressed(void);
			virtual void OnF5Pressed(void);
			virtual void OnF6Pressed(void);
			virtual void OnSHF2Pressed(void);
			//virtual void OnSHF3Pressed(void);
			//virtual void OnSHF4Pressed(void);
			virtual void OnSHF5Pressed(void);
			
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int /*lExitCode*/ );

        private:

			
            // Description: Copy constructor
            // Remarks    : not implemented
            DCP05PlaneScanControllerC( const DCP05PlaneScanControllerC& oDCP05PlaneScanController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05PlaneScanControllerC& operator=( const DCP05PlaneScanControllerC& oDCP05PlaneScanController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05PlaneScanDlgC* m_pDlg;
			DCP05PlaneScanModelC* m_pDataModel;
			bool CancelMeas;
			DCP05CommonC* m_pCommon;
			DCP05ModelC* m_pDCP05Model;
			//DCP::DCP05MeasXYZModelC* pMeasXYZModel;
    };


    class DCP05PlaneScanDlgC:public GUI::StandardDialogC, public OBS::CommandC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eFile=1,
				ePointId,
				eBoundary, 
				eResolutionHeigth,
				eResolutionWidth
			};

			DCP05PlaneScanDlgC(DCP05PlaneScanModelC* pPlaneScanModel,DCP05PlaneScanControllerC* controller);

            // Description: Destructor
            virtual ~DCP05PlaneScanDlgC();

			virtual void OnInitDialog(void);

			//virtual void OnF1Pressed(void);
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP05ModelC* GetDCP05Model() const;

			virtual void RefreshControls();
			

	protected:
			
			GUI::ComboLineCtrlC* m_pFile;
			GUI::ComboLineCtrlC* m_pPointId;
			GUI::ComboLineCtrlC* m_pBoundary;
			GUI::ComboLineCtrlC* m_pResolutionHeigth;
			GUI::ComboLineCtrlC* m_pResolutionWidth;

			
			OBS_DECLARE_EXECUTE(DCP05PlaneScanDlgC);

			DCP05PlaneScanModelC* m_pDataModel;
            

	private:
			OBS::ObserverC m_pResolutionHeigthObserver;
			OBS::ObserverC m_pResolutionWidthObserver;
			OBS::ObserverC m_pFileObserver;
			OBS::ObserverC m_pPointIdObserver;

			virtual void OnValueChanged(int unNotifyCode, int ulParam2);

			DCP05PlaneScanControllerC* pController;
	};

   class DCP05PlaneScanModelC : public GUI::ModelC
    {
        public: 

            // Description: Constructor
            //
            DCP05PlaneScanModelC(DCP05ModelC* pDCP05Model);
			//DCP05DomModelC(DCP05DomModelC* pModel);

            // Description: Destructor
            //
            virtual ~DCP05PlaneScanModelC();

			short iResolutionHeigth;
			short iResolutionWidth;
			StringC sFileName;
			StringC sPointId;
			ScanFileFunc* m_pScanFile;
			S_PLANE_BUFF boundary_plane[1];
			//S_POINT_BUFF boundary_points[3];
			bool generate_points(DCP05ModelC *pDCP05Model);
			bool isRunning;
			///DCP05FileModelC fileModel;

			
			S_SCAN_POINT_BUFF des_points[MAX_SCAN_POINTS];
			int current_point;
			unsigned int points_count;
			unsigned int total_points;
	private:	
			bool add_line_points(ScanFileFunc* m_pScanFile, struct line* line1, 
							short i, short pointCountWidth, double distWidth, double resolutionWidth, char* first_pointid,
							DCP05ModelC *pDCP05Model);

			
    };
	
};

#endif // DCP_BASEGUI_HPP




