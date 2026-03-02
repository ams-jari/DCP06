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

#ifndef DCP_CIRCLEDLG_HPP
#define DCP_CIRCLEDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_PointBuffModel.hpp>
#include <dcp06/core/DCP_MsgBox.hpp>
#include <dcp06/core/DCP_Common.hpp>

#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>

// Description:
//
namespace DCP
{
    // Forward declaration
	class DCP05ModelC;
	class DCP05CircleDlgC;
	class DCP05CircleModelC;


    // Description: Tabbed controller for the Hello World application
    class DCP05CircleControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05CircleControllerC(DCP05ModelC *pDCP05Model, short iDisplay = 0);
			~DCP05CircleControllerC();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			virtual void OnF2Pressed();
			virtual void OnF3Pressed();
			virtual void OnF4Pressed();
			virtual void OnF5Pressed();
			virtual void OnF6Pressed();
			virtual void OnSHF2Pressed();
			virtual void OnSHF3Pressed();
			// Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			virtual bool ConfirmClose(bool bEsc);
			//virtual bool Close(int lExitCode, bool bConfirmation);


			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP05CircleControllerC( const DCP05CircleControllerC& oDCP05CircleController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05CircleControllerC& operator=( const DCP05CircleControllerC& oDCP05CircleController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05CircleDlgC* m_pDlg;
			DCP05ModelC* m_pDCP05Model;
			DCP05CircleModelC* m_pDataModel;

			short PLANE_KEYS;
			void set_function_keys();
			short m_iDisplay;

    };


	class DCP05CircleDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC, public OBS::CommandC
	{
		 public:

			enum eCtrlId
			{
				eDefinePlaneInfo=1,
				ePlane,
				eToolRadius,
				eMeasureCirclePoints,
				eCirclePoints
			};

		   DCP05CircleDlgC(DCP05ModelC *pDCP05Model, DCP05CircleModelC* pCircleModel, short iDisplay);

            // Description: Destructor
            virtual ~DCP05CircleDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP05PointBuffModelC* GetPointBuffModelModel() const;
			
	protected:
  			
			DCP05ModelC * m_pDCP05Model;
			
			GUI::TextCtrlC*  m_pDefinePlaneInfo;
			GUI::ComboLineCtrlC* m_pPlane;
			GUI::ComboLineCtrlC* m_pToolRadius;
			GUI::TextCtrlC*  m_pMeasureCirclePoints;
			GUI::ComboLineCtrlC* m_pCirclePoints;

			OBS_DECLARE_EXECUTE(DCP05CircleDlgC);

	private:
			DCP05CircleModelC* m_pDataModel;

			StringC sXY_plane, sZX_plane, sYZ_plane, sCIRCLEPOINTS_plane, sMeasPlane;

			OBS::ObserverC m_pToolRadiusObserver;
			virtual void OnValueChanged( int unNotifyCode, int ulParam2);

			short m_iDisplay;
	};

	 class DCP05CircleModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            DCP05CircleModelC(DCP05ModelC* pDCP05Model);

            // Description: Destructor
            virtual ~DCP05CircleModelC();

			void delete_circle();
			short show_circle_points();
			DCP05ModelC* m_pDCP05Model;
			DCP05CommonC* pCommon;
			DCP05MsgBoxC* pMsgBox;
			StringC sTitle;
			S_PLANE_BUFF temp_plane_table[1];
			S_PLANE_BUFF planes[1];
			S_CIRCLE_BUFF circle_points[1];
			S_CIRCLE_BUFF circle_points_in_plane[1];
			short PLANE_TYPE;
			double cx,cy,cz, diameter;
			double vi,vj,vk;
			double bR;
			double rms_diameter;
			short max_dev_in_point;
			double ccs_matrix[4][4];
			double ccs_inv_matrix[4][4];
			short m_iCounts;
    };
	
};

#endif // DCP_BASEGUI_HPP




