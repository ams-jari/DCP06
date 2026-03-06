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

#ifndef DCP_3DMEAS_DLG_HPP
#define DCP_3DMEAS_DLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/file/AdfFileFunc.hpp>
#include <dcp06/core/MsgBox.hpp>

#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
//#include <GUI_Timer.hpp>
#include <GUI_HourGlass.hpp>
#include <TBL_SurveyModel.hpp>
#include <TBL_Measurement.hpp>

#include <cfa_Types.hpp>
#include <GSV_GeospatialViewDialog.hpp>

// Description: 
//
namespace DCP
{
    // Forward declaration
	class DCP06ModelC;
	class DCP063DMeasDlgC;
	class DCP063DMeasModelC;


    // Description: Tabbed controller for the Hello World application
    class DCP063DMeasControllerC : public GUI::ControllerC, public TBL::MeasurementC
    {
        public:

            // Description: Constructor
            DCP063DMeasControllerC(bool bShaf, DCP06ModelC* pDCP06Model);
			~DCP063DMeasControllerC();


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
			virtual bool ConfirmClose(bool bEsc);
		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			virtual void OnControllerActivated(void);
			//virtual void OnControllerClosed(int lExitCode);


			void show_function_keys();
			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP063DMeasControllerC( const DCP063DMeasControllerC& oDCP063DMeasController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP063DMeasControllerC& operator=( const DCP063DMeasControllerC& o3DMeasToolController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP063DMeasDlgC* m_pDlg;
			DCP063DMeasModelC* m_pDataModel;
			DCP06MsgBoxC* msgbox;
			DCP06CommonC* m_pCommon;
			StringC msg;
			GSV::GeospatialViewDialogC* poVideoDlg;

			bool m_bPointMenu;

			void ShowHiddenPointDlg();
			void ShowXorYorZDlg();
			void ShowCircleDlg();
			void ShowSelectPointDlg();
			void ShowHomePointsDlg();
			void ShowSeparateRecDlg();
			void ShowMidPointDlg();
			void ShowToolDlg();

			bool m_bShaft;
			bool m_bCamera;
	};


	class DCP063DMeasDlgC:public GUI::/*Standard*/DialogC, public GUI::ModelHandlerC, public OBS::CommandC
	{
		 public:

			enum eCtrlId
			{
				eFile=1,
				ePointId,
				eXDsg,
				eYDsg,
				eZDsg,
				eXAct,
				eYAct,
				eZAct,
				eXDev,
				eYDev,
				eZDev,

				/*
				eXActDev,
				eYActDev,
				eZActDev,
				*/
				eInfo
			};

			DCP063DMeasDlgC(DCP063DMeasModelC* pMeasModel);

            // Description: Destructor
            virtual ~DCP063DMeasDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			//virtual void OnTimer(void);
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP06ModelC* GetDCP06Model() const;
			
	protected:
  			
			
			GUI::ComboLineCtrlC* m_pFile;
			GUI::ComboLineCtrlC* m_pPointId;
			GUI::ComboLineCtrlC* m_pXDsg;
			GUI::ComboLineCtrlC* m_pYDsg;
			GUI::ComboLineCtrlC* m_pZDsg;
			/*
			GUI::ComboLineCtrlC* m_pXActDev;
			GUI::ComboLineCtrlC* m_pYActDev;
			GUI::ComboLineCtrlC* m_pZActDev;
			*/
			//GUI::TextCtrlC* m_pInfo;

			/*
			GUI::MultiTextValueCtrlC* m_pXActDev;
			GUI::MultiTextValueCtrlC* m_pYActDev;
			GUI::MultiTextValueCtrlC* m_pZActDev;
			*/
			GUI::ComboLineCtrlC* m_pXAct;
			GUI::ComboLineCtrlC* m_pYAct;
			GUI::ComboLineCtrlC* m_pZAct;
			GUI::ComboLineCtrlC* m_pXDev;
			GUI::ComboLineCtrlC* m_pYDev;
			GUI::ComboLineCtrlC* m_pZDev;

			OBS_DECLARE_EXECUTE(DCP063DMeasDlgC);

	private:
		DCP06CommonC* m_pCommon;
		DCP063DMeasModelC* m_pDataModel;

		OBS::ObserverC m_pPointIdObserver;
		OBS::ObserverC m_pXObserver;
		OBS::ObserverC m_pYObserver;
		OBS::ObserverC m_pZObserver;
		virtual void OnValueChanged(int unNotifyCode,  int ulParam2);
		//virtual void OnTimer(void);
		//GUI::TimerC m_pTimer;
			
		short iInfoInd;
		StringC strInfoText;
		
	};

   class DCP063DMeasModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP063DMeasModelC(DCP06ModelC* pDCP06Model);

            // Description: Destructor
            //
            virtual ~DCP063DMeasModelC();
			
			void set_dcp05_model(DCP06ModelC *pModel);

			AdfFileFunc* m_pFileFunc;

			char *xmea_ptr,*ymea_ptr,*zmea_ptr;
			char *xdes_ptr,*ydes_ptr,*zdes_ptr;
			char *pid_ptr,*note_ptr;

			short DSP_MODE;
			short file_updated;
			double x_temp, z_temp, y_temp;


			void set_xyz_mea_ptr();
			void set_xyz_des_ptr();
			void save_point();

			char bFid[ 100 ] ;
			char bPid[ 100 ] ;
			char bXmea[ 100 ] ;
			char  bYmea[ 100 ] ;
			char bZmea[ 100 ] ;

			char bXdes[ 100 ] ;
			char bYdes[ 100 ] ;
			char bZdes[ 100 ] ;

			char bXdif[ 100 ] ;
			char bYdif[ 100 ] ;
			char bZdif[ 100 ] ;

			char bInfo1[ 100 ] ;
			char bInfo2[ 100 ] ;
			char bNote[15];

			double Xdes;
			double Ydes;
			double Zdes;

	private:
			DCP06ModelC *m_pDCP06Model;
			DCP06CommonC *m_pCommon;
    };

};

#endif // DCP_BASEGUI_HPP




