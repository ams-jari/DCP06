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

#ifndef DCP_DEFINEPLANEDLG_HPP
#define DCP_DEFINEPLANEDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/orientation/DCP_DCP05Dom.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
#include <GUI_TextCtrl.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class DCP05ModelC;
	class DCP05DefinePlaneDlgC;
	class DCP05DefinePlaneModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP05DefinePlaneControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05DefinePlaneControllerC(DCP05ModelC* pDCP05Model);
			~DCP05DefinePlaneControllerC();

            // Description: Handle change of position values
           	virtual void OnF1Pressed(void);
			virtual void OnF2Pressed(void);
			virtual void OnF3Pressed(void);
			virtual void OnF4Pressed(void);
			virtual void OnF5Pressed(void);
			virtual void OnF6Pressed(void);
			virtual void OnSHF2Pressed(void);
			
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP05DefinePlaneControllerC( const DCP05DefinePlaneControllerC& oDCP05DefinePlaneController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05DefinePlaneControllerC& operator=( const DCP05DefinePlaneControllerC& oDCP05DefinePlaneController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05DefinePlaneDlgC* m_pDlg;
			DCP05ModelC* m_pDCP05Model;
    };


    class DCP05DefinePlaneDlgC:public GUI::StandardDialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				ePlane=1,
				ePlaneInfo1,
				ePlaneInfo2,
				ePlaneInfo3,
				ePlaneInfo4,
			};

		   DCP05DefinePlaneDlgC(DCP05ModelC* pDCP05Model);

            // Description: Destructor
            virtual ~DCP05DefinePlaneDlgC();

			virtual void OnInitDialog(void);

		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP05DefinePlaneModelC* GetDataModel() const;
		
			virtual void RefreshControls();

			virtual void xy_plane();
			virtual void zx_plane();
			virtual void yz_plane();
			virtual short hz_plane();
			virtual void delete_plane();
			virtual void MeasPressed();
			virtual bool CalculatePlaneAfterMeas();

	protected:
			
			GUI::ComboLineCtrlC* m_pPlane;
			GUI::TextCtrlC* m_pPlaneInfo1;
			GUI::TextCtrlC* m_pPlaneInfo2;
			GUI::TextCtrlC* m_pPlaneInfo3;
			GUI::TextCtrlC* m_pPlaneInfo4;
			//OBS_DECLARE_EXECUTE(DCP05DomDlgC);

			// Description: add all controls

			DCP05ModelC* m_pDCP05Model;
			//DCP::DCP05DefinePlaneModelC* m_pTempDefinePlaneModel;
            
	private:
			//OBS::ObserverC m_pComboBoxObserver;
			//virtual void OnComboBoxChanged(int unNotifyCode, int ulParam2);

			StringC	m_strXYPlane;
			StringC	m_strZXPlane;
			StringC	m_strYZPlane;
			StringC m_sHZText;

			short set_hz_plane1(/*S_PLANE_BUFF *planes, DCP_PLANE_TYPE plane_type, short display,*/ short actualdesign);
			short set_horizontal_plane(/*plane_buff_ *planes, short DISPLAY, short PLANE_TYPE*/);
			short defined_points_count_in_plane(S_PLANE_BUFF *plane,short *lastpoint);
			short calc_plane(S_PLANE_BUFF *plane, short actdes);

	};
    class DCP05DefinePlaneModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP05DefinePlaneModelC();

            // Description: Destructor
            //
            virtual ~DCP05DefinePlaneModelC();

			short /*DCP_PLANE_TYPE*/ active_plane;
			short /*DCP_LINE_TYPE*/  active_line; 
			bool hz_plane;
			S_PLANE_BUFF	plane_buff[1];
			S_PLANE_BUFF	hz_plane_buff[1];
			short display;
    };
};

#endif // DCP_BASEGUI_HPP




