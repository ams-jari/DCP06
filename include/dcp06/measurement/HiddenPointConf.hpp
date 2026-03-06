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

#ifndef DCP_USERDLG_HPP
#define DCP_USERDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Defs.hpp>
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
	class HiddenPointConfDialog;
	class HiddenPointBarConfModel;


    // Description: Tabbed controller for the Hello World application
    class HiddenPointConfController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            HiddenPointConfController(Model *pModel);
			~HiddenPointConfController();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            HiddenPointConfController( const HiddenPointConfController& oHiddenPointConfController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            HiddenPointConfController& operator=( const HiddenPointConfController& oHiddenPointConfController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            HiddenPointConfDialog* m_pDlg;
			Model *m_pModel;
    };


	class HiddenPointConfDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eDist1=1,
				eDist2,
				eDist3,
				eDist4,
				eDist5
			};

		   HiddenPointConfDialog(Model *pModel);

            // Description: Destructor
            virtual ~HiddenPointConfDialog();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            HiddenPointBarConfModel* GetModel() const;

			// Description: add all controls
            
		

	protected:
			virtual void RefreshControls();
  			
			GUI::ComboLineCtrlC* m_pDist1;
			GUI::ComboLineCtrlC* m_pDist2;
			GUI::ComboLineCtrlC* m_pDist3;
			GUI::ComboLineCtrlC* m_pDist4;
			GUI::ComboLineCtrlC* m_pDist5;

			Model *m_pModel;
	};

	class HiddenPointBarConfModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            HiddenPointBarConfModel();

            // Description: Destructor
            //
            virtual ~HiddenPointBarConfModel();
			
			double hidden_point_bar[MAX_POINTS_IN_HIDDENPOINT_BAR];
    };
};

#endif // DCP_BASEGUI_HPP




