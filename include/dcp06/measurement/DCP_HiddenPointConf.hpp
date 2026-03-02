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

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class DCP05ModelC;
	class DCP05HiddenPointConfDlgC;
	class DCP05HiddenPointBarConfModelC;


    // Description: Tabbed controller for the Hello World application
    class DCP05HiddenPointConfControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05HiddenPointConfControllerC(DCP05ModelC *pDCP05Model);
			~DCP05HiddenPointConfControllerC();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP05HiddenPointConfControllerC( const DCP05HiddenPointConfControllerC& oDCP05HiddenPointConfController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05HiddenPointConfControllerC& operator=( const DCP05HiddenPointConfControllerC& oDCP05HiddenPointConfController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05HiddenPointConfDlgC* m_pDlg;
			DCP05ModelC *m_pDCP05Model;
    };


	class DCP05HiddenPointConfDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC
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

		   DCP05HiddenPointConfDlgC(DCP05ModelC *pDCP05Model);

            // Description: Destructor
            virtual ~DCP05HiddenPointConfDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP05HiddenPointBarConfModelC* GetDCP05Model() const;

			// Description: add all controls
            
		

	protected:
			virtual void RefreshControls();
  			
			GUI::ComboLineCtrlC* m_pDist1;
			GUI::ComboLineCtrlC* m_pDist2;
			GUI::ComboLineCtrlC* m_pDist3;
			GUI::ComboLineCtrlC* m_pDist4;
			GUI::ComboLineCtrlC* m_pDist5;

			DCP05ModelC *m_pDCP05Model;
	};

	class DCP05HiddenPointBarConfModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP05HiddenPointBarConfModelC();

            // Description: Destructor
            //
            virtual ~DCP05HiddenPointBarConfModelC();
			
			double hidden_point_bar[MAX_POINTS_IN_HIDDENPOINT_BAR];
    };
};

#endif // DCP_BASEGUI_HPP




