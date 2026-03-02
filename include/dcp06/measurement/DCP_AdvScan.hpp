// ====================================================================================================================
//
// Project  : Viva
//
// Component: Hello World Scanning
//
// Summary  : Controller
//
// --------------------------------------------------------------------------------------------------------------------
//
// Copyright 2013 by Leica Geosystems AG, Heerbrugg
//
// ====================================================================================================================

#ifndef DCP_ADVSCANDLG_HPP
#define DCP_ADVSCANDLG_HPP

// ====================================================================================================================
// ===================================================  Includes   ====================================================
// ====================================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <GUI_Application.hpp>
//#include <HEW_ScanningTypes.hpp>
//#include <HEW_ScanningModel.hpp>
#include <OBS_Observer.hpp>
#include <GUI_GController.hpp>
#include <TBL_Measurement.hpp>
#include <SDI_Scan.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_ComboLineCtrl.hpp>

#include <dcp06/core/DCP_Common.hpp>
// ====================================================================================================================
// ==================================================  Declaration   ==================================================
// ====================================================================================================================

// Description: Survey application
namespace DCP
{
	class DCPScanningModelC;

	  // Description: Scanning dialog for predefined scans
    class DCPScanningDialogC : public GUI::StandardDialogC
    {
        public:
            // Description: Constructor
           DCPScanningDialogC(DCPScanningModelC* poScanningModel);

            // Description: Destructor
            virtual ~DCPScanningDialogC();

        // Description: Called by the framework when the dialog is activated
        virtual void OnDialogActivated(void);

		// Description: Refresh controls
        virtual void RefreshControls();

        private:
            // Description: Combobox with all available scans
            GUI::ComboLineCtrlC* m_poAvailableScan;

			DCPScanningModelC* m_poScanningModel;
    };

    // Description: Survey main controller
    class DCPScanningControllerC : public GUI::GControllerC, public TBL::MeasurementC,public OBS::CommandC
    {
        public:

            // Description: Dialogs used in Survey Controller
            enum SurveyDialogsT
            {
                CTL_MeasStart = 1,  // controller for the start measurement procedure
                DLG_Scanning,       // tabbed scanning dialog
            };

            // Description: Constructor
            DCPScanningControllerC(DCP06ModelC* pDCP06Model);

            // Description: Destructor
            virtual ~DCPScanningControllerC(void);

            // Description: Define function keys
            void SetFunctionKeys();

            // Description: this function is called whenever an event is raised by GUI Control
            virtual void OnUserEvent(int unParam1, int ulId);

            // Description: Add predefined scan definition
            //virtual void OnF1Pressed(void);

            // Description: Read SDB
            //virtual void OnF2Pressed(void);

            // Description: Start scan definition controller
            virtual void OnF1Pressed(void);

            // Description: Start scan configuration controller
            virtual void OnF2Pressed(void);

            // Description: Start scan configuration controller
            virtual void OnF3Pressed(void);

            // Description: Start scan status controller
            virtual void OnF5Pressed(void);

			// Description: Start scan status controller
            virtual void OnF6Pressed(void);

            // Description: Start auto scan status controller
           // virtual void OnSHF5Pressed(void);

			//virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			//virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );

			//virtual bool SetModel( GUI::ModelC* pModel );


        private:
			void ReadDb();
			//
			//
			OBS_DECLARE_EXECUTE(DCPScanningControllerC);
			OBS::ObserverC m_pOnApplicationClosedObserver;
			virtual void OnApplicationClosed( int unNewApp, int ulAppOwner);
			// Description: vector of all scans names (id is the position within the vector)
            //std::vector<StringC> m_vecScanList;

            // Description: Get Description
            //StringC GetDescription(HEW::ScanningModelC::PredefinedScanT eScanType);

            // Description: Create new Scan
            //SDI::SmartPtrScanC CreateScan(HEW::ScanningModelC::PredefinedScanT eScanType);

            // Description: Create the different scan types
            //void CreateRectangleScan(SDI::SmartPtrScanC spScan);
            //void CreatePolygonScan(SDI::SmartPtrScanC spScan);
            //void CreatePointListScan(SDI::SmartPtrScanC spScan);
            //void CreateHorizontalScan(SDI::SmartPtrScanC spScan, double dVertical );

            // Description: Copy a scan
            SDI::SmartPtrScanC CreateScanCopy(SDI::SmartPtrScanC spScan);

            // Description: Update available scans (update model)
            void UpdateAvailableScans();

            // Description: Model
            DCPScanningModelC* m_poScanningModel;
			DCPScanningDialogC* m_poDCPScanningDialogC;

            // Description: Smartpoint to actual scan;
            SDI::SmartPtrScanC m_spScan;

            // Description: survey model, used for TBL::MeasurementC
            TBL::SurveyModelC* m_poSurveyModel;

			DCP06ModelC* m_pDCP06Model;
			DCP06CommonC* m_pCommon;
            //double m_dHorizScanV;

    }; // end of SurveyControllerC


	 class DCPScanningModelC : public GUI::GModelC
    {
        public:
            // Description: model data items
            enum ModelItemsT
            {
                MI_PREDEF_SCAN = 1,
                MI_AVAILABLE_SCAN,
                MI_SCAN_DESCIPTION,
                MI_SCAN_NUMPOINTS,
                MI_SCAN_TIME,
				MI_SCAN_STATUS
            };

            enum PredefinedScanT
            {
                PS_40X40_RECT = 1,
                PS_TRIANGLE_POLY,
                PS_100_POINTS,
                PS_HZ_V45,
                PS_HZ_V90,
                PS_HZ_V135,
            };

            // Description: Constructor
           DCPScanningModelC();

            // Description: Destructor
            virtual ~DCPScanningModelC(void);

            // Description: Set default values
            void SetDefault();

            // Description: vector of all scans names (id is the position within the vector)
            std::vector<StringC> m_vecScanList;

    };

} // namespace DCP

#endif // HEW_SCANNINGCONTROLLER_HPP
