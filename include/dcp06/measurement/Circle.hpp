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

#include <dcp06/core/Types.hpp>
#include <dcp06/core/PointBuffModel.hpp>
#include <dcp06/core/MsgBox.hpp>
#include <dcp06/core/Common.hpp>

#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>

// Description:
//
namespace DCP
{
    // Forward declaration
	class DCP06ModelC;
	class DCP06CircleDlgC;
	class DCP06CircleModelC;


    // Description: Tabbed controller for the Hello World application
    class DCP06CircleControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06CircleControllerC(DCP06ModelC *pDCP06Model, short iDisplay = 0);
			~DCP06CircleControllerC();

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
            DCP06CircleControllerC( const DCP06CircleControllerC& oDCP05CircleController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06CircleControllerC& operator=( const DCP06CircleControllerC& oDCP05CircleController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06CircleDlgC* m_pDlg;
			DCP06ModelC* m_pDCP06Model;
			DCP06CircleModelC* m_pDataModel;

			short PLANE_KEYS;
			void set_function_keys();
			short m_iDisplay;

    };


	class DCP06CircleDlgC:public GUI::StandardDialogC, public GUI::ModelHandlerC, public OBS::CommandC
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

		   DCP06CircleDlgC(DCP06ModelC *pDCP06Model, DCP06CircleModelC* pCircleModel, short iDisplay);

            // Description: Destructor
            virtual ~DCP06CircleDlgC();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			DCP06PointBuffModelC* GetPointBuffModelModel() const;
			
	protected:
  			
			DCP06ModelC * m_pDCP06Model;
			
			GUI::TextCtrlC*  m_pDefinePlaneInfo;
			GUI::ComboLineCtrlC* m_pPlane;
			GUI::ComboLineCtrlC* m_pToolRadius;
			GUI::TextCtrlC*  m_pMeasureCirclePoints;
			GUI::ComboLineCtrlC* m_pCirclePoints;

			OBS_DECLARE_EXECUTE(DCP06CircleDlgC);

	private:
			DCP06CircleModelC* m_pDataModel;

			StringC sXY_plane, sZX_plane, sYZ_plane, sCIRCLEPOINTS_plane, sMeasPlane;

			OBS::ObserverC m_pToolRadiusObserver;
			virtual void OnValueChanged( int unNotifyCode, int ulParam2);

			short m_iDisplay;
	};

	 class DCP06CircleModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            DCP06CircleModelC(DCP06ModelC* pDCP06Model);

            // Description: Destructor
            virtual ~DCP06CircleModelC();

			void delete_circle();
			short show_circle_points();
			DCP06ModelC* m_pDCP06Model;
			DCP06CommonC* pCommon;
			DCP06MsgBoxC* pMsgBox;
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




