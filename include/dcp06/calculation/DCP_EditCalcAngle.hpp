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

#ifndef DCP_EDITCALCANGLE_HPP
#define DCP_EDITCALCANGLE_HPP

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
	class DCP06ModelC;
	class DCP06EditCalcAngleDlgC;
	class DCP06EditCalcAngleModelC;


    // Description: Tabbed controller for the Hello World application
    class DCP06EditCalcAngleControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06EditCalcAngleControllerC(DCP06ModelC *pDCP06Model);
			~DCP06EditCalcAngleControllerC();

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
            DCP06EditCalcAngleControllerC( const DCP06EditCalcAngleControllerC& oDCP05EditCalcAngleController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06EditCalcAngleControllerC& operator=( const DCP06EditCalcAngleControllerC& oDCP05EditCalcAngleController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06EditCalcAngleDlgC* m_pDlg;
			DCP06ModelC * m_pDCP06Model;
			DCP06CommonC* m_pCommon;
    };


	class DCP06EditCalcAngleDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eDistId=1,
				eRefId,
				eTrgtId,
				eNote
			};

		   DCP06EditCalcAngleDlgC(DCP06ModelC *pDCP06Model);

            // Description: Destructor
            virtual ~DCP06EditCalcAngleDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP06EditCalcAngleModelC* GetDataModel() const;
			

	protected:
  			
			GUI::ComboLineCtrlC* m_pDistId;
			GUI::ComboLineCtrlC* m_pRefId;
			GUI::ComboLineCtrlC* m_pTrgtId;

			GUI::ComboLineCtrlC* m_pNote;

			DCP06ModelC * m_pDCP06Model;
	private:
			DCP06CommonC* m_pCommon;
	};

	 class DCP06EditCalcAngleModelC : public GUI::ModelC
    {
        public:
			StringC sDistId;
			StringC sRefId;
			StringC sTrgtId;
			StringC	sNote;
	};
};

#endif // DCP_BASEGUI_HPP




