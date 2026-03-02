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

#ifndef DCP_ADDTOOLDLG_HPP
#define DCP_ADDTOOLDLG_HPP


// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
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
	class DCP06AddToolDlgC;
	class DCP06AddToolModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP06AddToolControllerC : public GUI::ControllerC,public OBS::CommandC
    {
        public:

            // Description: Constructor
            DCP06AddToolControllerC(DCP06ModelC *pDCP06Model);
			~DCP06AddToolControllerC();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			//virtual void OnF2Pressed();
			//virtual void OnF3Pressed();
			//virtual void OnF4Pressed();
			//virtual void OnF5Pressed();
			virtual void OnF6Pressed();
			//virtual void OnSHF1Pressed();
			//virtual void OnSHF2Pressed();
			//virtual void OnSHF3Pressed();
			//virtual void OnSHF4Pressed();
			//virtual void OnSHF5Pressed();
			//virtual void OnSHF6Pressed();
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			
			
			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
           /* DCP06AddToolControllerC( const DCP06AddToolControllerC& oDCP06AddToolController )
            {
                USER_APP_VERIFY( false );
            }*/

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06AddToolControllerC& operator=( const DCP06AddToolControllerC& oDCP06AddToolController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06AddToolDlgC* m_pDlg;
			DCP::DCP06ModelC * m_pDCP06Model;

			OBS_DECLARE_EXECUTE(DCP06AddToolControllerC);
			OBS::ObserverC m_pOnApplicationClosedObserver;
			virtual void OnApplicationClosed(int unNewApp,  int ulAppOwner);
    };


	class DCP06AddToolDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eToolId=1,
				eTargetType,
				eConstant,
				eToolX,
				eToolY,
				eToolZ
			};

		   DCP06AddToolDlgC(DCP06ModelC *pDCP06Model);

            // Description: Destructor
            virtual ~DCP06AddToolDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP::DCP06AddToolModelC* GetDataModel() const;

	protected:
  			
			GUI::ComboLineCtrlC* m_pToolId;
			GUI::ComboLineCtrlC* m_pTargetType;
			GUI::ComboLineCtrlC* m_pConstant;
			GUI::ComboLineCtrlC* m_pToolX;
			GUI::ComboLineCtrlC* m_pToolY;
			GUI::ComboLineCtrlC* m_pToolZ;
	private:
			DCP::DCP06ModelC * m_pDCP06Model;
	};

   class DCP06AddToolModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            DCP06AddToolModelC();

			// Description: Destructor
			~DCP06AddToolModelC();
            
			S_TOOL tool;
			/*
            char  sName[11];
			short iPrismOrTape; // 0=Tape 1=prism
			double dConstant;
			double dX;
			double dY;
			double dZ;
            */
	
    };

};

#endif // DCP_BASEGUI_HPP




