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

#ifndef DCP_SELECTTOOL_DLG_HPP
#define DCP_SELECTTOOL_DLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Defs.hpp>

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
    class DCP05ModelC;
	class DCP05SelectToolDlgC;
	class DCP05SelectToolModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP05SelectToolControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05SelectToolControllerC();
			~DCP05SelectToolControllerC();

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
			//void OnActiveControllerClosed( int lCtrlID, int lExitCode );

        private:

			void OnActiveControllerClosed( int lCtrlID, int lExitCode );

			// Description: Copy constructor
            // Remarks    : not implemented
            DCP05SelectToolControllerC( const DCP05SelectToolControllerC& oDCP05SelectToolController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05SelectToolControllerC& operator=( const DCP05SelectToolControllerC& oDCP05SelectToolController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05SelectToolDlgC* m_pDlg;
    };


    class DCP05SelectToolDlgC:public GUI::TableDialogC, public GUI::ModelHandlerC
	{
	 public:

			// TODO VIVA
			/*
			enum CW_ColumnWidth
			{
				CW_No = GUI::ListConfigColsC::CW_10
			};	
			*/
			enum CI_ColumnId
			{
				CI_No = 1
			};

		   DCP05SelectToolDlgC();

            // Description: Destructor
            virtual ~DCP05SelectToolDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();

			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP05SelectToolModelC* GetDataModel() const;

		protected:
			
			GUI::ListMultiColCtrlC* poMultiColCtrl;	
			
		
		private:
			StringC sActualSelected;
			StringC sDesignSelected;
	};

	//              
    class DCP05SelectToolModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP05SelectToolModelC();

            // Description: Destructor
            //
            virtual ~DCP05SelectToolModelC();
			
			S_TOOL* tool_table;
			short m_iCounts;
						
			short m_iSelectedId;

    };
};

#endif // DCP_BASEGUI_HPP




