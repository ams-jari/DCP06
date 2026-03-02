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

#ifndef DCP_DEFINEPLANEUSERDEFDLG_HPP
#define DCP_DEFINEPLANEUSERDEFDLG_HPP

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

#include <dcp06/orientation/DCP_DefinePlane.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
	class DCP05DefinePlaneUserDefDlgC;
    class DCP05ModelC;
	class DCP05DefinePlaneUserDefModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP05DefinePlaneUserDefControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05DefinePlaneUserDefControllerC(DCP05ModelC* pDCP05Model);
			~DCP05DefinePlaneUserDefControllerC();

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
            DCP05DefinePlaneUserDefControllerC( const DCP05DefinePlaneUserDefControllerC& oDCP05DefinePlaneUserDefController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05DefinePlaneUserDefControllerC& operator=( const DCP05DefinePlaneUserDefControllerC& oDCP05DefinePlaneUserDefController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05DefinePlaneUserDefDlgC* m_pDlg;
			DCP05ModelC* m_pDCP05Model;
    };


    class DCP05DefinePlaneUserDefDlgC:public GUI::StandardDialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				ePlane=1,
				ePlanePoints,
				ePoints
				/*
				ePlaneInfo1,
				ePlaneInfo2,
				ePlaneInfo3,
				ePlaneInfo4,
				*/
			};

		   DCP05DefinePlaneUserDefDlgC(DCP05ModelC* pDCP05Model);

            // Description: Destructor
            virtual ~DCP05DefinePlaneUserDefDlgC();

			virtual void OnInitDialog(void);

		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP05DefinePlaneUserDefModelC* GetDataModel() const;
		
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
			GUI::TextCtrlC* m_pPlanePoints;
			GUI::TextCtrlC* m_pPoints; 

			// Description: add all controls

			DCP05ModelC* m_pDCP05Model;
            
	private:

			StringC	m_strXYPlane;
			StringC	m_strZXPlane;
			StringC	m_strYZPlane;
			StringC m_sHZText;

			short set_hz_plane1(/*S_PLANE_BUFF *planes, DCP_PLANE_TYPE plane_type, short display,*/ short actualdesign);
			short set_horizontal_plane(/*plane_buff_ *planes, short DISPLAY, short PLANE_TYPE*/);
			short defined_points_count_in_plane(S_PLANE_BUFF *plane,short *lastpoint);
			short calc_plane(S_PLANE_BUFF *plane, short actdes);

	};
	
    class DCP05DefinePlaneUserDefModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP05DefinePlaneUserDefModelC();

            // Description: Destructor
            //
            virtual ~DCP05DefinePlaneUserDefModelC();

			DCP05DefinePlaneModelC* planeModel;

			
			S_POINT_BUFF userdef_measured_points[MAX_USERDEF_POINTS];
			short userdef_plane_points_no[20];

			S_SELECT_POINTS select_point_list[MAX_USERDEF_POINTS];
			/*
			short active_plane;
			short   active_line; 
			bool hz_plane;
			S_PLANE_BUFF	plane_buff[1];
			S_PLANE_BUFF	hz_plane_buff[1];
			short display;
			*/
    };
	
};

#endif // DCP_BASEGUI_HPP




