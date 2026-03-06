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

#ifndef DCP_AUTOMATCHDLG_HPP
#define DCP_AUTOMATCHDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>

// Description:
//
namespace DCP
{
    // Forward declaration
	class DCP06ModelC;
	class DCP06AutoMatchDlgC;
	class DCP06AutoMatchModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP06AutoMatchControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06AutoMatchControllerC(DCP06ModelC *pDCP06Model);
			~DCP06AutoMatchControllerC();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			//virtual void OnF2Pressed();
			//virtual void OnF3Pressed();
			//virtual void OnF4Pressed();
			//virtual void OnF5Pressed();
			virtual void OnF5Pressed();
			//virtual void OnSHF1Pressed();
			//virtual void OnSHF2Pressed();
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
            DCP06AutoMatchControllerC( const DCP06AutoMatchControllerC& oDCP06AutoMatchController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06AutoMatchControllerC& operator=( const DCP06AutoMatchControllerC& oDCP06AutoMatchController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06AutoMatchDlgC* m_pDlg;
			DCP06ModelC* m_pDCP06Model;
    };


	class DCP06AutoMatchDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eLine1=1,
				eLine2,
				eX,
				eY,
				eZ
			};

			DCP06AutoMatchDlgC(DCP::DCP06ModelC* pModel);

            // Description: Destructor
            virtual ~DCP06AutoMatchDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP::DCP06AutoMatchModelC* GetDataModel() const;

			
	protected:
  			
			GUI::TextCtrlC* m_pLine1;
			GUI::TextCtrlC* m_pLine2;
			GUI::TextCtrlC* m_pX;
			GUI::TextCtrlC* m_pY;
			GUI::TextCtrlC* m_pZ;
	private:
			
			DCP06ModelC* m_pDCP06Model;
	};

	class DCP06AutoMatchModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06AutoMatchModelC();

            // Description: Destructor
            //
            virtual ~DCP06AutoMatchModelC();
			double xmea, ymea, zmea;
			double xdsg, ydsg, zdsg;
			StringC pointid;
			short pno;
	 };


};

#endif // DCP_BASEGUI_HPP




