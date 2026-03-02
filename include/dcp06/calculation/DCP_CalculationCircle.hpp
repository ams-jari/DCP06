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

#ifndef DCP_CALCCIRCLEDLG_HPP
#define DCP_CALCCIRCLEDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/file/DCP_AdfFileFunc.hpp>
#include <dcp06/file/DCP_CircleFile.hpp>
#include <dcp06/measurement/DCP_Circle.hpp>
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
	class DCP06ModelC;
	class DCP06CalculationCircleDlgC;
	class DCP06CalculationCircleModelC;


    // Description: Tabbed controller for the Hello World application
    class DCP06CalculationCircleControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06CalculationCircleControllerC(DCP06ModelC *pDCP06Model);
			~DCP06CalculationCircleControllerC();

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
            DCP06CalculationCircleControllerC( const DCP06CalculationCircleControllerC& oDCP06CalculationCircleController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06CalculationCircleControllerC& operator=( const DCP06CalculationCircleControllerC& oDCP06CalculationCircleController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06CalculationCircleDlgC* m_pDlg;
			DCP06CalculationCircleModelC * m_pDataModel;
			DCP06ModelC* m_pDCP06Model;
    };


	class DCP06CalculationCircleDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC //, public OBS::CommandC
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

		   DCP06CalculationCircleDlgC(DCP06CalculationCircleModelC * pCircleModel);

            // Description: Destructor
            virtual ~DCP06CalculationCircleDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP06ModelC* GetDCP06Model() const;

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

			//OBS_DECLARE_EXECUTE(DCP06CalculationCircleDlgC);

		private:
			DCP06CalculationCircleModelC *m_pDataModel;
			//OBS::ObserverC m_pCircleIdObserver;
			//virtual void OnValueChanged(int unNotifyCode, int ulParam2);
	};

	 class DCP06CalculationCircleModelC : public GUI::ModelC
     {
        public:

            // Description: Constructor
            DCP06CalculationCircleModelC(DCP06ModelC* pDCP06Model);

            // Description: Destructor
            virtual ~DCP06CalculationCircleModelC();

			DCP06CircleModelC *pCircleModel;
			StringC sSelected3DFile;
			StringC sSelectedCircleFile;
			AdfFileFunc* pAdfFileFunc;
			short iPointCount3dfile;
			S_SELECT_POINTS point_list[MAX_POINTS_IN_FILE];

    };
	
};

#endif // DCP_BASEGUI_HPP




