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

#ifndef DCP_DOMDLG_HPP
#define DCP_DOMDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>

#include <GUI_AppBase.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class DCP05ModelC;
	class DCP05DomModelC;
	class DCP05DomDlgC;

    // Description: Tabbed controller for the Hello World application
    class DCP05DomControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05DomControllerC();
			~DCP05DomControllerC();

            // Description: Handle change of position values
           	virtual void OnF1Pressed(void);
			virtual void OnF2Pressed(void);
			virtual void OnF3Pressed(void);
			virtual void OnF4Pressed(void);
			virtual void OnF5Pressed(void);
			virtual void OnF6Pressed(void);
			virtual void OnSHF2Pressed(void);
			virtual void OnSHF3Pressed(void);
			virtual void OnSHF4Pressed(void);

			
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int /*lExitCode*/ );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP05DomControllerC( const DCP05DomControllerC& oDCP05DomController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05DomControllerC& operator=( const DCP05DomControllerC& oDCP05DomController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05DomDlgC* m_pDlg;
			DCP05DomModelC* m_pDataModel;
    };


    class DCP05DomDlgC:public GUI::StandardDialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				ePlane=1,
				eLine,
				ePointOffs,
				ePointMeas,
				eRotPlane,
				eRotLine,
				eCalc
			};

		   DCP05DomDlgC(DCP05DomModelC* pDomModel);

            // Description: Destructor
            virtual ~DCP05DomDlgC();

			virtual void OnInitDialog(void);

		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP05ModelC* GetDCP05Model() const;

			virtual void RefreshControls();

			virtual void delete_dom();

	protected:
			
			GUI::ComboLineCtrlC* m_pPlane;
			GUI::ComboLineCtrlC* m_pLine;
			GUI::ComboLineCtrlC* m_pPointOffs;
			GUI::ComboLineCtrlC* m_pPointMeas;
			GUI::ComboLineCtrlC* m_pRotPlane;
			GUI::ComboLineCtrlC* m_pCalc;
			GUI::ComboLineCtrlC* m_pRotLine;
			//OBS_DECLARE_EXECUTE(DCP05DomDlgC);

			// Description: add all controls
			DCP05DomModelC* m_pDataModel;
            

	private:
			StringC sXLineText;
			StringC sYLineText;
			StringC sZLineText;
			StringC sXYPlaneText;
			StringC sZXPlaneText;
			StringC sYZPlaneText;
			StringC sHZText;
			
			//short /*DCP_COORDINATE_SYSTEM*/ m_eOldCoordinateSystem;
			//OBS::ObserverC m_pComboBoxObserver;
			//virtual void OnComboBoxChanged(int unNotifyCode, int ulParam2);
	};
		
   class DCP05DomModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP05DomModelC();
			//DCP05DomModelC(DCP05DomModelC* pModel);

            // Description: Destructor
            //
            virtual ~DCP05DomModelC();

			short /*DCP_PLANE_TYPE*/ dom_active_plane;
			short /*DCP_LINE_TYPE*/  dom_active_line; 
			bool dom_hz_plane;
			S_PLANE_BUFF	dom_plane_buff[1];
			S_PLANE_BUFF	dom_hz_plane_buff[1];
			S_LINE_BUFF		dom_line_buff[1];
			S_POINT_BUFF	dom_ovalues_buff;
			S_POINT_BUFF	dom_ovalues_tool_buff;
			S_POINT_BUFF	dom_ref_point_buff;
			S_POINT_BUFF	dom_rot_plane_buff;
			S_POINT_BUFF	dom_rot_line_buff;

			double matrix[4][4];
			double inv_matrix[4][4];
			bool calculated;
			bool ocsd_defined;
			short /*DCP_COORDINATE_SYSTEM*/ old_active_coodinate_system;
	
    };
	
};

#endif // DCP_BASEGUI_HPP




