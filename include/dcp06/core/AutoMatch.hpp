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

#ifndef DCP_AUTOMATCHDLG_HPP
#define DCP_AUTOMATCHDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
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
	class AutoMatchDialog;
	class AutoMatchModel;

    // Description: Tabbed controller for the Hello World application
    class AutoMatchController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            AutoMatchController(Model *pModel);
			~AutoMatchController();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			//virtual void OnF2Pressed();
			//virtual void OnF3Pressed();
			//virtual void OnF4Pressed();
			//virtual void OnF5Pressed();
			virtual void OnF5Pressed();
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
            AutoMatchController( const AutoMatchController& oAutoMatchController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            AutoMatchController& operator=( const AutoMatchController& oAutoMatchController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            AutoMatchDialog* m_pDlg;
			Model* m_pModel;
    };


	class AutoMatchDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eLine1=1,
				eLine2,
				eX,
				eY,
				eZ
			};

			AutoMatchDialog(DCP::Model* pModel);

            // Description: Destructor
            virtual ~AutoMatchDialog();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP::AutoMatchModel* GetDataModel() const;

			
	protected:
  			
			GUI::TextCtrlC* m_pLine1;
			GUI::TextCtrlC* m_pLine2;
			GUI::TextCtrlC* m_pX;
			GUI::TextCtrlC* m_pY;
			GUI::TextCtrlC* m_pZ;
	private:
			
			Model* m_pModel;
	};

	class AutoMatchModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            AutoMatchModel();

            // Description: Destructor
            //
            virtual ~AutoMatchModel();
			double xmea, ymea, zmea;
			double xdsg, ydsg, zdsg;
			StringC pointid;
			short pno;
	 };


};

#endif // DCP_BASEGUI_HPP




