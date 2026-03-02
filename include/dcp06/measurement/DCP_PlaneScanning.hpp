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
    class DCP06ModelC;
	class DCP06PlaneScanDlgC;
	class DCP06PlaneScanModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP06PlaneScanControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06PlaneScanControllerC(DCP06ModelC* pDCP06Model);
			~DCP06PlaneScanControllerC();

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
            DCP06PlaneScanControllerC( const DCP06PlaneScanControllerC& oDCP05PlaneScanController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06PlaneScanControllerC& operator=( const DCP06PlaneScanControllerC& oDCP05PlaneScanController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06PlaneScanDlgC* m_pDlg;
			DCP06PlaneScanModelC* m_pDataModel;
			bool CancelMeas;
			DCP06CommonC* m_pCommon;
			DCP06ModelC* m_pDCP06Model;
			//DCP::DCP06MeasXYZModelC* pMeasXYZModel;
    };


    class DCP06PlaneScanDlgC:public GUI::StandardDialogC, public OBS::CommandC, public GUI::ModelHandlerC
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

			DCP06PlaneScanDlgC(DCP06PlaneScanModelC* pPlaneScanModel,DCP06PlaneScanControllerC* controller);

            // Description: Destructor
            virtual ~DCP06PlaneScanDlgC();

			virtual void OnInitDialog(void);

			//virtual void OnF1Pressed(void);
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP06ModelC* GetDCP06Model() const;

			virtual void RefreshControls();
			

	protected:
			
			GUI::ComboLineCtrlC* m_pFile;
			GUI::ComboLineCtrlC* m_pPointId;
			GUI::ComboLineCtrlC* m_pBoundary;
			GUI::ComboLineCtrlC* m_pResolutionHeigth;
			GUI::ComboLineCtrlC* m_pResolutionWidth;

			
			OBS_DECLARE_EXECUTE(DCP06PlaneScanDlgC);

			DCP06PlaneScanModelC* m_pDataModel;
            

	private:
			OBS::ObserverC m_pResolutionHeigthObserver;
			OBS::ObserverC m_pResolutionWidthObserver;
			OBS::ObserverC m_pFileObserver;
			OBS::ObserverC m_pPointIdObserver;

			virtual void OnValueChanged(int unNotifyCode, int ulParam2);

			DCP06PlaneScanControllerC* pController;
	};

   class DCP06PlaneScanModelC : public GUI::ModelC
    {
        public: 

            // Description: Constructor
            //
            DCP06PlaneScanModelC(DCP06ModelC* pDCP06Model);
			//DCP06DomModelC(DCP06DomModelC* pModel);

            // Description: Destructor
            //
            virtual ~DCP06PlaneScanModelC();

			short iResolutionHeigth;
			short iResolutionWidth;
			StringC sFileName;
			StringC sPointId;
			ScanFileFunc* m_pScanFile;
			S_PLANE_BUFF boundary_plane[1];
			//S_POINT_BUFF boundary_points[3];
			bool generate_points(DCP06ModelC *pDCP06Model);
			bool isRunning;
			///DCP06FileModelC fileModel;

			
			S_SCAN_POINT_BUFF des_points[MAX_SCAN_POINTS];
			int current_point;
			unsigned int points_count;
			unsigned int total_points;
	private:	
			bool add_line_points(ScanFileFunc* m_pScanFile, struct line* line1, 
							short i, short pointCountWidth, double distWidth, double resolutionWidth, char* first_pointid,
							DCP06ModelC *pDCP06Model);

			
    };
	
};

#endif // DCP_BASEGUI_HPP




