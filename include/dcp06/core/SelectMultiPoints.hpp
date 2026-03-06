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

#ifndef DCP_SELECTMULTIPOINTS_DLG_HPP
#define DCP_SELECTMULTIPOINTS_DLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/Common.hpp>

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
	class DCP06SelectMultiPointsDlgC;
	class DCP06SelectMultiPointsModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP06SelectMultiPointsControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06SelectMultiPointsControllerC(DCP06ModelC *pDCP06Model);
			~DCP06SelectMultiPointsControllerC();

            // Description: Handle change of position values
           	virtual void OnF6Pressed();
		
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			void OnActiveControllerClosed( int lCtrlID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP06SelectMultiPointsControllerC( const DCP06SelectMultiPointsControllerC& oDCP06SelectMultiPointsController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06SelectMultiPointsControllerC& operator=( const DCP06SelectMultiPointsControllerC& oDCP06SelectMultiPointsController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06SelectMultiPointsDlgC* m_pDlg;
			DCP06ModelC* m_pDCP06Model;
			
    };


    class DCP06SelectMultiPointsDlgC:public GUI::TableDialogC, public GUI::ModelHandlerC, public OBS::CommandC
	{
	 public:
			/*
			enum CW_ColumnWidth
			{
				CW_Point = GUI::ListConfigColsC::CW_10,
				CW_ActualDesign = GUI::ListConfigColsC::CW_5,
				CW_Selected = GUI::ListConfigColsC::CW_5
			};
			*/
			enum CI_ColumnId
			{
				CI_Point = 1,
				CI_ActualDesign,
				CI_Selected
			};

		   DCP06SelectMultiPointsDlgC(DCP06ModelC *pDCP06Model);

            // Description: Destructor
            virtual ~DCP06SelectMultiPointsDlgC();

			virtual void OnInitDialog(void);

			virtual void OnF1Pressed(void);

			virtual void OnDialogActivated();

			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP06SelectMultiPointsModelC* GetDataModel() const;

			OBS::ObserverC m_pMultiColCtrlObserver;
			virtual void OnChanged( int unNotifyCode,  int ulParam2);

			virtual void RefreshControls();

		protected:
			
			GUI::ListMultiColCtrlC* poMultiColCtrl;	
			DCP06ModelC* m_pDCP06Model;

			OBS_DECLARE_EXECUTE(DCP06SelectMultiPointsDlgC);
		
		private:
				//S_SELECT_POINTS sel_points[MAX_POINTS_IN_FILE];
				StringC sActualSelected;
				StringC sActualNonSelected;
				StringC sDesignSelected;
				StringC sDesignNonSelected;

				void remove_point_table(short sel);
				void add_point_table(short sel, bool bActual);

				short m_iSelectedCount;
				StringC m_strMaxPointSelected;
				DCP06CommonC* m_pCommon;
 	};


	class DCP06SelectMultiPointsModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06SelectMultiPointsModelC();

            // Description: Destructor
            //
            virtual ~DCP06SelectMultiPointsModelC();
						
			short m_iMinPoint;
			short m_iMaxPoint;
			short m_iPointsCount;
			short m_iCurrentPoint;
			short m_iDef;
			StringC sSelectedFile;

			S_SELECT_POINTS sel_points[MAX_POINTS_IN_FILE];
			short nro_table[MAX_POINTS_IN_FILE][2];

			StringC sTitle;
			StringC sInfo;
			RSC::TxtC::TOKEN helpToken;
			
    };

};

#endif // DCP_BASEGUI_HPP




