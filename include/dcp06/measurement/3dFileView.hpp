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

#include <dcp06/core/Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
#include <TBL_SurveyModel.hpp>
#include <TBL_Measurement.hpp>

#include "dcp06/file/File.hpp"
// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class DCP06ModelC;
	class DCP063DFileViewDlgC;


    // Description: Tabbed controller for the Hello World application
    class DCP063DFileViewControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP063DFileViewControllerC(DCP::DCP06FileModelC* pDataModel);
			~DCP063DFileViewControllerC();

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
            DCP063DFileViewControllerC( const DCP063DFileViewControllerC& oDCP063DMeasController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP063DFileViewControllerC& operator=( const DCP063DFileViewControllerC& o3DMeasToolController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP063DFileViewDlgC* m_pDlg;
			//DCP06ModelC *m_pDCP06Model;
			DCP::DCP06FileModelC* m_pDataModel;
    };


	class DCP063DFileViewDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC
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

		   DCP063DFileViewDlgC(DCP::DCP06FileModelC* pDataModelModel);

            // Description: Destructor
            virtual ~DCP063DFileViewDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP::DCP06ModelC* GetDCP06Model() const;

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
			DCP::DCP06CommonC *pCommon;
			//DCP::DCP06ModelC* m_pDCP06Model;
			DCP::DCP06FileModelC* m_pDataModel;
	};
};

#endif // DCP_BASEGUI_HPP




