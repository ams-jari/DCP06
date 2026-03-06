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

#ifndef DCP_PLANEESCANSETUPDLG_HPP
#define DCP_PLANESCANSETUPDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
#include "calc.h"
#include <dcp06/measurement/ScanFileFunc.hpp>
#include <dcp06/file/File.hpp>
#include <dcp06/measurement/MeasXYZ.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class Model;
	class PlaneScanDialog;
	class PlaneScanModel;

    // Description: Tabbed controller for the Hello World application
    class PlaneScanController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            PlaneScanController(Model* pModel);
			~PlaneScanController();

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
            PlaneScanController( const PlaneScanController& oPlaneScanController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            PlaneScanController& operator=( const PlaneScanController& oPlaneScanController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            PlaneScanDialog* m_pDlg;
			PlaneScanModel* m_pDataModel;
			bool CancelMeas;
			Common* m_pCommon;
			Model* m_pModel;
			//DCP::MeasXYZModel* pMeasXYZModel;
    };


    class PlaneScanDialog:public GUI::StandardDialogC, public OBS::CommandC, public GUI::ModelHandlerC
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

			PlaneScanDialog(PlaneScanModel* pPlaneScanModel,PlaneScanController* controller);

            // Description: Destructor
            virtual ~PlaneScanDialog();

			virtual void OnInitDialog(void);

			//virtual void OnF1Pressed(void);
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            Model* GetModel() const;

			virtual void RefreshControls();
			

	protected:
			
			GUI::ComboLineCtrlC* m_pFile;
			GUI::ComboLineCtrlC* m_pPointId;
			GUI::ComboLineCtrlC* m_pBoundary;
			GUI::ComboLineCtrlC* m_pResolutionHeigth;
			GUI::ComboLineCtrlC* m_pResolutionWidth;

			
			OBS_DECLARE_EXECUTE(PlaneScanDialog);

			PlaneScanModel* m_pDataModel;
            

	private:
			OBS::ObserverC m_pResolutionHeigthObserver;
			OBS::ObserverC m_pResolutionWidthObserver;
			OBS::ObserverC m_pFileObserver;
			OBS::ObserverC m_pPointIdObserver;

			virtual void OnValueChanged(int unNotifyCode, int ulParam2);

			PlaneScanController* pController;
	};

   class PlaneScanModel : public GUI::ModelC
    {
        public: 

            // Description: Constructor
            //
            PlaneScanModel(Model* pModel);
			//Alignment321Model(Alignment321Model* pModel);

            // Description: Destructor
            //
            virtual ~PlaneScanModel();

			short iResolutionHeigth;
			short iResolutionWidth;
			StringC sFileName;
			StringC sPointId;
			ScanFileFunc* m_pScanFile;
			S_PLANE_BUFF boundary_plane[1];
			//S_POINT_BUFF boundary_points[3];
			bool generate_points(Model *pModel);
			bool isRunning;
			///FileModel fileModel;

			
			S_SCAN_POINT_BUFF des_points[MAX_SCAN_POINTS];
			int current_point;
			unsigned int points_count;
			unsigned int total_points;
	private:	
			bool add_line_points(ScanFileFunc* m_pScanFile, struct line* line1, 
							short i, short pointCountWidth, double distWidth, double resolutionWidth, char* first_pointid,
							Model *pModel);

			
    };
	
};

#endif // DCP_BASEGUI_HPP




