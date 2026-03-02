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

#ifndef DCP_POMDLG_HPP
#define DCP_POMDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

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
    class DCP06ModelC;
	class DCP06PomDlgC;
	class DCP06PomModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP06PomControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06PomControllerC(DCP06ModelC* pDCP06Model);
			~DCP06PomControllerC();

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
            DCP06PomControllerC( const DCP06PomControllerC& oDCP06PomController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06PomControllerC& operator=( const DCP06PomControllerC& oDCP06PomController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06PomDlgC* m_pDlg;
			DCP06PomModelC* m_pDataModel;
			DCP06ModelC* m_pDCP06Model;

			short/*DCP_COORDINATE_SYSTEM*/ old_coordinate;
			//AdfFileFunc* adf;
			bool delete_pom();

    };


    class DCP06PomDlgC:public GUI::StandardDialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
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

			DCP06PomDlgC(DCP06PomModelC* pPomModel);

            // Description: Destructor
            virtual ~DCP06PomDlgC();

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
			
			void update_bft_adf();
			DCP06CommonC* m_pCommon;

	protected:
			
			GUI::TextCtrlC* m_pInfo1;
			GUI::ComboLineCtrlC* m_pFile;
			GUI::ComboLineCtrlC* m_pPoints;
			GUI::TextCtrlC* m_pInfo2;
			GUI::ComboLineCtrlC* m_pPointMeas;
			GUI::ComboLineCtrlC* m_pCalc;
			//OBS_DECLARE_EXECUTE(DCP06DomDlgC);

			DCP06PomModelC* m_pDataModel;
            

	private:
			//OBS::ObserverC m_pComboBoxObserver;
			//virtual void OnComboBoxChanged(int unNotifyCode, int ulParam2);
	};

   class DCP06PomModelC : public GUI::ModelC
    {
        public: 

            // Description: Constructor
            //
            DCP06PomModelC();
			//DCP06DomModelC(DCP06DomModelC* pModel);

            // Description: Destructor
            //
            virtual ~DCP06PomModelC();

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
			short pom_chst;
			bool ocs_defined;

			//short LAST_SEL;
			/*
			short dom_hz_plane; //muista asettaa
			DCP::DCP_PLANE_TYPE dom_plane_type;
			dom_rot_plane_buff
			*/

    };

};

#endif // DCP_BASEGUI_HPP




