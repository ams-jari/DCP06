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

#ifndef DCP_LINEFIT_DLG_HPP
#define DCP_LINEFIT_DLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
#include <GUI_TextCtrl.hpp>
#include <dcp06/calculation/DCP_CalcLineFit.hpp>
#include <dcp06/core/DCP_Model.hpp>
#include <dcp06/orientation/DCP_DCP05Dom.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class DCP05ModelC;
	class DCP05LineFitDlgC;
	class DCP05LineFitModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP05LineFitControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP05LineFitControllerC(DCP05ModelC* pDCP05Model);
			~DCP05LineFitControllerC();

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
			DCP05LineFitControllerC( const DCP05LineFitControllerC& oDCP05LineFitController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05LineFitControllerC& operator=( const DCP05LineFitControllerC& oDCP05LineFitController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP05LineFitDlgC* m_pDlg;
			DCP05LineFitModelC* m_pLineFitModel;
			DCP05ModelC* m_pDCP05Model;
			DCP05CalcLineFitC* calcLineFit;
			short SaveLineFitting(char *fname);
			DCP05CommonC* common;
    };


    class DCP05LineFitDlgC:public GUI::StandardDialogC, public OBS::CommandC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eRefLine = 0,
				eLine,
				eUsedHeight,
				eEnteredHeight,
				eShiftLine,
				eShiftValue,
				eRotateLine,
				eRotateValue
			};

		   DCP05LineFitDlgC(DCP05LineFitModelC* pModel,DCP05CalcLineFitC* calcLineFit);

            // Description: Destructor
            virtual ~DCP05LineFitDlgC();

			virtual void OnInitDialog(void);

		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            DCP05ModelC* GetDCP05Model() const;
			virtual void RefreshControls();

			virtual void delete_line();
			virtual void x_line();
			virtual void y_line();
			virtual void z_line();
			virtual bool CalculateLineAfterMeas();

	protected:
			GUI::ComboLineCtrlC* m_pRefLine;
			GUI::ComboLineCtrlC* m_pLine;
			GUI::ComboLineCtrlC* m_pUsedHeight;
			GUI::ComboLineCtrlC* m_pEnteredHeight;
			GUI::ComboLineCtrlC* m_pShiftLine;
			GUI::ComboLineCtrlC* m_pShiftValue;
			GUI::ComboLineCtrlC* m_pRotateLine;
			GUI::ComboLineCtrlC* m_pRotateValue;

			OBS_DECLARE_EXECUTE(DCP05LineFitDlgC);

			//GUI::TextCtrlC* m_pLineInfo3;

			//DCP::DCP05ModelC* m_pDCP05Model;

			//OBS_DECLARE_EXECUTE(DCP05DomDlgC);

			// Description: add all controls
            

	private:
			//OBS::ObserverC m_pComboBoxObserver;
			//virtual void OnComboBoxChanged(int unNotifyCode, int ulParam2);

			OBS::ObserverC m_pUsedHeightObserver;
			OBS::ObserverC m_pShiftLineObserver;
			OBS::ObserverC m_pRotateLineObserver;

			OBS::ObserverC m_pManualHeightObserver;
			OBS::ObserverC m_pShiftValueObserver;
			OBS::ObserverC m_pRotateAngleObserver;
			
			OBS::ObserverC m_pRefLineObserver;

			virtual void OnValueChanged( int unNotifyCode,  int ulParam2);

			StringC m_strXLine;
			StringC m_strYLine;
			StringC m_strZLine;
			StringC m_strMeasLine;
			DCP05LineFitModelC* m_pLineFitModel;
			DCP05CalcLineFitC* m_pCalcLineFit;

	};
    class DCP05LineFitModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP05LineFitModelC();

            // Description: Destructor
            //
            virtual ~DCP05LineFitModelC();
			
			//short active_line; 
			S_LINE_BUFF	line_buff[1];
			S_POINT_BUFF points_buff[MAX_LINEFIT_POINTS];	
			S_LINE_FITTING_RESULTS linefit_results[MAX_LINEFIT_POINTS];
			double manualHeight;
			double shiftValue;
			double rotateAngle;
			
			int selectedRefLine;
			int selectedHeight;
			int selectedShift;
			int selectedRotate;
			StringC fileName;
			DCP05DomModelC* domModel;

			S_LINE_BUFF line_ocs[1];
			S_POINT_BUFF points_in_line[MAX_LINEFIT_POINTS];
    };
};

#endif // DCP_BASEGUI_HPP




