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
#include <dcp06/orientation/DCP_DCP06Alignment321.hpp>

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class DCP06ModelC;
	class DCP06LineFitDlgC;
	class DCP06LineFitModelC;

    // Description: Tabbed controller for the Hello World application
    class DCP06LineFitControllerC : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            DCP06LineFitControllerC(DCP06ModelC* pDCP06Model);
			~DCP06LineFitControllerC();

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
			DCP06LineFitControllerC( const DCP06LineFitControllerC& oDCP05LineFitController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06LineFitControllerC& operator=( const DCP06LineFitControllerC& oDCP05LineFitController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            DCP06LineFitDlgC* m_pDlg;
			DCP06LineFitModelC* m_pLineFitModel;
			DCP06ModelC* m_pDCP06Model;
			DCP06CalcLineFitC* calcLineFit;
			short SaveLineFitting(char *fname);
			DCP06CommonC* common;
    };


    class DCP06LineFitDlgC:public GUI::StandardDialogC, public OBS::CommandC, public GUI::ModelHandlerC
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

		   DCP06LineFitDlgC(DCP06LineFitModelC* pModel,DCP06CalcLineFitC* calcLineFit);

            // Description: Destructor
            virtual ~DCP06LineFitDlgC();

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
			GUI::ComboLineCtrlC* m_pRefLine;
			GUI::ComboLineCtrlC* m_pLine;
			GUI::ComboLineCtrlC* m_pUsedHeight;
			GUI::ComboLineCtrlC* m_pEnteredHeight;
			GUI::ComboLineCtrlC* m_pShiftLine;
			GUI::ComboLineCtrlC* m_pShiftValue;
			GUI::ComboLineCtrlC* m_pRotateLine;
			GUI::ComboLineCtrlC* m_pRotateValue;

			OBS_DECLARE_EXECUTE(DCP06LineFitDlgC);

			//GUI::TextCtrlC* m_pLineInfo3;

			//DCP::DCP06ModelC* m_pDCP06Model;

			//OBS_DECLARE_EXECUTE(DCP06DomDlgC);

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
			DCP06LineFitModelC* m_pLineFitModel;
			DCP06CalcLineFitC* m_pCalcLineFit;

	};
    class DCP06LineFitModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06LineFitModelC();

            // Description: Destructor
            //
            virtual ~DCP06LineFitModelC();
			
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
			DCP06DomModelC* domModel;

			S_LINE_BUFF line_ocs[1];
			S_POINT_BUFF points_in_line[MAX_LINEFIT_POINTS];
    };
};

#endif // DCP_BASEGUI_HPP




