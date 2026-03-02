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

#ifndef DCP_SHAFTDLG_HPP
#define DCP_SHAFTDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
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
    class DCP06ModelC;
	class DCP06ShaftDlgC;
	class DCP06ShaftModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP06ShaftControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06ShaftControllerC(DCP06ModelC* pDCP06Model);
			~DCP06ShaftControllerC();

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
            DCP06ShaftControllerC( const DCP06ShaftControllerC& oDCP06ShaftController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06ShaftControllerC& operator=( const DCP06ShaftControllerC& oDCP06ShaftController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06ShaftDlgC* m_pDlg;
			DCP06ShaftModelC* m_pShaftModel;
			DCP06ModelC* m_pDCP06Model;
			
    };


    class DCP06ShaftDlgC:public GUI::StandardDialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
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

		   DCP06ShaftDlgC(DCP06ShaftModelC* pModel);

            // Description: Destructor
            virtual ~DCP06ShaftDlgC();

			virtual void OnInitDialog(void);

		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP06ModelC* GetDCP06Model() const;
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

			//DCP::DCP06ModelC* m_pDCP06Model;

			//OBS_DECLARE_EXECUTE(DCP06DomDlgC);

			// Description: add all controls
            

	private:
			//OBS::ObserverC m_pComboBoxObserver;
			//virtual void OnComboBoxChanged(int unNotifyCode, int ulParam2);

			StringC m_strXLine;
			StringC m_strYLine;
			StringC m_strZLine;
			StringC m_strMeasLine;
			DCP06ShaftModelC* m_pShaftModel;
	};
    class DCP06ShaftModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06ShaftModelC();

            // Description: Destructor
            //
            virtual ~DCP06ShaftModelC();
			
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




