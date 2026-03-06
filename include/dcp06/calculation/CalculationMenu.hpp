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

#ifndef DCP_CALCMENU_HPP
#define DCP_CALCMENU_HPP

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
	class DCP06ModelC;
	class DCP06CalcMenuDlgC;

  	class DCP06CalcMenuDlgC:public GUI::GraphMenuDialogC
	{
		 public:

		   DCP06CalcMenuDlgC(DCP06ModelC* pDCP06Model);

            // Description: Destructor
            virtual ~DCP06CalcMenuDlgC();

			virtual void OnInitDialog(void);

			// Description: Called if selection is completed (by ENTER, mouse click or numeric keys)
			//
			virtual void OnSelectionDone(void);
			
			// virtual bool SetModel( GUI::ModelC* pModel );
			// DCP::DCP06ModelC* DCP::DCP06SpecialMenuDlgC::GetDCP06Model() const;

			virtual void OnF1Pressed(void);

	private:
		DCP06ModelC* m_pDCP06Model;
	};

};

#endif // DCP_APPLICATION_HPP



