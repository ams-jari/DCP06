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

#include <dcp06/core/DCP_Types.hpp>
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
	class DCP06ModelC;
	class DCP06SpecialMenuDlgC;


    // Description: Tabbed controller for the Hello World application
    class DCP06SpecialMenuControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06SpecialMenuControllerC(DCP06ModelC* pDCP06Model, int showItems = 0);
			~DCP06SpecialMenuControllerC();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			
            // Description: Route model to everybody else
            //virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP06SpecialMenuControllerC( const DCP06SpecialMenuControllerC& oDCP06SpecialMenuControllerC )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06SpecialMenuControllerC& operator=( const DCP06SpecialMenuControllerC& oDCP06SpecialMenuControllerC )
            {
                USER_APP_VERIFY( false );
                return *this;
            }
			DCP06ModelC* m_pDCP06Model;
            DCP06SpecialMenuDlgC* m_pDlg;
    };


  	class DCP06SpecialMenuDlgC:public GUI::GraphMenuDialogC
	{
		 public:

		   DCP06SpecialMenuDlgC(DCP06ModelC* pDCP06Model,int showItems);

            // Description: Destructor
            virtual ~DCP06SpecialMenuDlgC();

			virtual void OnInitDialog(void);

			// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
			//
			virtual void OnSelectionDone(void);
			
			// virtual bool SetModel( GUI::ModelC* pModel );
			// DCP::DCP06ModelC* DCP::DCP06SpecialMenuDlgC::GetDCP06Model() const;

			virtual void OnF1Pressed(void);

	private:
			DCP06ModelC* m_pDCP06Model;
			int m_pShowItems;

	};

};

#endif // DCP_APPLICATION_HPP



