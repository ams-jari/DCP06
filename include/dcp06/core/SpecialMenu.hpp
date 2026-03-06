// ====================================================================================================================
//
// Project  : Pluto/Venus Onboard Applications SW
//
// Component: 
//
// $Workfile: HEW_Application.hpp $
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

#ifndef DCP_SPECIALMENU_HPP
#define DCP_SPECIALMENU_HPP

// ================================================================================================
// ===========================================  Includes   ========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <UTL_Macro.hpp>
#include <GUI_Application.hpp>
#include <GUI_GraphMenuDialog.hpp>
#include <GUI_TableDialog.hpp>
#include <GUI_ListMultiColCtrl.hpp>

// Description: The Hello World application
//
namespace DCP
{
   // Forward declaration
	class Model;
	class SpecialMenuDialog;


    // Description: Tabbed controller for the Hello World application
    class SpecialMenuController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            SpecialMenuController(Model* pModel, int showItems = 0);
			~SpecialMenuController();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			
            // Description: Route model to everybody else
            //virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            SpecialMenuController( const SpecialMenuController& oSpecialMenuController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            SpecialMenuController& operator=( const SpecialMenuController& oSpecialMenuController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }
			Model* m_pModel;
            SpecialMenuDialog* m_pDlg;
    };


  	class SpecialMenuDialog:public GUI::GraphMenuDialogC
	{
		 public:

		   SpecialMenuDialog(Model* pModel,int showItems);

            // Description: Destructor
            virtual ~SpecialMenuDialog();

			virtual void OnInitDialog(void);

			// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
			//
			virtual void OnSelectionDone(void);
			
			// virtual bool SetModel( GUI::ModelC* pModel );
			// DCP::Model* DCP::SpecialMenuDialog::GetModel() const;

			virtual void OnF1Pressed(void);

	private:
			Model* m_pModel;
			int m_pShowItems;

	};

};

#endif // DCP_APPLICATION_HPP



