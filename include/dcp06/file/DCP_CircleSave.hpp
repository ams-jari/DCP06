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

#ifndef DCP_CIRCLESAVEDLG_HPP
#define DCP_CIRCLESAVEDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/measurement/DCP_Circle.hpp>
#include <dcp06/file/DCP_CircleFile.hpp>
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
    class DCP06ModelC;
	class DCP06SaveCircleDlgC;
	class DCP06SaveCircleModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP06SaveCircleControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06SaveCircleControllerC(DCP06CircleModelC* pCircleModel, DCP06ModelC* pDCP06Model);
			~DCP06SaveCircleControllerC();

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
            DCP06SaveCircleControllerC( const DCP06SaveCircleControllerC& oDCP06SaveCircleController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06SaveCircleControllerC& operator=( const DCP06SaveCircleControllerC& oDCP06SaveCircleController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06SaveCircleDlgC* m_pDlg;
			DCP06CircleModelC* m_pCircleModel;
			DCP06SaveCircleModelC* m_pDataModel;

			DCP06ModelC* m_pDCP06Model;
			
    };


    class DCP06SaveCircleDlgC:public GUI::StandardDialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eId=1,
				eFile,
			};

		   DCP06SaveCircleDlgC(DCP06CircleModelC* pCircleModel, DCP06SaveCircleModelC* pSaveCircleModel);

            // Description: Destructor
            virtual ~DCP06SaveCircleDlgC();

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

			DCP06CircleModelC* m_pCircleModel;
			DCP06SaveCircleModelC* m_pDataModel;

			//OBS_DECLARE_EXECUTE(DCP06DomDlgC);

			// Description: add all controls
            

	private:
			//OBS::ObserverC m_pComboBoxObserver;
			//virtual void OnComboBoxChanged(int unNotifyCode,  int ulParam2);

			StringC m_strXLine;
			StringC m_strYLine;
			StringC m_strZLine;
	};
	
	class DCP06SaveCircleModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06SaveCircleModelC(DCP06ModelC* pDCP06Model);

            // Description: Destructor
            //
            virtual ~DCP06SaveCircleModelC();

			CircleFileFunc* m_pFileFunc;
    };
};

#endif // DCP_BASEGUI_HPP




