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

#ifndef DCP_CALCDISTDLG_HPP
#define DCP_CALCDISTDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/file/AdfFileFunc.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/file/DistFile.hpp>


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
	class CalculationDistDialog;
	class CalculationDistModel;


    // Description: Tabbed controller for the Hello World application
    class CalculationDistController : public GUI::ControllerC
    {
        public:
		
            // Description: Constructor
            CalculationDistController(Model* pModel);
			~CalculationDistController();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			virtual void OnF2Pressed();
			virtual void OnF3Pressed();
			virtual void OnF4Pressed();
			virtual void OnF5Pressed();
			virtual void OnF6Pressed();
			//virtual void OnF6Pressed();
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
            CalculationDistController( const CalculationDistController& oCalcDistController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            CalculationDistController& operator=( const CalculationDistController& oCalcDistController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            CalculationDistDialog* m_pDlg;
			CalculationDistModel * m_pDataModel;
			void change_function_keys();
			Model* m_pModel;

	private:
			
    };


	class CalculationDistDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC, public OBS::CommandC
	{
		 public:

			enum eCtrlId
			{
				e3DFile=1,
				eDistFile,
				eRefType,
				eTargetType,
				eRefId,
				eTargetId,
				eDistId,
				eDist,
				eNote	
			};

		   CalculationDistDialog(CalculationDistModel * pCalcDistModel);

            // Description: Destructor
            virtual ~CalculationDistDialog();

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
			GUI::ComboLineCtrlC* m_pDistFile;
			GUI::ComboLineCtrlC* m_pRefId;
			GUI::ComboLineCtrlC* m_pTargetId;
			GUI::ComboLineCtrlC* m_pDistId;
			GUI::ComboLineCtrlC* m_pDist;
			GUI::ComboLineCtrlC* m_pNote;
			GUI::ComboLineCtrlC* m_pRefType;
			GUI::ComboLineCtrlC* m_pTargetType;

			OBS_DECLARE_EXECUTE(CalculationDistDialog);

			/*
			GUI::ComboLineCtrlC* m_pPointNo;
			GUI::ComboLineCtrlC* m_pPointId;
			GUI::ComboLineCtrlC* m_pX;
			GUI::ComboLineCtrlC* m_pY;
			GUI::ComboLineCtrlC* m_pZ;
			*/
	private:
		CalculationDistModel * m_pDataModel;
		OBS::ObserverC m_pDistIdObserver;
		OBS::ObserverC m_pNoteObserver;
		OBS::ObserverC m_pRefIdObserver;
		OBS::ObserverC m_pTrgtIdObserver;
		virtual void OnValueChanged(int unNotifyCode, int ulParam2);
	};

	 class CalculationDistModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            CalculationDistModel(Model* pModel);
			//Alignment321Model(Alignment321Model* pModel);

            // Description: Destructor
            //
            virtual ~CalculationDistModel();

			enum eDisplayMode
			{
				eNormal = 0,
				eRef,
				eTarget
			};
	
			eDisplayMode dspMode;
			short active_reftype;
			S_PLANE_BUFF rplane_buff[1];
			S_LINE_BUFF	rline_buff[1];
			S_POINT_BUFF refpoint;
			S_POINT_BUFF trgtpoint;
			AdfFileFunc* pAdfFileFunc;
			CdfFileFunc* pCdfFileFunc;
			StringC sSelected3DFile;
			StringC sSelectedDistFile;
			S_SELECT_POINTS point_list[MAX_POINTS_IN_FILE];
			short iPointCount3dfile;
			StringC sActualSelected;
			StringC sDesignSelected;
			StringC sActualNonSelected;
			StringC sDesignNonSelected;

			Common* pCommon;
			MsgBox* pMsgBox;
			double dCalculatedDist;
			short iDistanceCalculated;
			short all_defined();
			void clear_buffers();
			void save();
			short REF_ACTDES;
			short TRG_ACTDES;
			char cDid[15];
			char cNote[15];
			char cDist[15];
			char cRefId[15];
			char cRefType[15];
			char cTrgtId[15];
			char cTrgtType[15];
	private:
			
			short calc_distance();
			short is_active_target_defined();
			short is_active_reference_defined();
			
    };
	
};

#endif // DCP_BASEGUI_HPP




