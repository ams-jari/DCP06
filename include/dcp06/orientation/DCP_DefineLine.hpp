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

#ifndef DCP_DEFINELINEDLG_HPP
#define DCP_DEFINELINEDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
#include <GUI_TextCtrl.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class DCP06ModelC;
	class DCP06DefineLineDlgC;
	class DCP06DefineLineModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP06DefineLineControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06DefineLineControllerC(DCP06ModelC* pDCP06Model);
			~DCP06DefineLineControllerC();

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
            DCP06DefineLineControllerC( const DCP06DefineLineControllerC& oDCP06DefineLineController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06DefineLineControllerC& operator=( const DCP06DefineLineControllerC& oDCP06DefineLineController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06DefineLineDlgC* m_pDlg;
			DCP06ModelC* m_pDCP06Model;
			
    };


    class DCP06DefineLineDlgC:public GUI::StandardDialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eLine=1,
				eLineInfo1,
				eLineInfo2,
				eLineInfo3
			};

		   DCP06DefineLineDlgC(DCP06ModelC* pDCP06Model);

            // Description: Destructor
            virtual ~DCP06DefineLineDlgC();

			virtual void OnInitDialog(void);

		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP06DefineLineModelC* GetDataModel() const;
			virtual void RefreshControls();

			virtual void delete_line();
			virtual void x_line();
			virtual void y_line();
			virtual void z_line();
			virtual bool CalculateLineAfterMeas();

	protected:
			
			GUI::ComboLineCtrlC* m_pLine;
			GUI::TextCtrlC* m_pLineInfo1;
			GUI::TextCtrlC* m_pLineInfo2;
			GUI::TextCtrlC* m_pLineInfo3;

			DCP06ModelC* m_pDCP06Model;

			//OBS_DECLARE_EXECUTE(DCP06DomDlgC);

			// Description: add all controls
            

	private:
			//OBS::ObserverC m_pComboBoxObserver;
			//virtual void OnComboBoxChanged(int unNotifyCode, int ulParam2);

			StringC m_strXLine;
			StringC m_strYLine;
			StringC m_strZLine;
	};
    class DCP06DefineLineModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06DefineLineModelC();

            // Description: Destructor
            //
            virtual ~DCP06DefineLineModelC();

			short /*DCP_PLANE_TYPE*/ active_plane;
			short /*DCP_LINE_TYPE*/  active_line; 
			S_LINE_BUFF	line_buff[1];
			short display;
			StringC sTitle;
    };

};

#endif // DCP_BASEGUI_HPP




