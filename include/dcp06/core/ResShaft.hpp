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

#ifndef DCP_RES_SHAFT_DLG_HPP
#define DCP_RES_SHAFT_DLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/application/Shaft.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/measurement/Circle.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_TableDialog.hpp>
#include <GUI_ListMultiColCtrl.hpp>
#include <GUI_AppBase.hpp>
//#include <GUI_Timer.hpp>
// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class DCP06ModelC;
	class DCP06ResShaftDlgC;
	//class DCP06ResShaftModelC;
	//class DCP06SelectOnePointModelC;

    // Description: Tabbed controller for the Hello World application
	
    class DCP06ResShaftControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
			DCP06ResShaftControllerC(DCP06ModelC *pDCP06Model, DCP06ShaftModelC *pShaftModel);
			~DCP06ResShaftControllerC();

            // Description: Handle change of position values
            //virtual void OnF1Pressed();
			//virtual void OnF2Pressed();
			virtual void OnF3Pressed();
			//virtual void OnF5Pressed();
			virtual void OnF6Pressed();
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
            DCP06ResShaftControllerC( const DCP06ResShaftControllerC& oDCP06ResShaftController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
DCP06ResShaftControllerC& operator=( const DCP06ResShaftControllerC& oDCP06ResShaftController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06ResShaftDlgC* m_pDlg;
			DCP06ModelC* m_pDCP06Model;
			DCP06ShaftModelC* m_pShaftModel;
    };
	

	class DCP06ResShaftDlgC: public GUI::TableDialogC,public GUI::ModelHandlerC//, public OBS::CommandC
	{
	 public:

			 enum eCtrlId
			{
				eInfo=1,
				eMulti=1,
				eDist=2,
				eAngle=3
			 };

			/* VIVA
			enum CW_ColumnWidth
			{
				CW_No = GUI::ListConfigColsC::CW_5,
				CW_Point = GUI::ListConfigColsC::CW_10,
				CW_Deviation = GUI::ListConfigColsC::CW_5
				
			};
			*/
			enum CI_ColumnId
			{
				CI_No	=1,
				CI_Point,
				CI_Deviation
			};
			enum CI_ColumnIdDist
			{
				CI_Text	=1,
				CI_Dist
			};
			enum CI_ColumnIdAngle
			{
					CI_Angle=2
		
			};
		   DCP06ResShaftDlgC(DCP06ModelC *pDCP06Model, DCP06ShaftModelC *pShaftModel);

            // Description: Destructor
            virtual ~DCP06ResShaftDlgC();

			virtual void OnInitDialog(void);

			//virtual void OnF1Pressed(void);
			//virtual void OnF6Pressed();

			virtual void OnDialogActivated();
			virtual void RefreshControls();

			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP06ModelC* GetDataModel() const;

			//OBS::ObserverC m_pMultiColCtrlObserver;
			//virtual void OnChanged(int unNotifyCode, int ulParam2);

		protected:
			GUI::TextCtrlC* m_pInfo1;
			//GUI::ComboLineCtrlC* m_pDist;
			GUI::ListMultiColCtrlC* poMultiColCtrl;	
			//GUI::ListMultiColCtrlC* poMultiColCtrlDist;	
			//GUI::ListMultiColCtrlC* poMultiColCtrlAngle;	
			DCP06ModelC* m_pDCP06Model;
			DCP06ShaftModelC* m_pShaftModel;
			// Description: add all controls
       

			//OBS_DECLARE_EXECUTE(DCP06SelectOnePointDlgC);
		
		private:
				//S_SELECT_POINTS sel_points[MAX_POINTS_IN_FILE];
				StringC sActualSelected;
				StringC sActualNonSelected;
				StringC sDesignSelected;
				StringC sDesignNonSelected;
				//virtual void OnTimer(void);

				DCP06CommonC* m_pCommon;
				double calc_pdist(short pno);
				StringC sTitle;
				

				//void DCP::DCP06SelectMultiPointsDlgC::remove_point_table(short sel);
				//void DCP::DCP06SelectMultiPointsDlgC::add_point_table(short sel, bool bActual);
 	};

	/*
	class DCP06ResShaftModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06ResShaftModelC();

            // Description: Destructor
            //
            virtual ~DCP06ResShaftModelC();
			S_POINT_BUFF shaft_res_points[MAX_POINTS_IN_CIRCLE];
    };
	*/
};

#endif // DCP_BASEGUI_HPP




