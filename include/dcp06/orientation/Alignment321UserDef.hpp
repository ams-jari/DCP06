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

#ifndef DCP_DOMUSERDEFDLG_HPP
#define DCP_DOMUSERDEFDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Defs.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>

#include <GUI_AppBase.hpp>
#include <dcp06/orientation/Alignment321.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class Model;
	class Alignment321UserDefModel;
	class Alignment321UserDefDialog;

    // Description: Tabbed controller for the Hello World application
    class Alignment321UserDefController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            Alignment321UserDefController(Model* pModel);
			~Alignment321UserDefController();

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
			virtual void OnSHF5Pressed(void);


			
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int /*lExitCode*/ );

			virtual void OnControllerActivated(void);

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            Alignment321UserDefController( const Alignment321UserDefController& oAlignment321UserDefController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            Alignment321UserDefController& operator=( const Alignment321UserDefController& oAlignment321UserDefController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            Alignment321UserDefDialog* m_pDlg;
			Alignment321UserDefModel* m_pDataModel;

			void show_function_keys();
			void open_meas_display(); 
			Model* m_pModel;
    };


    class Alignment321UserDefDialog:public GUI::StandardDialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
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

		   Alignment321UserDefDialog(Alignment321UserDefModel* pDomModel);

            // Description: Destructor
            virtual ~Alignment321UserDefDialog();

			virtual void OnInitDialog(void);

		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            Model* GetModel() const;

			virtual void RefreshControls();

			virtual void delete_dom();
			void load_data_from_dcp05model();

	protected:
			
			GUI::ComboLineCtrlC* m_pPlane;
			GUI::ComboLineCtrlC* m_pLine;
			GUI::ComboLineCtrlC* m_pPointOffs;
			GUI::ComboLineCtrlC* m_pPointMeas;
			GUI::ComboLineCtrlC* m_pRotPlane;
			GUI::ComboLineCtrlC* m_pCalc;
			GUI::ComboLineCtrlC* m_pRotLine;
			//OBS_DECLARE_EXECUTE(Alignment321Dialog);

			// Description: add all controls
			Alignment321UserDefModel* m_pDataModel;
            

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
			//virtual void OnComboBoxChanged(int unNotifyCode,  int ulParam2);
	};

   class Alignment321UserDefModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            Alignment321UserDefModel(Model* pModel);
			//Alignment321Model(Alignment321Model* pModel);

            // Description: Destructor
            //
            virtual ~Alignment321UserDefModel();
			/*
			short dom_active_plane;
			short   dom_active_line; 
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
			short  old_active_coodinate_system;
			*/
			Alignment321Model* domModel;

			S_POINT_BUFF userdef_measured_points[MAX_USERDEF_POINTS];
			short userdef_plane_points_no[20];
			short userdef_line_points_no[20];
			short userdef_point_no;

			S_SELECT_POINTS select_point_list[MAX_USERDEF_POINTS];

			short get_measured_points_count();	

			short build_select_point_list();
			short copy_measured_points();
			bool is_point_id_defined();
			short get_last_defined_point();

	private:
		Model* m_pModel;
	
    };
};

#endif // DCP_BASEGUI_HPP




