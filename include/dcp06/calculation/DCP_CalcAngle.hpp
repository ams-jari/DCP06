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

#ifndef DCP_CALCANGLEDLG_HPP
#define DCP_CALCANGLEDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/file/DCP_AdfFileFunc.hpp>
#include <dcp06/file/DCP_AngleFile.hpp>

#include <dcp06/core/DCP_Common.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>

#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>

// Description: 
//
namespace DCP
{
    // Forward declaration
	class DCP05ModelC;
	class DCP05CalcAngleDlgC;
	class DCP05CalcAngleModelC;


    // Description: Tabbed controller for the Hello World application
    class DCP05CalcAngleControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05CalcAngleControllerC(DCP05ModelC* pDCP05Model);
			~DCP05CalcAngleControllerC();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			virtual void OnF2Pressed();
			virtual void OnF3Pressed();
			virtual void OnF4Pressed();
			virtual void OnF5Pressed();
			virtual void OnF6Pressed();
			//virtual void OnSHF1Pressed();
			virtual void OnSHF2Pressed();
			//virtual void OnSHF3Pressed();
			//virtual void OnSHF4Pressed();
			virtual void OnSHF5Pressed();
			//virtual void OnSHF6Pressed();
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			
			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP05CalcAngleControllerC( const DCP05CalcAngleControllerC& oDCP05CalcAngleController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05CalcAngleControllerC& operator=( const DCP05CalcAngleControllerC& oDCP05CalcAngleController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05CalcAngleDlgC* m_pDlg;
			DCP05CalcAngleModelC * m_pDataModel;
			void change_function_keys();
			DCP05ModelC* m_pDCP05Model;
    };


	class DCP05CalcAngleDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC, public OBS::CommandC
	{
		 public:

			enum eCtrlId
			{
				e3DFile=1,
				eAngleFile,
				eRefType,
				eTargetType,
				eRefId,
				eTargetId,
				eAngleId,
				eAngle,
				eNote	

			};

		   DCP05CalcAngleDlgC(DCP05CalcAngleModelC * pCalcAngleModel);

            // Description: Destructor
            virtual ~DCP05CalcAngleDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP05ModelC* GetDCP05Model() const;

	protected:
			GUI::ComboLineCtrlC* m_p3DFile;
			GUI::ComboLineCtrlC* m_pAngleFile;
			GUI::ComboLineCtrlC* m_pRefId;
			GUI::ComboLineCtrlC* m_pTargetId;
			GUI::ComboLineCtrlC* m_pAngleId;
			GUI::ComboLineCtrlC* m_pAngle;
			GUI::ComboLineCtrlC* m_pNote;
			GUI::ComboLineCtrlC* m_pRefType;
			GUI::ComboLineCtrlC* m_pTargetType;
			OBS_DECLARE_EXECUTE(DCP05CalcAngleDlgC);

		private:
			DCP05CalcAngleModelC * m_pDataModel;
			OBS::ObserverC m_pAngleIdObserver;
			OBS::ObserverC m_pNoteObserver;
			OBS::ObserverC m_pRefIdObserver;
			OBS::ObserverC m_pTrgtIdObserver;
			virtual void OnValueChanged(int unNotifyCode, int ulParam2);
	};

	class DCP05CalcAngleModelC : public GUI::ModelC

    {
        public:

            // Description: Constructor
            //
            DCP05CalcAngleModelC(DCP05ModelC* pDCP05Model);
			//DCP05DomModelC(DCP05DomModelC* pModel);

            // Description: Destructor
            //
            virtual ~DCP05CalcAngleModelC();

			enum eDisplayMode
			{
				eNormal = 0,
				eRef,
				eTarget
			};
	
			eDisplayMode dspMode;
			short active_reftype;
			short active_targettype;
			S_PLANE_BUFF rplane_buff[1];
			S_LINE_BUFF	rline_buff[1];
			S_POINT_BUFF refpoint;
			S_POINT_BUFF trgtpoint;
			S_LINE_BUFF trgt_line_buff[1];
			S_PLANE_BUFF trgt_plane_buff[1];

			StringC sSelected3DFile;
			StringC sSelectedAngleFile;
			AdfFileFunc* pAdfFileFunc;
			AgfFileFunc* pAgfFileFunc;
			DCP05CommonC* pCommon;
			DCP05MsgBoxC* pMsgBox;
			short iPointCount3dfile;
			S_SELECT_POINTS point_list[MAX_POINTS_IN_FILE];
			short REF_ACTDES;
			short TRG_ACTDES;
			char cAid[10];
			char cNote[15];
			char cAngle[15];
			char cRefId[15];
			char cRefType[15];
			char cTrgtId[15];
			char cTrgtType[15];

			double dCalculatedAngle;
			short iAngleCalculated ;
			short all_defined();
			short is_active_target_defined();
			short is_active_reference_defined();
			void save();
			short calc_angle_line_point();
			short calc_angle_line_line();
			short calc_angle_plane_plane();
			short calc_angle();
			void clear_buffers();

			StringC sActualSelected;
			StringC sDesignSelected;
			StringC sActualNonSelected;
			StringC sDesignNonSelected;
    };
	
};

#endif // DCP_BASEGUI_HPP




