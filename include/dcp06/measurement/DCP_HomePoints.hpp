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

#ifndef DCP_HOMEPOINTSDLG_HPP
#define DCP_HOMEPOINTSDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Common.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class DCP05ModelC;
	class DCP05HomePointsDlgC;
	class DCP05HomePointsModelC;
	class DCP05SelectCoordinateSystemC;


    // Description: Tabbed controller for the Hello World application
    class DCP05HomePointsControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05HomePointsControllerC(DCP05ModelC* pDCP05Model);
			~DCP05HomePointsControllerC();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			virtual void OnF2Pressed();
			//virtual void OnF3Pressed();
			virtual void OnF4Pressed();
			virtual void OnF5Pressed();
			//virtual void OnF6Pressed();
			//virtual void OnSHF1Pressed();
			//virtual void OnSHF2Pressed();
			//virtual void OnSHF3Pressed();
			virtual void OnSHF4Pressed();
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
            DCP05HomePointsControllerC( const DCP05HomePointsControllerC& oDCP05HomePointsController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05HomePointsControllerC& operator=( const DCP05HomePointsControllerC& oDCP05HomePointsController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05HomePointsDlgC* m_pDlg;
			DCP05HomePointsModelC* m_pDataModel;
			DCP05ModelC* m_pDCP05Model;
			DCP05CommonC* m_pCommon;
    };


	class DCP05HomePointsDlgC:public GUI::TableDialogC, public GUI::ModelHandlerC
	{
		 public:

		    // VIVA REMOVED
		    /*
			enum CW_ColumnWidth
			{
				CW_PointNo = GUI::ListConfigColsC::CW_5,
				CW_PointId = GUI::ListConfigColsC::CW_10,
				CW_PointStatus =GUI::ListConfigColsC::CW_10
			};
			*/

			enum CI_ColumnId
			{
				CI_PointNo = 1,
				CI_PointId,
				CI_PointStatus
			};


		   DCP05HomePointsDlgC(DCP05HomePointsModelC* pHomePointsModel);

            // Description: Destructor
            virtual ~DCP05HomePointsDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP05ModelC* GetDCP05Model() const;

			short get_selected_id();

	protected:
  			
			GUI::ListMultiColCtrlC* poMultiColCtrl;	

			StringC sPointNo;
			StringC sPointId;
			StringC sPointStatus;

			StringC sDCS;
			StringC sOCS;
	private:
			
			DCP05HomePointsModelC* m_pDataModel;
	};

	
	class DCP05SelectCoordinateSystemC:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:
			enum eCtrlId
			{
				eLineInfo1=1,
				eLineInfo2,
				eLineInfo3
			};
		
		   DCP05SelectCoordinateSystemC();
			
            // Description: Destructor
            virtual ~DCP05SelectCoordinateSystemC();

			virtual void OnInitDialog(void);
			virtual void OnDialogActivated();
    
			 virtual void OnF1Pressed();
			 virtual void OnF3Pressed();
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP05ModelC* GetDCP05Model() const;

			short get_selected_id();

	protected:
  			
			GUI::TextCtrlC* m_pLineInfo1;
			GUI::TextCtrlC* m_pLineInfo2;
			GUI::TextCtrlC* m_pLineInfo3;
	private:
			short m_iSelected;
	};

    class DCP05HomePointsModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP05HomePointsModelC();

            // Description: Destructor
            //
            virtual ~DCP05HomePointsModelC();
			
			S_POINT_BUFF home_points[MAX_HOME_POINTS];
			short iOldActiveCds;


    };

};

#endif // DCP_BASEGUI_HPP




