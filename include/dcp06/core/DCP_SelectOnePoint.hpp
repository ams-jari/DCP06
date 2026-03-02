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

#ifndef DCP_SELECTONEPOINT_DLG_HPP
#define DCP_SELECTONEPOINT_DLG_HPP

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
	class DCP05SelectOnePointDlgC;
	class DCP05SelectOnePointModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP05SelectOnePointControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05SelectOnePointControllerC(DCP05ModelC *pDCP05Model);
			~DCP05SelectOnePointControllerC();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			virtual void OnF2Pressed();
			virtual void OnF5Pressed();
			virtual void OnF6Pressed();
			//virtual void OnSHF1Pressed();
			virtual void OnSHF2Pressed();
			virtual void OnSHF5Pressed();

            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			void OnActiveControllerClosed( int lCtrlID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP05SelectOnePointControllerC( const DCP05SelectOnePointControllerC& oDCP05SelectOnePointController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05SelectOnePointControllerC& operator=( const DCP05SelectOnePointControllerC& oDCP05SelectOnePointController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05SelectOnePointDlgC* m_pDCP05SelectOnePointDlg;
			DCP05ModelC* m_pDCP05Model;
    };


    class DCP05SelectOnePointDlgC:public GUI::TableDialogC, public GUI::ModelHandlerC, public OBS::CommandC
	{
	 public:
			/*
			enum CW_ColumnWidth
			{
				CW_No = GUI::ListConfigColsC::CW_5,
				CW_Point = GUI::ListConfigColsC::CW_10,
				CW_ActualDesign = GUI::ListConfigColsC::CW_5
				
			};
			*/
			enum CI_ColumnId
			{
				CI_No	=1,
				CI_Point,
				CI_ActualDesign
			};

		   DCP05SelectOnePointDlgC(DCP05ModelC *pDCP05Model);

            // Description: Destructor
            virtual ~DCP05SelectOnePointDlgC();

			virtual void OnInitDialog(void);

			virtual void OnF1Pressed(void);

			virtual void OnDialogActivated();

			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP05SelectOnePointModelC* GetDCP05Model() const;

			OBS::ObserverC m_pMultiColCtrlObserver;
			virtual void OnChanged(int unNotifyCode, int ulParam2);

		protected:
			
			GUI::ListMultiColCtrlC* poMultiColCtrl;	
			DCP05ModelC* m_pDCP05Model;

			OBS_DECLARE_EXECUTE(DCP05SelectOnePointDlgC);
		
		private:
				//S_SELECT_POINTS sel_points[MAX_POINTS_IN_FILE];
				StringC sActualSelected;
				StringC sActualNonSelected;
				StringC sDesignSelected;
				StringC sDesignNonSelected;

				//void DCP::DCP05SelectMultiPointsDlgC::remove_point_table(short sel);
				//void DCP::DCP05SelectMultiPointsDlgC::add_point_table(short sel, bool bActual);
 	};


		/*
		 public:

			enum eCtrlId
			{
				eUnit=1,
				eDecimals
			};

		   DCP05UnitDlgC();

            // Description: Destructor
            virtual ~DCP05UnitDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP::DCP05ModelC* GetDCP05Model() const;


	protected:
			
			GUI::ComboLineCtrlC* m_pUnit;
			GUI::ComboLineCtrlC* m_pDecimals;
			OBS_DECLARE_EXECUTE(DCP05UnitDlgC);

			// Description: add all controls
            virtual void RefreshControls();

	private:
			OBS::ObserverC m_pComboBoxObserver;
			virtual void OnComboBoxChanged(int unNotifyCode, int ulParam2);
			
	};*/

	class DCP05SelectOnePointModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP05SelectOnePointModelC();

            // Description: Destructor
            //
            virtual ~DCP05SelectOnePointModelC();
						
			short m_iPointsCount;
			short m_iCurrentPoint;
			short m_iDef;

			S_SELECT_POINTS points[MAX_POINTS_IN_FILE];
			//short nro_table[MAX_POINTS_IN_FILE][2];
			short iActual_or_design;
			short iSelectedNo;
			StringC sSelectedFile;
    };

};

#endif // DCP_BASEGUI_HPP




