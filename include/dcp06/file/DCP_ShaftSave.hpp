
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

#ifndef DCP_SHAFTSAVEDLG_HPP
#define DCP_SHAFTSAVEDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/application/DCP_Shaft.hpp>
#include <dcp06/file/DCP_ShaftFile.hpp>
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
	class DCP06SaveShaftDlgC;
	class DCP06SaveShaftModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP06SaveShaftControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06SaveShaftControllerC(DCP06ShaftModelC* pShaftModel, DCP06ModelC* pDCP06Model);
			~DCP06SaveShaftControllerC();

            // Description: Handle change of position values
           	virtual void OnF1Pressed(void);
			virtual void OnF3Pressed(void);
			virtual void OnF6Pressed(void);
			
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP06SaveShaftControllerC( const DCP06SaveShaftControllerC& oDCP05SaveShaftController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06SaveShaftControllerC& operator=( const DCP06SaveShaftControllerC& oDCP05SaveShaftController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06SaveShaftDlgC* m_pDlg;
			DCP06ShaftModelC* m_pShaftModel;
			DCP06SaveShaftModelC* m_pDataModel;

			DCP06ModelC* m_pDCP06Model;
			
    };


    class DCP06SaveShaftDlgC:public GUI::StandardDialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eId=1,
				eFile,
			};

		   DCP06SaveShaftDlgC(DCP06ShaftModelC* pShaftModel, DCP06SaveShaftModelC* pSaveShaftModel);

            // Description: Destructor
            virtual ~DCP06SaveShaftDlgC();

			virtual void OnInitDialog(void);

		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP06ModelC* GetDCP06Model() const;
			virtual void RefreshControls();

			StringC get_id();


	protected:
			
			GUI::ComboLineCtrlC* m_pId;
			GUI::ComboLineCtrlC* m_pFile;

			DCP06ShaftModelC* m_pShaftModel;
			DCP06SaveShaftModelC* m_pDataModel;

			//OBS_DECLARE_EXECUTE(DCP06DomDlgC);

			// Description: add all controls
            

	private:
			//OBS::ObserverC m_pComboBoxObserver;
			//virtual void OnComboBoxChanged(int unNotifyCode,  int ulParam2);

			StringC m_strXLine;
			StringC m_strYLine;
			StringC m_strZLine;
	};
	
	class DCP06SaveShaftModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06SaveShaftModelC(DCP06ModelC* pDCP06Model);

            // Description: Destructor
            //
            virtual ~DCP06SaveShaftModelC();

			ShaftFileFunc* m_pFileFunc;
    };
};

#endif // DCP_BASEGUI_HPP




