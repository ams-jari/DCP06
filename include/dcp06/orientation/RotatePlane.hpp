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
	class DCP06ModelC;
	class DCP06RotatePlaneDlgC;
	class DCP06RotatePlaneModelC;


    // Description: Tabbed controller for the Hello World application
    class DCP06RotatePlaneControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06RotatePlaneControllerC(DCP06ModelC *pDCP06Model);
			~DCP06RotatePlaneControllerC();

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
            DCP06RotatePlaneControllerC( const DCP06RotatePlaneControllerC& oDCP06RotatePlaneController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06RotatePlaneControllerC& operator=( const DCP06RotatePlaneControllerC& oDCP06RotatePlaneController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06RotatePlaneDlgC* m_pDlg;
			DCP06ModelC *m_pDCP06Model;
    };


	class DCP06RotatePlaneDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC
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

		   DCP06RotatePlaneDlgC(DCP06ModelC *pDCP06Model);

            // Description: Destructor
            virtual ~DCP06RotatePlaneDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP06RotatePlaneModelC* GetDataModel() const;

	protected:
			GUI::TextCtrlC* m_pInfo1;

			GUI::ComboLineCtrlC* m_pAxis1;
			GUI::ComboLineCtrlC* m_pAxis2;
			GUI::TextCtrlC* m_pInfo2;
	private:
			DCP06ModelC *m_pDCP06Model;
	};

	class DCP06RotatePlaneModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06RotatePlaneModelC();

            // Description: Destructor
            //
            virtual ~DCP06RotatePlaneModelC();
						
			short/*DCP_PLANE_TYPE*/ plane_type;
			S_POINT_BUFF point_buff;
			int display;
			StringC sTitle;
    };
};

#endif // DCP_BASEGUI_HPP




