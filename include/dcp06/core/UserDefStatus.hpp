#ifndef DCP_USERDEFSTATUS_HPP
#define DCP_USERDEFSTATUS_HPP

// ================================================================================================
// ===========================================  Includes   ========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Model.hpp>
#include <UTL_Macro.hpp>
#include <GUI_Application.hpp>
#include <GUI_GraphMenuDialog.hpp>
#include <GUI_TableDialog.hpp>
#include <GUI_ListMultiColCtrl.hpp>
#include <GUI_TextCtrl.hpp>

// Description: The Hello World application
//
namespace DCP
{
	class DCP05UserDefStatusDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				
				
			};

			DCP05UserDefStatusDlgC(DCP05ModelC* pDCP05Model, bool defined_);
			~DCP05UserDefStatusDlgC();


            virtual void OnInitDialog(void);
			virtual void OnDialogActivated();
			virtual void OnF1Pressed();
			virtual void OnF3Pressed();

			virtual void RefreshControls();
	//protected:
		
	private:
			GUI::TextCtrlC* m_pText1;

			DCP05ModelC* m_pDCP05Model;
			bool Defined;
		
	};

};

#endif // DCP_APPLICATION_HPP