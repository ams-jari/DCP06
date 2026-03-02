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

#ifndef DCP_TOOLDLG_HPP
#define DCP_TOOLDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Model.hpp>
#include <dcp06/core/DCP_Common.hpp>

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
	class DCP06ModelC;
	class DCP06ToolDlgC;
	class DCP06ToolModelC;


    // Description: Tabbed controller for the Hello World application
    class DCP06ToolControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06ToolControllerC();
			~DCP06ToolControllerC();

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
            DCP06ToolControllerC( const DCP06ToolControllerC& oDCP06ToolController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06ToolControllerC& operator=( const DCP06ToolControllerC& oDCP06ToolController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06ToolDlgC* m_pDlg;
			DCP06ToolModelC *m_pDataModel;
			DCP06ModelC* m_pDCP06Model;
			DCP06CommonC* m_pCommon;
    };


	class DCP06ToolDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC, public OBS::CommandC
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

		   DCP06ToolDlgC(DCP06ToolModelC* pToolModel);

            // Description: Destructor
            virtual ~DCP06ToolDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP06ModelC* GetDCP06Model() const;

	protected:
  			
			GUI::ComboLineCtrlC* m_pToolId;
			GUI::ComboLineCtrlC* m_pConstant;
			GUI::ComboLineCtrlC* m_pTranslX;
			GUI::ComboLineCtrlC* m_pTranslY;
			GUI::ComboLineCtrlC* m_pTranslZ;
			GUI::ComboLineCtrlC* m_pToolX_Total;
			GUI::ComboLineCtrlC* m_pToolY_Total;
			GUI::ComboLineCtrlC* m_pToolZ_Total;

			OBS_DECLARE_EXECUTE(DCP06ToolDlgC);

	private:
			DCP06ToolModelC *m_pDataModel;
			DCP06CommonC* m_pCommon;

			OBS::ObserverC m_pXObserver;
			OBS::ObserverC m_pYObserver;
			OBS::ObserverC m_pZObserver;
			virtual void OnValueChanged( int unNotifyCode,  int ulParam2);
			//DCP06ModelC* m_pDCP06Model;
	};

	class DCP06ToolModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            DCP06ToolModelC();

			// Description: Destructor
			~DCP06ToolModelC();
            
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




