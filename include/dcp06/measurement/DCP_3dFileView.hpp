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

#ifndef DCP_3DFILEVIEW_DLG_HPP
#define DCP_3DFILEVIEW_DLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
#include <TBL_SurveyModel.hpp>
#include <TBL_Measurement.hpp>

#include "dcp06/file/DCP_File.hpp"
// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class DCP05ModelC;
	class DCP053DFileViewDlgC;


    // Description: Tabbed controller for the Hello World application
    class DCP053DFileViewControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP053DFileViewControllerC(DCP::DCP05FileModelC* pDataModel);
			~DCP053DFileViewControllerC();

            // Description: Handle change of position values
            virtual void OnF1Pressed(); 
			virtual void OnF2Pressed();
			virtual void OnF3Pressed();
			virtual void OnF4Pressed();
			virtual void OnF5Pressed();
			virtual void OnF6Pressed();
			//virtual void OnSHF1Pressed();
			virtual void OnSHF2Pressed();
			/*
			virtual void OnSHF3Pressed();
			virtual void OnSHF4Pressed();
			virtual void OnSHF5Pressed();
			virtual void OnSHF6Pressed();
            */
			// Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );
			virtual bool ConfirmClose(bool bEsc);
		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			virtual void OnControllerActivated(void);
			
			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP053DFileViewControllerC( const DCP053DFileViewControllerC& oDCP053DMeasController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP053DFileViewControllerC& operator=( const DCP053DFileViewControllerC& o3DMeasToolController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP053DFileViewDlgC* m_pDlg;
			//DCP05ModelC *m_pDCP05Model;
			DCP::DCP05FileModelC* m_pDataModel;
    };


	class DCP053DFileViewDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				
				eFile=1,
				ePointId,
				eXDsg,
				eYDsg,
				eZDsg,
				eXActDev,
				eYActDev,
				eZActDev
			};

		   DCP053DFileViewDlgC(DCP::DCP05FileModelC* pDataModelModel);

            // Description: Destructor
            virtual ~DCP053DFileViewDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP::DCP05ModelC* GetDCP05Model() const;

	protected:
  			
			
			GUI::ComboLineCtrlC* m_pFile;
			GUI::ComboLineCtrlC* m_pPointId;
			GUI::ComboLineCtrlC* m_pXDsg;
			GUI::ComboLineCtrlC* m_pYDsg;
			GUI::ComboLineCtrlC* m_pZDsg;
			GUI::ComboLineCtrlC* m_pXActDev;
			GUI::ComboLineCtrlC* m_pYActDev;
			GUI::ComboLineCtrlC* m_pZActDev;
			/*
			GUI::ComboLineCtrlC* m_pPointId;
			GUI::ComboLineCtrlC* m_pX;
			GUI::ComboLineCtrlC* m_pY;
			GUI::ComboLineCtrlC* m_pZ;
			*/

	private:
			DCP::DCP05CommonC *pCommon;
			//DCP::DCP05ModelC* m_pDCP05Model;
			DCP::DCP05FileModelC* m_pDataModel;
	};
};

#endif // DCP_BASEGUI_HPP




