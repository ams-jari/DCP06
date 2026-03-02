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

#ifndef DCP_VIEWAGF_DLG_HPP
#define DCP_VIEWAGF_DLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_Common.hpp>
#include <dcp06/calculation/DCP_CalcAngle.hpp>

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
    class DCP06ModelC;
	class DCP06ViewAgfDlgC;
	class DCP06ViewAgfModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP06ViewAgfControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06ViewAgfControllerC(AgfFileFunc* pFileFunc,DCP06ModelC* pDCP06Model);
			~DCP06ViewAgfControllerC();

            // Description: Handle change of position values
			virtual void OnF1Pressed();
			virtual void OnSHF5Pressed();
           	virtual void OnF6Pressed();
		
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			void OnActiveControllerClosed( int lCtrlID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP06ViewAgfControllerC( const DCP06ViewAgfControllerC& oDCP05ViewAgfController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06ViewAgfControllerC& operator=( const DCP06ViewAgfControllerC& oDCP05ViewAgfController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06ViewAgfDlgC* m_pDlg;
			AgfFileFunc* m_pFileFunc;
			DCP06ModelC* m_pDCP06Model;
			
    };


    class DCP06ViewAgfDlgC:public GUI::TableDialogC, public GUI::ModelHandlerC//, public OBS::CommandC
	{
	 public:
			/*
			enum CW_ColumnWidth
			{
				CW_No = GUI::ListConfigColsC::CW_5,
				CW_AngleId = GUI::ListConfigColsC::CW_10,
				CW_Angle = GUI::ListConfigColsC::CW_10,
				CW_Note = GUI::ListConfigColsC::CW_10

			};
			*/
			enum CI_ColumnId
			{
				CI_No = 1,
				CI_AngleId,
				CI_Angle,
				CI_Note
			};

		   DCP06ViewAgfDlgC(AgfFileFunc *pFileFunc,DCP06ModelC* pDCP06Model);

            // Description: Destructor
            virtual ~DCP06ViewAgfDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();

			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP06ModelC* GetDCP06Model() const;

			//OBS::ObserverC m_pMultiColCtrlObserver;
			//virtual void OnChanged(int unNotifyCode, int ulParam2);

			virtual void RefreshControls();

			bool DeletePoint();

			bool GetSelectedData(StringC& dDistId, StringC& sRefId, StringC& sTrgtId, StringC& sNote);

		protected:
			
			GUI::ListMultiColCtrlC* poMultiColCtrl;	
			DCP06ModelC* m_pDCP06Model;

			//OBS_DECLARE_EXECUTE(DCP06SelectMultiPointsDlgC);
		
		private:
				//S_SELECT_POINTS sel_points[MAX_POINTS_IN_FILE];

				//void DCP::DCP06SelectMultiPointsDlgC::remove_point_table(short sel);
				//void DCP::DCP06SelectMultiPointsDlgC::add_point_table(short sel, bool bActual);

				short m_iSelectedCount;
				StringC m_strMaxPointSelected;
				DCP06CommonC* m_pCommon;
				AgfFileFunc* m_pFileFunc;
 	};
};

#endif // DCP_BASEGUI_HPP




