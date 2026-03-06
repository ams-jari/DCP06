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

#ifndef DCP_ROTATE_PLANE_DLG_HPP
#define DCP_ROTATE_PLANE_DLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/PointBuffModel.hpp>

#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class Model;
	class RotatePlaneDialog;
	class RotatePlaneModel;


    // Description: Tabbed controller for the Hello World application
    class RotatePlaneController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            RotatePlaneController(Model *pModel);
			~RotatePlaneController();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			//virtual void OnF2Pressed();
			//virtual void OnF3Pressed();
			//virtual void OnF4Pressed();
			//virtual void OnF5Pressed();
			//virtual void OnF6Pressed();
			//virtual void OnSHF1Pressed();
			//virtual void OnSHF2Pressed();
			//virtual void OnSHF3Pressed();
			//virtual void OnSHF4Pressed();
			//virtual void OnSHF5Pressed();
			//virtual void OnSHF6Pressed();
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );
		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			
			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            RotatePlaneController( const RotatePlaneController& oRotatePlaneController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            RotatePlaneController& operator=( const RotatePlaneController& oRotatePlaneController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            RotatePlaneDialog* m_pDlg;
			Model *m_pModel;
    };


	class RotatePlaneDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{	
				eInfo1=1,
				eAxis1,
				eAxis2,
                eInfo2
				/*
				ePointNo=1,
				ePointId,
				eX,
				eY,
				eZ
				*/
			};

		   RotatePlaneDialog(Model *pModel);

            // Description: Destructor
            virtual ~RotatePlaneDialog();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			RotatePlaneModel* GetDataModel() const;

	protected:
			GUI::TextCtrlC* m_pInfo1;

			GUI::ComboLineCtrlC* m_pAxis1;
			GUI::ComboLineCtrlC* m_pAxis2;
			GUI::TextCtrlC* m_pInfo2;
	private:
			Model *m_pModel;
	};

	class RotatePlaneModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            RotatePlaneModel();

            // Description: Destructor
            //
            virtual ~RotatePlaneModel();
						
			short/*DCP_PLANE_TYPE*/ plane_type;
			S_POINT_BUFF point_buff;
			int display;
			StringC sTitle;
    };
};

#endif // DCP_BASEGUI_HPP




