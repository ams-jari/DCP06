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
    class DCP05ModelC;
	class DCP05SaveCircleDlgC;
	class DCP05SaveCircleModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP05SaveCircleControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05SaveCircleControllerC(DCP05CircleModelC* pCircleModel, DCP05ModelC* pDCP05Model);
			~DCP05SaveCircleControllerC();

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
            DCP05SaveCircleControllerC( const DCP05SaveCircleControllerC& oDCP05SaveCircleController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05SaveCircleControllerC& operator=( const DCP05SaveCircleControllerC& oDCP05SaveCircleController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05SaveCircleDlgC* m_pDlg;
			DCP05CircleModelC* m_pCircleModel;
			DCP05SaveCircleModelC* m_pDataModel;

			DCP05ModelC* m_pDCP05Model;
			
    };


    class DCP05SaveCircleDlgC:public GUI::StandardDialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eId=1,
				eFile,
			};

		   DCP05SaveCircleDlgC(DCP05CircleModelC* pCircleModel, DCP05SaveCircleModelC* pSaveCircleModel);

            // Description: Destructor
            virtual ~DCP05SaveCircleDlgC();

			virtual void OnInitDialog(void);

		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP05ModelC* GetDCP05Model() const;
			virtual void RefreshControls();

			StringC get_id();


	protected:
			
			GUI::ComboLineCtrlC* m_pId;
			GUI::ComboLineCtrlC* m_pFile;

			DCP05CircleModelC* m_pCircleModel;
			DCP05SaveCircleModelC* m_pDataModel;

			//OBS_DECLARE_EXECUTE(DCP05DomDlgC);

			// Description: add all controls
            

	private:
			//OBS::ObserverC m_pComboBoxObserver;
			//virtual void OnComboBoxChanged(int unNotifyCode,  int ulParam2);

			StringC m_strXLine;
			StringC m_strYLine;
			StringC m_strZLine;
	};
	
	class DCP05SaveCircleModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP05SaveCircleModelC(DCP05ModelC* pDCP05Model);

            // Description: Destructor
            //
            virtual ~DCP05SaveCircleModelC();

			CircleFileFunc* m_pFileFunc;
    };
};

#endif // DCP_BASEGUI_HPP




