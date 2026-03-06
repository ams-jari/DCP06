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

#ifndef DCP_CIRCLESAVEDLG_HPP
#define DCP_CIRCLESAVEDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/measurement/Circle.hpp>
#include <dcp06/file/CircleFile.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
//#include <GUI_TextBoxCtrl.hpp>
#include <GUI_AppBase.hpp>
#include <GUI_TextCtrl.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class Model;
	class SaveCircleDialog;
	class SaveCircleModel;

    // Description: Tabbed controller for the Hello World application
    class SaveCircleController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            SaveCircleController(CircleModel* pCircleModel, Model* pModel);
			~SaveCircleController();

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
            SaveCircleController( const SaveCircleController& oSaveCircleController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            SaveCircleController& operator=( const SaveCircleController& oSaveCircleController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            SaveCircleDialog* m_pDlg;
			CircleModel* m_pCircleModel;
			SaveCircleModel* m_pDataModel;

			Model* m_pModel;
			
    };


    class SaveCircleDialog:public GUI::StandardDialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eId=1,
				eFile,
			};

		   SaveCircleDialog(CircleModel* pCircleModel, SaveCircleModel* pSaveCircleModel);

            // Description: Destructor
            virtual ~SaveCircleDialog();

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

			CircleModel* m_pCircleModel;
			SaveCircleModel* m_pDataModel;

			//OBS_DECLARE_EXECUTE(Alignment321Dialog);

			// Description: add all controls
            

	private:
			//OBS::ObserverC m_pComboBoxObserver;
			//virtual void OnComboBoxChanged(int unNotifyCode,  int ulParam2);

			StringC m_strXLine;
			StringC m_strYLine;
			StringC m_strZLine;
	};
	
	class SaveCircleModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            SaveCircleModel(Model* pModel);

            // Description: Destructor
            //
            virtual ~SaveCircleModel();

			CircleFileFunc* m_pFileFunc;
    };
};

#endif // DCP_BASEGUI_HPP




