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

#ifndef DCP_CALCLINE_CONTROLLER_HPP
#define DCP_CALCLINE_CONTROLLER_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Common.hpp>

#include <GUI_AppBase.hpp>
#include <TBL_SurveyModel.hpp>
#include <TBL_Measurement.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration


    // Description: Tabbed controller for the Hello World application
    class DCP06CalcLineControllerC : public GUI::ControllerC
	{
	public:

            // Description: Constructor
            DCP06CalcLineControllerC(S_LINE_BUFF* oLine, short actdes,short iAskId);
			~DCP06CalcLineControllerC();


            // Description: Handle change of position values
           // virtual void OnF1Pressed(); 
			//virtual void OnF2Pressed();
			//virtual void OnF3Pressed();
			//virtual void OnF4Pressed();
			//virtual void OnF5Pressed();
			//virtual void OnF6Pressed();
			//virtual void OnSHF1Pressed();
			//virtual void OnSHF2Pressed();
			//virtual void OnSHF3Pressed();
			// Description: Route model to everybody else

			virtual bool SetModel( GUI::ModelC* pModel );
		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			virtual void OnControllerActivated(void);
			virtual void Run(void);
			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP06CalcLineControllerC( const DCP06CalcLineControllerC& oDCP06CalcLineController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06CalcLineControllerC& operator=( const DCP06CalcLineControllerC& oDCP06CalcLineController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

			DCP06CommonC* m_pCommon;
			DCP06ModelC* m_pDCP06Model;
			
			S_LINE_BUFF* m_pLineBuff;
			short m_iActDes;
			short m_iAskId;
    };



};

#endif // DCP_BASEGUI_HPP




