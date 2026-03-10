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
	class Model;
	class CircleDialog;
	class CircleModel;


    // Description: Tabbed controller for the Hello World application
    class CircleController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            CircleController(Model *pModel, short iDisplay = 0);
			~CircleController();

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
            CircleController( const CircleController& oCircleController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            CircleController& operator=( const CircleController& oCircleController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            CircleDialog* m_pDlg;
			Model* m_pModel;
			CircleModel* m_pDataModel;

			short PLANE_KEYS;
			void set_function_keys();
			short m_iDisplay;

    };


	class CircleDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC, public OBS::CommandC
	{
		 public:

			enum eCtrlId
			{
				eDefinePlaneInfo=1,
				eCircleId,
				ePlane,
				eToolRadius,
				eMeasureCirclePoints,
				eCirclePoints
			};

		   CircleDialog(Model *pModel, CircleModel* pCircleModel, short iDisplay);

            // Description: Destructor
            virtual ~CircleDialog();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			PointBuffModel* GetPointBuffModelModel() const;
			bool GetCircleIdString(char* buf, size_t bufLen) const;
			
	protected:
  			
			Model * m_pModel;
			
			GUI::TextCtrlC*  m_pDefinePlaneInfo;
			GUI::ComboLineCtrlC* m_pCircleId;
			GUI::ComboLineCtrlC* m_pPlane;
			GUI::ComboLineCtrlC* m_pToolRadius;
			GUI::TextCtrlC*  m_pMeasureCirclePoints;
			GUI::ComboLineCtrlC* m_pCirclePoints;

			OBS_DECLARE_EXECUTE(CircleDialog);

	private:
			CircleModel* m_pDataModel;

			StringC sXY_plane, sZX_plane, sYZ_plane, sCIRCLEPOINTS_plane, sMeasPlane;

			OBS::ObserverC m_pToolRadiusObserver;
			virtual void OnValueChanged( int unNotifyCode, int ulParam2);

			short m_iDisplay;
	};

	 class CircleModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            CircleModel(Model* pModel);

            // Description: Destructor
            virtual ~CircleModel();

			void clear_circle();
			void delete_circle();
			short show_circle_points();
			Model* m_pModel;
			Common* pCommon;
			MsgBox* pMsgBox;
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




