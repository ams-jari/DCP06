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

#ifndef DCP_EDITCALCDIST_HPP
#define DCP_EDITCALCDIST_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_PointBuffModel.hpp>
#include <dcp06/core/DCP_Common.hpp>

#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
//#include <GUI_Timer.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class DCP05ModelC;
	class DCP05EditCalcDistDlgC;
	class DCP05EditCalcDistModelC;


    // Description: Tabbed controller for the Hello World application
    class DCP05EditCalcDistControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05EditCalcDistControllerC(DCP05ModelC *pDCP05Model);
			~DCP05EditCalcDistControllerC();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			//virtual void OnF2Pressed();
			//virtual void OnF3Pressed();
			//virtual void OnF4Pressed();
			//virtual void OnF6Pressed();
			//virtual void OnSHF2Pressed();
			//virtual void OnSHF3Pressed();
			//virtual void OnSHF4Pressed();
			// Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			
			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP05EditCalcDistControllerC( const DCP05EditCalcDistControllerC& oDCP05EditCalcDistController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05EditCalcDistControllerC& operator=( const DCP05EditCalcDistControllerC& oDCP05EditCalcDistController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05EditCalcDistDlgC* m_pDlg;
			DCP05ModelC * m_pDCP05Model;
			DCP05CommonC* m_pCommon;
    };


	class DCP05EditCalcDistDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eDistId=1,
				eRefId,
				eNote
			};

		   DCP05EditCalcDistDlgC(DCP05ModelC *pDCP05Model);

            // Description: Destructor
            virtual ~DCP05EditCalcDistDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP05EditCalcDistModelC* GetDataModel() const;
			

	protected:
  			
			GUI::ComboLineCtrlC* m_pDistId;
			GUI::ComboLineCtrlC* m_pRefId;
			GUI::ComboLineCtrlC* m_pNote;

			DCP05ModelC * m_pDCP05Model;
	private:
			DCP05CommonC* m_pCommon;
	};

	 class DCP05EditCalcDistModelC : public GUI::ModelC
    {
        public:
			StringC sDistId;
			StringC sRefId;
			StringC	sNote;
	};
};

#endif // DCP_BASEGUI_HPP




