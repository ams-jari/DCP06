
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

#ifndef DCP_SHAFTSAVEDLG_HPP
#define DCP_SHAFTSAVEDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/application/Shaft.hpp>
#include <dcp06/file/ShaftFile.hpp>
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
    class Model;
	class SaveShaftDialog;
	class SaveShaftModel;

    // Description: Tabbed controller for the Hello World application
    class SaveShaftController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            SaveShaftController(ShaftModel* pShaftModel, Model* pModel);
			~SaveShaftController();

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
            SaveShaftController( const SaveShaftController& oSaveShaftController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            SaveShaftController& operator=( const SaveShaftController& oSaveShaftController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            SaveShaftDialog* m_pDlg;
			ShaftModel* m_pShaftModel;
			SaveShaftModel* m_pDataModel;

			Model* m_pModel;
			
    };


    class SaveShaftDialog:public GUI::StandardDialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eId=1,
				eFile,
			};

		   SaveShaftDialog(ShaftModel* pShaftModel, SaveShaftModel* pSaveShaftModel);

            // Description: Destructor
            virtual ~SaveShaftDialog();

			virtual void OnInitDialog(void);

		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            Model* GetModel() const;
			virtual void RefreshControls();

			StringC get_id();


	protected:
			
			GUI::ComboLineCtrlC* m_pId;
			GUI::ComboLineCtrlC* m_pFile;

			ShaftModel* m_pShaftModel;
			SaveShaftModel* m_pDataModel;

			//OBS_DECLARE_EXECUTE(Alignment321Dialog);

			// Description: add all controls
            

	private:
			//OBS::ObserverC m_pComboBoxObserver;
			//virtual void OnComboBoxChanged(int unNotifyCode,  int ulParam2);

			StringC m_strXLine;
			StringC m_strYLine;
			StringC m_strZLine;
	};
	
	class SaveShaftModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            SaveShaftModel(Model* pModel);

            // Description: Destructor
            //
            virtual ~SaveShaftModel();

			ShaftFileFunc* m_pFileFunc;
    };
};

#endif // DCP_BASEGUI_HPP




