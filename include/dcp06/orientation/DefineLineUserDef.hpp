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

#ifndef DCP_DEFINELINEUSERDEFDLG_HPP
#define DCP_DEFINELINEUSERDEFDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
#include <GUI_TextCtrl.hpp>

#include <dcp06/orientation/DefineLine.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class Model;
	class DefineLineUserDefDialog;
	class DefineLineUserDefModel;

    // Description: Tabbed controller for the Hello World application
    class DefineLineUserDefController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DefineLineUserDefController(Model* pModel);
			~DefineLineUserDefController();

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
            DefineLineUserDefController( const DefineLineUserDefController& oDefineLineUserDefController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DefineLineUserDefController& operator=( const DefineLineUserDefController& oDefineLineController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DefineLineUserDefDialog* m_pDlg;
			Model* m_pModel;
			
    };


    class DefineLineUserDefDialog:public GUI::StandardDialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
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

		   DefineLineUserDefDialog(Model* pModel);

            // Description: Destructor
            virtual ~DefineLineUserDefDialog();

			virtual void OnInitDialog(void);

		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DefineLineUserDefModel* GetDataModel() const;
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

			Model* m_pModel;
			// Description: add all controls
            

	private:
			//OBS::ObserverC m_pComboBoxObserver;
			//virtual void OnComboBoxChanged(int unNotifyCode, int ulParam2);

			StringC m_strXLine;
			StringC m_strYLine;
			StringC m_strZLine;
	};
	
    class DefineLineUserDefModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DefineLineUserDefModel();

            // Description: Destructor
            //
            virtual ~DefineLineUserDefModel();
			
			DefineLineModel* lineModel;

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




