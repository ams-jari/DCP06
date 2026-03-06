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

#ifndef DCP_3DFBS_DLG_HPP
#define DCP_3DFBS_DLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/file/AdfFileFunc.hpp>

#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp> 
#include <GUI_AppBase.hpp>
#include <TBL_SurveyModel.hpp>
#include <TBL_Measurement.hpp>
//#include <GUI_Timer.hpp>


#include <cfa_Types.hpp>
#include <GSV_GeospatialViewDialog.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class DCP06ModelC;
	class DCP063DFbsDlgC;
	class DCP063DFBSMeasModelC;


    // Description: Tabbed controller for the Hello World application
    class DCP063DFbsControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP063DFbsControllerC(DCP06ModelC* pDCP06Model);
			~DCP063DFbsControllerC();

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

		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			virtual void OnControllerActivated(void);
			virtual bool ConfirmClose(bool bEsc);
			void show_function_keys();
			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP063DFbsControllerC( const DCP063DFbsControllerC& oDCP063DFbsController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP063DFbsControllerC& operator=( const DCP063DFbsControllerC& o3DFbsToolController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP063DFbsDlgC* m_pDlg;
			DCP063DFBSMeasModelC* m_pDataModel;
			void change_dsp();
			StringC sMsg;

			void ShowHiddenPointDlg();
			void ShowXorYorZDlg();
			void ShowCircleDlg();
			void ShowSelectPointDlg();
			void ShowHomePointsDlg();
			void ShowSeparateRecDlg();
			void ShowMidPointDlg();
			bool m_bCamera;
			GSV::GeospatialViewDialogC* poVideoDlg;

    };

	
	class DCP063DFbsDlgC:public GUI::DialogC, public GUI::ModelHandlerC, public OBS::CommandC
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
				eInfo
			};

		   DCP063DFbsDlgC(DCP063DFBSMeasModelC* pDataModel);

            // Description: Destructor
            virtual ~DCP063DFbsDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP06ModelC* GetDCP06Model() const;
			
	protected:
  			
			GUI::ComboLineCtrlC* m_pFile;
			GUI::ComboLineCtrlC* m_pPointId;
			GUI::ComboLineCtrlC* m_pXDsg;
			GUI::ComboLineCtrlC* m_pYDsg;
			GUI::ComboLineCtrlC* m_pZDsg;
			GUI::ComboLineCtrlC* m_pXAct;
			GUI::ComboLineCtrlC* m_pYAct;
			GUI::ComboLineCtrlC* m_pZAct;
			GUI::ComboLineCtrlC* m_pXDev;
			GUI::ComboLineCtrlC* m_pYDev;
			GUI::ComboLineCtrlC* m_pZDev;

			OBS_DECLARE_EXECUTE(DCP063DFbsDlgC);
	private:
		DCP063DFBSMeasModelC* m_pDataModel;
		OBS::ObserverC m_pPointIdObserver;
		OBS::ObserverC m_pXObserver;
		OBS::ObserverC m_pYObserver;
		OBS::ObserverC m_pZObserver;
		virtual void OnValueChanged( int unNotifyCode,  int ulParam2);
		//virtual void OnTimer(void);
		//GUI::TimerC m_pTimer;
		short iInfoInd;
		StringC strInfoText;
		DCP06CommonC* m_pCommon;
			StringC sFront;
			StringC sBack;
			StringC sSingle;
			StringC sFrontBack;
	};
	 class DCP063DFBSMeasModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP063DFBSMeasModelC(DCP06ModelC* pDCP06Model);

            // Description: Destructor
            //
            virtual ~DCP063DFBSMeasModelC();
			
			AdfFileFunc* pFileFunc;
			DCP06CommonC* pCommon;
			DCP06MsgBoxC* pMsgBox;
			char *xmea_ptr,*ymea_ptr,*zmea_ptr;
			char *xdes_ptr,*ydes_ptr,*zdes_ptr;
			char *pid_ptr,*note_ptr;

			char *xmea_f_ptr, *ymea_f_ptr, *zmea_f_ptr ;
			char *xmea_b_ptr, *ymea_b_ptr, *zmea_b_ptr ;
			char *xdes_f_ptr, *ydes_f_ptr, *zdes_f_ptr ;
			char *xdes_b_ptr, *ydes_b_ptr, *zdes_b_ptr ;



			short DSP_MODE;
			short FRONTBACK_ACTUAL;
			short iPointKeys;
			short file_updated;
			double x_temp, z_temp, y_temp;


			void set_xyz_mea_ptr();
			void set_xyz_des_ptr();
			void save_point();

			char bFid[ 100 ] ;
			char bPid[ 100 ] ;
			char bXmea[ 100 ] ;
			char bYmea[ 100 ] ;
			char bZmea[ 100 ] ;

			char bXdes[ 100 ] ;
			char bYdes[ 100 ] ;
			char bZdes[ 100 ] ;

			char bXdif[ 100 ] ;
			char bYdif[ 100 ] ;
			char bZdif[ 100 ] ;

			char bXmea_f[ 100 ] ;
			char bYmea_f[ 100 ] ;
			char bZmea_f[ 100 ] ;
			char bXdes_f[ 100 ] ;
			char bYdes_f[ 100 ] ;
			char bZdes_f[ 100 ] ;

			char bXmea_b[ 100 ] ;
			char bYmea_b[ 100 ] ;
			char bZmea_b[ 100 ] ;
			char bXdes_b[ 100 ] ;
			char bYdes_b[ 100 ] ;
			char bZdes_b[ 100 ] ;


			char bInfo1[ 100 ] ;
			char bInfo2[ 100 ] ;
			char bNote[15];

			double Xdes;
			double Ydes;
			double Zdes;
    };
};

#endif // DCP_BASEGUI_HPP




