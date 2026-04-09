// ====================================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: 
//
// $Workfile: HEW_DCP05GUI.hpp $
//
// Summary  : 
//
// ------------------------------------------------------------------------------------------------
//
// Copyright (c) AMS. Based on Leica Captivate plugin framework.
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
	class SelectOnePointDialog;
	class SelectOnePointModel;

    // Description: Tabbed controller for the Hello World application
    class SelectOnePointController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            SelectOnePointController(Model *pModel);
			~SelectOnePointController();

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
            SelectOnePointController( const SelectOnePointController& oSelectOnePointController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            SelectOnePointController& operator=( const SelectOnePointController& oSelectOnePointController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            SelectOnePointDialog* m_pSelectOnePointDlg;
			Model* m_pModel;
    };


    class SelectOnePointDialog:public GUI::TableDialogC, public GUI::ModelHandlerC, public OBS::CommandC
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

		   SelectOnePointDialog(Model *pModel);

            // Description: Destructor
            virtual ~SelectOnePointDialog();

			virtual void OnInitDialog(void);

			virtual void OnF1Pressed(void);

			virtual void OnDialogActivated();

			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            SelectOnePointModel* GetModel() const;

			OBS::ObserverC m_pMultiColCtrlObserver;
			virtual void OnChanged(int unNotifyCode, int ulParam2);

		protected:
			
			GUI::ListMultiColCtrlC* poMultiColCtrl;	
			Model* m_pModel;

			OBS_DECLARE_EXECUTE(SelectOnePointDialog);
		
		private:
				//S_SELECT_POINTS sel_points[MAX_POINTS_IN_FILE];
				StringC sActualSelected;
				StringC sActualNonSelected;
				StringC sDesignSelected;
				StringC sDesignNonSelected;

				//void DCP::SelectMultiPointsDialog::remove_point_table(short sel);
				//void DCP::SelectMultiPointsDialog::add_point_table(short sel, bool bActual);
 	};


		/*
		 public:

			enum eCtrlId
			{
				eUnit=1,
				eDecimals
			};

		   UnitDialog();

            // Description: Destructor
            virtual ~UnitDialog();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP::Model* GetModel() const;


	protected:
			
			GUI::ComboLineCtrlC* m_pUnit;
			GUI::ComboLineCtrlC* m_pDecimals;
			OBS_DECLARE_EXECUTE(UnitDialog);

			// Description: add all controls
            virtual void RefreshControls();

	private:
			OBS::ObserverC m_pComboBoxObserver;
			virtual void OnComboBoxChanged(int unNotifyCode, int ulParam2);
			
	};*/

	class SelectOnePointModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            SelectOnePointModel();

            // Description: Destructor
            //
            virtual ~SelectOnePointModel();
						
			short m_iPointsCount;
			short m_iCurrentPoint;
			short m_iDef;

			S_SELECT_POINTS points[MAX_POINTS_IN_FILE];
			//short nro_table[MAX_POINTS_IN_FILE][2];
			short iActual_or_design;
			short iSelectedNo;
			StringC sSelectedFile;
			/// When true, LIST was built with design-values-only filter (must match getPointByIndexForList).
			bool m_listDesignValuesOnly;
    };

};

#endif // DCP_BASEGUI_HPP




