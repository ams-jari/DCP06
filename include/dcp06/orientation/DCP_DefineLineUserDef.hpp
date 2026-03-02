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

#ifndef DCP_DEFINELINEUSERDEFDLG_HPP
#define DCP_DEFINELINEUSERDEFDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
#include <GUI_TextCtrl.hpp>

#include <dcp06/orientation/DCP_DefineLine.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class DCP05ModelC;
	class DCP05DefineLineUserDefDlgC;
	class DCP05DefineLineUserDefModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP05DefineLineUserDefControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05DefineLineUserDefControllerC(DCP05ModelC* pDCP05Model);
			~DCP05DefineLineUserDefControllerC();

            // Description: Handle change of position values
           	virtual void OnF1Pressed(void);
			virtual void OnF2Pressed(void);
			virtual void OnF3Pressed(void);
			virtual void OnF5Pressed(void);
			virtual void OnF6Pressed(void);
			virtual void OnSHF2Pressed(void);
			
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP05DefineLineUserDefControllerC( const DCP05DefineLineUserDefControllerC& oDCP05DefineLineUserDefController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05DefineLineUserDefControllerC& operator=( const DCP05DefineLineUserDefControllerC& oDCP05DefineLineController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05DefineLineUserDefDlgC* m_pDlg;
			DCP05ModelC* m_pDCP05Model;
			
    };


    class DCP05DefineLineUserDefDlgC:public GUI::StandardDialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eLine=1,
				eLinePoints,
				ePoints
				/*
				eLineInfo1,
				eLineInfo2,
				eLineInfo3
				*/
			};

		   DCP05DefineLineUserDefDlgC(DCP05ModelC* pDCP05Model);

            // Description: Destructor
            virtual ~DCP05DefineLineUserDefDlgC();

			virtual void OnInitDialog(void);

		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP05DefineLineUserDefModelC* GetDataModel() const;
			virtual void RefreshControls();

			virtual void delete_line();
			virtual void x_line();
			virtual void y_line();
			virtual void z_line();
			//virtual bool CalculateLineAfterMeas();

	protected:
			
			GUI::ComboLineCtrlC* m_pLine;
			GUI::TextCtrlC* m_pLinePoints; 
			GUI::TextCtrlC* m_pPoints; 

			DCP05ModelC* m_pDCP05Model;
			// Description: add all controls
            

	private:
			//OBS::ObserverC m_pComboBoxObserver;
			//virtual void OnComboBoxChanged(int unNotifyCode, int ulParam2);

			StringC m_strXLine;
			StringC m_strYLine;
			StringC m_strZLine;
	};
	
    class DCP05DefineLineUserDefModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP05DefineLineUserDefModelC();

            // Description: Destructor
            //
            virtual ~DCP05DefineLineUserDefModelC();
			
			DCP05DefineLineModelC* lineModel;

			S_POINT_BUFF userdef_measured_points[MAX_USERDEF_POINTS];
			short userdef_line_points_no[20];

			S_SELECT_POINTS select_point_list[MAX_USERDEF_POINTS];

			/*
			short  active_plane;
			short  active_line; 
			S_LINE_BUFF	line_buff[1];
			short display;
			StringC sTitle;
			*/
	};
	
};

#endif // DCP_BASEGUI_HPP




