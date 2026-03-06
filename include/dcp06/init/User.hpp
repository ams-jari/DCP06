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

#ifndef DCP_USERDLG_HPP
#define DCP_USERDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class Model;
	class UserDialog;


    // Description: Tabbed controller for the Hello World application
    class UserController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            UserController();
			~UserController();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            UserController( const UserController& oUserController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            UserController& operator=( const UserController& oUserController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            UserDialog* m_pDlg;
    };


	class UserDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eUser1=1,
				eUser2,
				eUser3,
				eUser4,
				eUser5,
				eUser6,
				eUser7,
				eUser8,
				eUser9,
				eUser10
			};

		   UserDialog();

            // Description: Destructor
            virtual ~UserDialog();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            Model* GetModel() const;

			// Description: add all controls
            
		

	protected:
			virtual void RefreshControls();
  			
			GUI::ComboLineCtrlC* m_pUser1;
			GUI::ComboLineCtrlC* m_pUser2;
			GUI::ComboLineCtrlC* m_pUser3;
			GUI::ComboLineCtrlC* m_pUser4;
			GUI::ComboLineCtrlC* m_pUser5;
			GUI::ComboLineCtrlC* m_pUser6;
			GUI::ComboLineCtrlC* m_pUser7;
			GUI::ComboLineCtrlC* m_pUser8;
			GUI::ComboLineCtrlC* m_pUser9;
			GUI::ComboLineCtrlC* m_pUser10;

	};
};

#endif // DCP_BASEGUI_HPP




