// ====================================================================================================================
//
// Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)
//
// Component: 321 Alignment (main orientation method)
//
// Summary  : 321 alignment dialog, controller, and model. Plane + Line + Reference point.
//            DCP9 Alignment321Bridge reference for database usage.
//
// ================================================================================================

#ifndef DCP_ALIGNMENT321_HPP
#define DCP_ALIGNMENT321_HPP

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Defs.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>

namespace DCP
{

	class Model;
	class Alignment321Model;
	class Alignment321Dialog;

	// 321 alignment controller
    class Alignment321Controller : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            Alignment321Controller();
			~Alignment321Controller();

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
            Alignment321Controller( const Alignment321Controller& oAlignment321Controller )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            Alignment321Controller& operator=( const Alignment321Controller& oAlignment321Controller )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            Alignment321Dialog* m_pDlg;
			Alignment321Model* m_pDataModel;
    };


    class Alignment321Dialog:public GUI::StandardDialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
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

		   Alignment321Dialog(Alignment321Model* pAlign321Model);

            // Description: Destructor
            virtual ~Alignment321Dialog();

			virtual void OnInitDialog(void);

		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            Model* GetModel() const;

			virtual void RefreshControls();

			virtual void delete_align321();

	protected:
			
			GUI::ComboLineCtrlC* m_pPlane;
			GUI::ComboLineCtrlC* m_pLine;
			GUI::ComboLineCtrlC* m_pPointOffs;
			GUI::ComboLineCtrlC* m_pPointMeas;
			GUI::ComboLineCtrlC* m_pRotPlane;
			GUI::ComboLineCtrlC* m_pCalc;
			GUI::ComboLineCtrlC* m_pRotLine;
			//OBS_DECLARE_EXECUTE(Alignment321Dialog);

			Alignment321Model* m_pDataModel;

	private:
			StringC sXLineText;
			StringC sYLineText;
			StringC sZLineText;
			StringC sXYPlaneText;
			StringC sZXPlaneText;
			StringC sYZPlaneText;
			StringC sHZText;
	};

	class Alignment321Model : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            Alignment321Model();
			//Alignment321Model(Alignment321Model* pModel);

            // Description: Destructor
            //
            virtual ~Alignment321Model();

			short /*DCP_PLANE_TYPE*/ align321_active_plane;
			short /*DCP_LINE_TYPE*/  align321_active_line; 
			bool align321_hz_plane;
			S_PLANE_BUFF	align321_plane_buff[1];
			S_PLANE_BUFF	align321_hz_plane_buff[1];
			S_LINE_BUFF		align321_line_buff[1];
			S_POINT_BUFF	align321_ovalues_buff;
			S_POINT_BUFF	align321_ovalues_tool_buff;
			S_POINT_BUFF	align321_ref_point_buff;
			S_POINT_BUFF	align321_rot_plane_buff;
			S_POINT_BUFF	align321_rot_line_buff;

			double matrix[4][4];
			double inv_matrix[4][4];
			bool calculated;
			bool ocsd_defined;
			short /*DCP_COORDINATE_SYSTEM*/ old_active_coodinate_system;
	
    };
	
};

#endif // DCP_ALIGNMENT321_HPP




