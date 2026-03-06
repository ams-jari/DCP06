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

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Common.hpp>
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
	class HomePointsDialog;
	class HomePointsModel;
	class SelectCoordinateSystem;


    // Description: Tabbed controller for the Hello World application
    class HomePointsController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            HomePointsController(Model* pModel);
			~HomePointsController();

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
            HomePointsController( const HomePointsController& oHomePointsController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            HomePointsController& operator=( const HomePointsController& oHomePointsController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            HomePointsDialog* m_pDlg;
			HomePointsModel* m_pDataModel;
			Model* m_pModel;
			Common* m_pCommon;
    };


	class HomePointsDialog:public GUI::TableDialogC, public GUI::ModelHandlerC
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


		   HomePointsDialog(HomePointsModel* pHomePointsModel);

            // Description: Destructor
            virtual ~HomePointsDialog();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			Model* GetModel() const;

			short get_selected_id();

	protected:
  			
			GUI::ListMultiColCtrlC* poMultiColCtrl;	

			StringC sPointNo;
			StringC sPointId;
			StringC sPointStatus;

			StringC sDCS;
			StringC sOCS;
	private:
			
			HomePointsModel* m_pDataModel;
	};

	
	class SelectCoordinateSystem:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:
			enum eCtrlId
			{
				eLineInfo1=1,
				eLineInfo2,
				eLineInfo3
			};
		
		   SelectCoordinateSystem();
			
            // Description: Destructor
            virtual ~SelectCoordinateSystem();

			virtual void OnInitDialog(void);
			virtual void OnDialogActivated();
    
			 virtual void OnF1Pressed();
			 virtual void OnF3Pressed();
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			Model* GetModel() const;

			short get_selected_id();

	protected:
  			
			GUI::TextCtrlC* m_pLineInfo1;
			GUI::TextCtrlC* m_pLineInfo2;
			GUI::TextCtrlC* m_pLineInfo3;
	private:
			short m_iSelected;
	};

    class HomePointsModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            HomePointsModel();

            // Description: Destructor
            //
            virtual ~HomePointsModel();
			
			S_POINT_BUFF home_points[MAX_HOME_POINTS];
			short iOldActiveCds;


    };

};

#endif // DCP_BASEGUI_HPP




