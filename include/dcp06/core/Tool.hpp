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

#ifndef DCP_TOOLDLG_HPP
#define DCP_TOOLDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Model.hpp>
#include <dcp06/core/Common.hpp>

#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
//#include <GUI_TextBoxCtrl.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
//#include <GUI_DrawCtrl.hpp>
// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class Model;
	class ToolDialog;
	class ToolModel;


    // Description: Tabbed controller for the Hello World application
    class ToolController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            ToolController();
			~ToolController();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			virtual void OnF2Pressed();
			virtual void OnF3Pressed();
			virtual void OnF4Pressed();
			virtual void OnF5Pressed();
			//virtual void OnF6Pressed();
			//virtual void OnSHF1Pressed();
			virtual void OnSHF2Pressed();
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
            ToolController( const ToolController& oToolController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            ToolController& operator=( const ToolController& oToolController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            ToolDialog* m_pDlg;
			ToolModel *m_pDataModel;
			Model* m_pModel;
			Common* m_pCommon;
    };


	class ToolDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC, public OBS::CommandC
	{
		 public:

			enum eCtrlId
			{
				eToolId=1,
				eConstant,
				eTranslX,
				eTranslY,
				eTranslZ,
				eToolX_Total,
				eToolY_Total,
				eToolZ_Total
			};

		   ToolDialog(ToolModel* pToolModel);

            // Description: Destructor
            virtual ~ToolDialog();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			Model* GetModel() const;

	protected:
  			
			GUI::ComboLineCtrlC* m_pToolId;
			GUI::ComboLineCtrlC* m_pConstant;
			GUI::ComboLineCtrlC* m_pTranslX;
			GUI::ComboLineCtrlC* m_pTranslY;
			GUI::ComboLineCtrlC* m_pTranslZ;
			GUI::ComboLineCtrlC* m_pToolX_Total;
			GUI::ComboLineCtrlC* m_pToolY_Total;
			GUI::ComboLineCtrlC* m_pToolZ_Total;

			OBS_DECLARE_EXECUTE(ToolDialog);

	private:
			ToolModel *m_pDataModel;
			Common* m_pCommon;

			OBS::ObserverC m_pXObserver;
			OBS::ObserverC m_pYObserver;
			OBS::ObserverC m_pZObserver;
			virtual void OnValueChanged( int unNotifyCode,  int ulParam2);
			//Model* m_pModel;
	};

	class ToolModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            ToolModel();

			// Description: Destructor
			~ToolModel();
            
			short iOld_active_tool;
			double dOld_x_trans;
			double dOld_y_trans;
			double dOld_z_trans;
			double dConstant;
			short  iActiveTool;
			double dTransX;
			double dTransY;
			double dTransZ;

			
			S_TOOL tool_table[MAX_TOOLS];
	  };
};

#endif // DCP_BASEGUI_HPP




