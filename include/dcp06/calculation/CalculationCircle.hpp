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

#ifndef DCP_CALCCIRCLEDLG_HPP
#define DCP_CALCCIRCLEDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/file/AdfFileFunc.hpp>
#include <dcp06/file/CircleFile.hpp>
#include <dcp06/measurement/Circle.hpp>
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
	class CalculationCircleDialog;
	class CalculationCircleModel;


    // Description: Tabbed controller for the Hello World application
    class CalculationCircleController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            CalculationCircleController(Model *pModel);
			~CalculationCircleController();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			//virtual void OnF2Pressed();
			virtual void OnF3Pressed();
			//virtual void OnF4Pressed();
			//virtual void OnF5Pressed();
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
            CalculationCircleController( const CalculationCircleController& oCalculationCircleController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            CalculationCircleController& operator=( const CalculationCircleController& oCalculationCircleController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            CalculationCircleDialog* m_pDlg;
			CalculationCircleModel * m_pDataModel;
			Model* m_pModel;
    };


	class CalculationCircleDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC //, public OBS::CommandC
	{
		 public:

			enum eCtrlId
			{
				e3DFile=1,
				//eCircleFile,
				//eCircleId,
				eDiameter,
				eRadius,
				eRMS,
				eCx,
				eCy,
				eCz
			};

		   CalculationCircleDialog(CalculationCircleModel * pCircleModel);

            // Description: Destructor
            virtual ~CalculationCircleDialog();

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
			//GUI::ComboLineCtrlC* m_pCircleFile;
			//GUI::ComboLineCtrlC* m_pCircleId;
			GUI::ComboLineCtrlC* m_pDiameter;
			GUI::ComboLineCtrlC* m_pRadius;
			GUI::ComboLineCtrlC* m_pRMS;
			GUI::ComboLineCtrlC* m_pCx;
			GUI::ComboLineCtrlC* m_pCy;
			GUI::ComboLineCtrlC* m_pCz;

			//OBS_DECLARE_EXECUTE(CalculationCircleDialog);

		private:
			CalculationCircleModel *m_pDataModel;
			//OBS::ObserverC m_pCircleIdObserver;
			//virtual void OnValueChanged(int unNotifyCode, int ulParam2);
	};

	 class CalculationCircleModel : public GUI::ModelC
     {
        public:

            // Description: Constructor
            CalculationCircleModel(Model* pModel);

            // Description: Destructor
            virtual ~CalculationCircleModel();

			CircleModel *pCircleModel;
			StringC sSelected3DFile;
			StringC sSelectedCircleFile;
			AdfFileFunc* pAdfFileFunc;
			short iPointCount3dfile;
			S_SELECT_POINTS point_list[MAX_POINTS_IN_FILE];

    };
	
};

#endif // DCP_BASEGUI_HPP




