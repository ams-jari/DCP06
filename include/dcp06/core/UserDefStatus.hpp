#ifndef DCP_USERDEFSTATUS_HPP
#define DCP_USERDEFSTATUS_HPP

// ================================================================================================
// ===========================================  Includes   ========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Model.hpp>
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
	class DCP06UserDefStatusDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				
				
			};

			DCP06UserDefStatusDlgC(DCP06ModelC* pDCP06Model, bool defined_);
			~DCP06UserDefStatusDlgC();


            virtual void OnInitDialog(void);
			virtual void OnDialogActivated();
			virtual void OnF1Pressed();
			virtual void OnF3Pressed();

			virtual void RefreshControls();
	//protected:
		
	private:
			GUI::TextCtrlC* m_pText1;

			DCP06ModelC* m_pDCP06Model;
			bool Defined;
		
	};

};

#endif // DCP_APPLICATION_HPP