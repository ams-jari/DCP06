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

#ifndef DCP_POMDLG_HPP
#define DCP_POMDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Common.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>

#include <GUI_AppBase.hpp>

#include <dcp06/file/AdfFileFunc.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class Model;
	class BestFitDialog;
	class BestFitModel;

    // Description: Tabbed controller for the Hello World application
    class BestFitController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            BestFitController(Model* pModel);
			~BestFitController();

            // Description: Handle change of position values
           	virtual void OnF1Pressed(void);
			virtual void OnF2Pressed(void);
			virtual void OnF3Pressed(void);
			virtual void OnF4Pressed(void);
			virtual void OnF6Pressed(void);
			virtual void OnSHF2Pressed(void);
			virtual void OnSHF3Pressed(void);
			virtual void OnSHF4Pressed(void);

			
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int /*lExitCode*/ );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            BestFitController( const BestFitController& oBestFitController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            BestFitController& operator=( const BestFitController& oBestFitController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            BestFitDialog* m_pDlg;
			BestFitModel* m_pDataModel;
			Model* m_pModel;

			short/*DCP_COORDINATE_SYSTEM*/ old_coordinate;
			//AdfFileFunc* adf;
			bool delete_bestFit();

    };


    class BestFitDialog:public GUI::StandardDialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eInfo1=1,
				eFile,
				ePoints, 
				eInfo2,
				ePointMeas,
				eCalc
			};

			BestFitDialog(BestFitModel* pPomModel);

            // Description: Destructor
            virtual ~BestFitDialog();

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
			
			void update_bft_adf();
			Common* m_pCommon;

	protected:
			
			GUI::TextCtrlC* m_pInfo1;
			GUI::ComboLineCtrlC* m_pFile;
			GUI::ComboLineCtrlC* m_pPoints;
			GUI::TextCtrlC* m_pInfo2;
			GUI::ComboLineCtrlC* m_pPointMeas;
			GUI::ComboLineCtrlC* m_pCalc;
			//OBS_DECLARE_EXECUTE(Alignment321Dialog);

			BestFitModel* m_pDataModel;
            

	private:
			//OBS::ObserverC m_pComboBoxObserver;
			//virtual void OnComboBoxChanged(int unNotifyCode, int ulParam2);
	};

   class BestFitModel : public GUI::ModelC
    {
        public: 

            // Description: Constructor
            //
            BestFitModel();
			//Alignment321Model(Alignment321Model* pModel);

            // Description: Destructor
            //
            virtual ~BestFitModel();

			S_POINT_BUFF	point_DCS[MAX_BESTFIT_POINTS];
			S_POINT_BUFF	point_OCS[MAX_BESTFIT_POINTS];
			S_POINT_BUFF	point_RES[MAX_BESTFIT_POINTS];

			short INTO_TEMPLATE,INTO_CAPTURE;

			double matrix[4][4];
			double inv_matrix[4][4];
			bool calculated;
			//bool ocsd_defined;
			short /*DCP_COORDINATE_SYSTEM*/ old_active_coodinate_system;
			double rms_x;
			double rms_y;
			double rms_z;
			short bestFit_chst;
			bool ocs_defined;

			//short LAST_SEL;
			/*
			short align321_hz_plane; //muista asettaa
			DCP::DCP_PLANE_TYPE align321_plane_type;
			align321_rot_plane_buff
			*/

    };

};

#endif // DCP_BASEGUI_HPP




