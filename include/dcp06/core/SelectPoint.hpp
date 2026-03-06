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

#ifndef DCP_SELECTPOINT_DLG_HPP
#define DCP_SELECTPOINT_DLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Defs.hpp>

#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_TableDialog.hpp>
#include <GUI_ListMultiColCtrl.hpp>
#include <GUI_AppBase.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class Model;
	class SelectPointDialog;
	class SelectPointModel;

    // Description: Tabbed controller for the Hello World application
    class SelectPointController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            SelectPointController();
			~SelectPointController();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			//virtual void OnF2Pressed();
			//virtual void OnF5Pressed();
			//virtual void OnF6Pressed();
			//virtual void OnSHF1Pressed();
			//virtual void OnSHF2Pressed();
			//virtual void OnSHF5Pressed();

            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			void OnActiveControllerClosed( int lCtrlID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            SelectPointController( const SelectPointController& oSelectPointController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            SelectPointController& operator=( const SelectPointController& oSelectPointController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            SelectPointDialog* m_pDlg;
    };


    class SelectPointDialog:public GUI::TableDialogC, public GUI::ModelHandlerC
	{
	 public:

			// VIVA REMOVED
			/*
			enum CW_ColumnWidth
			{
				CW_No = GUI:ListConfigColsC::CW_10,
				CW_Point = GUI::ListConfigColsC::CW_10,
				CW_ActualDesign = GUI::ListConfigColsC::CW_5,
			};
			*/
			enum CI_ColumnId
			{
				CI_No = 1,
				CI_Point,  
				CI_ActualDesign
			};

		   SelectPointDialog();

            // Description: Destructor
            virtual ~SelectPointDialog();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();

			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            SelectPointModel* GetDataModel() const;

		protected:
			
			GUI::ListMultiColCtrlC* poMultiColCtrl;	
			
		
		private:
			StringC sActualSelected;
			StringC sDesignSelected;
	};

	//              
    class SelectPointModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            SelectPointModel();

            // Description: Destructor
            //
            virtual ~SelectPointModel();
			
			S_SELECT_POINT points[MAX_SELECT_POINTS];
			short m_iCounts;
			
			StringC m_strSelectedPoint;
			short m_iSelectedId;

    };
};

#endif // DCP_BASEGUI_HPP




