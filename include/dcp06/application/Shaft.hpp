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

#ifndef DCP_SHAFTDLG_HPP
#define DCP_SHAFTDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
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
    class Model;
	class ShaftDialog;
	class ShaftModel;

    // Description: Tabbed controller for the Hello World application
    class ShaftController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            ShaftController(Model* pModel);
			~ShaftController();

            // Description: Handle change of position values
           	virtual void OnF1Pressed(void);
			virtual void OnF2Pressed(void);
			virtual void OnF3Pressed(void);
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
            ShaftController( const ShaftController& oShaftController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            ShaftController& operator=( const ShaftController& oShaftController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            ShaftDialog* m_pDlg;
			ShaftModel* m_pShaftModel;
			Model* m_pModel;
			
    };


    class ShaftDialog:public GUI::StandardDialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eLine=1,
				eCircle,
				eLineInfo1,
				eLineInfo2,
				eLineInfo3
			};

		   ShaftDialog(ShaftModel* pModel);

            // Description: Destructor
            virtual ~ShaftDialog();

			virtual void OnInitDialog(void);

		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            Model* GetModel() const;
			virtual void RefreshControls();

			virtual void delete_line();
			virtual void x_line();
			virtual void y_line();
			virtual void z_line();
			virtual bool CalculateLineAfterMeas();

	protected:
			
			GUI::ComboLineCtrlC* m_pLine;
			GUI::ComboLineCtrlC* m_pCircle;
			GUI::TextCtrlC* m_pLineInfo1;
			GUI::TextCtrlC* m_pLineInfo2;
			GUI::TextCtrlC* m_pLineInfo3;

			//DCP::Model* m_pModel;

			//OBS_DECLARE_EXECUTE(Alignment321Dialog);

			// Description: add all controls
            

	private:
			//OBS::ObserverC m_pComboBoxObserver;
			//virtual void OnComboBoxChanged(int unNotifyCode, int ulParam2);

			StringC m_strXLine;
			StringC m_strYLine;
			StringC m_strZLine;
			StringC m_strMeasLine;
			ShaftModel* m_pShaftModel;
	};
    class ShaftModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            ShaftModel();

            // Description: Destructor
            //
            virtual ~ShaftModel();
			
			short active_line; 
			S_LINE_BUFF	line_buff[1];

			S_CIRCLE_BUFF shaft_circle_points[1];
			S_CIRCLE_BUFF shaft_circle_points_in_plane[1];
			double shaft_circle_cx;
			double shaft_circle_cy;
			double shaft_circle_cz;
			double shaft_circle_diameter;
			double shaft_circle_rms;
			double shaft_circle_vi;
			double shaft_circle_vj;
			double shaft_circle_vk;

			S_PLANE_BUFF calc_plane[1];
			short display;
			StringC sTitle;
			//double x;
			//double y;
			//double z;
			//double cx;
			//double cy;
			//double cz;
			//double diameter;
			//double rms;
			char pid[10];
			
			short calc_shaft();

			double centerOfCircleDist;
			double angleLines;
    };

};

#endif // DCP_BASEGUI_HPP




