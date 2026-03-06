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

#include <dcp06/core/Types.hpp>
#include <dcp06/file/AdfFileFunc.hpp>
#include <dcp06/file/AngleFile.hpp>

#include <dcp06/core/Common.hpp>
#include <dcp06/core/MsgBox.hpp>

#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>

// Description: 
//
namespace DCP
{
    // Forward declaration
	class Model;
	class CalculationAngleDialog;
	class CalculationAngleModel;


    // Description: Tabbed controller for the Hello World application
    class CalculationAngleController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            CalculationAngleController(Model* pModel);
			~CalculationAngleController();

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
            CalculationAngleController( const CalculationAngleController& oCalcAngleController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            CalculationAngleController& operator=( const CalculationAngleController& oCalcAngleController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            CalculationAngleDialog* m_pDlg;
			CalculationAngleModel * m_pDataModel;
			void change_function_keys();
			Model* m_pModel;
    };


	class CalculationAngleDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC, public OBS::CommandC
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

		   CalculationAngleDialog(CalculationAngleModel * pCalcAngleModel);

            // Description: Destructor
            virtual ~CalculationAngleDialog();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			Model* GetModel() const;

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
			OBS_DECLARE_EXECUTE(CalculationAngleDialog);

		private:
			CalculationAngleModel * m_pDataModel;
			OBS::ObserverC m_pAngleIdObserver;
			OBS::ObserverC m_pNoteObserver;
			OBS::ObserverC m_pRefIdObserver;
			OBS::ObserverC m_pTrgtIdObserver;
			virtual void OnValueChanged(int unNotifyCode, int ulParam2);
	};

	class CalculationAngleModel : public GUI::ModelC

    {
        public:

            // Description: Constructor
            //
            CalculationAngleModel(Model* pModel);
			//Alignment321Model(Alignment321Model* pModel);

            // Description: Destructor
            //
            virtual ~CalculationAngleModel();

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
			Common* pCommon;
			MsgBox* pMsgBox;
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




