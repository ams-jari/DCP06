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

#ifndef DCP_VIEWCDF_DLG_HPP
#define DCP_VIEWCDF_DLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_Common.hpp>
#include <dcp06/calculation/DCP_CalcDist.hpp>

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
	class DCP05ViewCdfDlgC;
	class DCP05ViewCdfModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP05ViewCdfControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05ViewCdfControllerC(CdfFileFunc* pFileFunc,DCP05ModelC* pDCP05Model);
			~DCP05ViewCdfControllerC();

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
            DCP05ViewCdfControllerC( const DCP05ViewCdfControllerC& oDCP05ViewCdfController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05ViewCdfControllerC& operator=( const DCP05ViewCdfControllerC& oDCP05ViewCdfController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05ViewCdfDlgC* m_pDlg;
			CdfFileFunc* m_pFileFunc;
			DCP05ModelC* m_pDCP05Model;

			
    };


    class DCP05ViewCdfDlgC:public GUI::TableDialogC, public GUI::ModelHandlerC//, public OBS::CommandC
	{
	 public:
			/*
			enum CW_ColumnWidth
			{
				CW_No = GUI::ListConfigColsC::CW_5,
				CW_DistId = GUI::ListConfigColsC::CW_10,
				CW_Dist = GUI::ListConfigColsC::CW_10,
				CW_Note = GUI::ListConfigColsC::CW_10

			};
			*/
			enum CI_ColumnId
			{
				CI_No = 1,
				CI_DistId,
				CI_Dist,
				CI_Note
			};

		   DCP05ViewCdfDlgC(CdfFileFunc *pFileFunc,DCP05ModelC* pDCP05Model);

            // Description: Destructor
            virtual ~DCP05ViewCdfDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();

			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP05ModelC* GetDCP05Model() const;

			//OBS::ObserverC m_pMultiColCtrlObserver;
			//virtual void OnChanged(int unNotifyCode, int ulParam2);

			virtual void RefreshControls();

			bool DeletePoint();

			bool GetSelectedData(StringC& dDistId, StringC& sRefId, StringC& sNote);

		protected:
			
			GUI::ListMultiColCtrlC* poMultiColCtrl;	
			DCP05ModelC* m_pDCP05Model;

			//OBS_DECLARE_EXECUTE(DCP05SelectMultiPointsDlgC);
		
		private:
				//S_SELECT_POINTS sel_points[MAX_POINTS_IN_FILE];
				StringC sActualSelected;
				StringC sActualNonSelected;
				StringC sDesignSelected;
				StringC sDesignNonSelected;

				//void DCP::DCP05SelectMultiPointsDlgC::remove_point_table(short sel);
				//void DCP::DCP05SelectMultiPointsDlgC::add_point_table(short sel, bool bActual);

				short m_iSelectedCount;
				StringC m_strMaxPointSelected;
				DCP05CommonC* m_pCommon;
				CdfFileFunc* m_pFileFunc;
 	};
};

#endif // DCP_BASEGUI_HPP




