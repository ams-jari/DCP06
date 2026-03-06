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

#ifndef DCP_CALCDIST2POINTSDLG_HPP
#define DCP_CALCDIST2POINTSDLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Model.hpp>
#include <dcp06/file/AdfFileFunc.hpp>
#include <dcp06/core/Common.hpp>

#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp> // CAPTIVATE
#include <GUI_AppBase.hpp>
#include <GUI_TextCtrl.hpp>
//#include <GUI_MultiTextValueCtrl.hpp> CAPTIVATe

// Description: The Hello World application
//
namespace DCP
{

    // Forward declaration
    class Model;
	class CalculationDist2PointsDialog;
	class CalculationDist2PointsModel;

    // Description: Tabbed controller for the Hello World application
    class CalculationDist2PointsController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
            CalculationDist2PointsController(Model* pModel);
			~CalculationDist2PointsController();

            // Description: Handle change of position values
           	virtual void OnF1Pressed(void);
			virtual void OnF6Pressed(void);
			
            // Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );

        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            CalculationDist2PointsController( const CalculationDist2PointsController& oCalculationDist2PointsController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            CalculationDist2PointsController& operator=( const CalculationDist2PointsController& oCalculationDist2PointsController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

            CalculationDist2PointsDialog* m_pDlg;
			Model* m_pModel;
    };


    class CalculationDist2PointsDialog:public GUI::DialogC/*, public OBS::CommandC*/, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				ePoints=1,
				eHeader,
				eX,
				eY,
				eZ,
				eTotal
			};

		   CalculationDist2PointsDialog(Model* pModel);

            // Description: Destructor
            virtual ~CalculationDist2PointsDialog();

			virtual void OnInitDialog(void);
		
			virtual void OnDialogActivated();

    		// Description: update the Hello World model with the new values
            virtual void UpdateData();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );

			// Description: Hello World model
            CalculationDist2PointsModel* GetDataModel() const;
			virtual void RefreshControls();

			void calc_distances(
						short p1, char* pid1, char* bXmea1, char* bXdes1, char* bYmea1, char* bYdes1,char* bZmea1, char* bZdes1,
						short p2, char* pid2, char* bXmea2, char* bXdes2, char* bYmea2, char* bYdes2,char* bZmea2, char* bZdes2);

	protected:
			// Description: add all controls			
			//GUI::MultiTextValueCtrlC*  m_pHeader;

			GUI::ComboLineCtrlC* m_pPoints;
			
			GUI::TextCtrlC* m_pHeaderDevAct;
			GUI::TextCtrlC* m_pHeaderDevDes;
			GUI::TextCtrlC* m_pHeaderDevDev;
			
			GUI::TextCtrlC* m_pXText;
			GUI::TextCtrlC* m_pYText;
			GUI::TextCtrlC* m_pZText;
			GUI::TextCtrlC* m_pTotalText;
			
			GUI::TextCtrlC* m_pXAct;
			GUI::TextCtrlC* m_pYAct;
			GUI::TextCtrlC* m_pZAct;
			GUI::TextCtrlC* m_pTotalAct;

			GUI::TextCtrlC* m_pXDsg;
			GUI::TextCtrlC* m_pYDsg;
			GUI::TextCtrlC* m_pZDsg;
			GUI::TextCtrlC* m_pTotalDsg;

			GUI::TextCtrlC* m_pXDev;
			GUI::TextCtrlC* m_pYDev;
			GUI::TextCtrlC* m_pZDev;
			GUI::TextCtrlC* m_pTotalDev;
			/*
			GUI::ComboLineCtrlC* m_pX;
			GUI::ComboLineCtrlC* m_pY;
			GUI::ComboLineCtrlC* m_pZ;
			GUI::ComboLineCtrlC* m_pTotal;
			*/
			// pointer to Model
			Model* m_pModel;

	private:
			// load text...
			StringC m_strDevActual;
			StringC m_strDevDesign;
			StringC m_strDevDev;
			StringC m_strX;
			StringC m_strY;
			StringC m_strZ;
			StringC m_strTotal;

			Common *m_pCommon;

			StringC sXline;
			StringC sYline;
			StringC sZline;
			StringC sTline;
			StringC sPoints;

			char bXmea[15], bYmea[15], bZmea[15]; 
			char bXdes[15], bYdes[15], bZdes[15]; 
			char bXdif[15], bYdif[15], bZdif[15];
			char bDmea[15], bDdes[15], bDdif[15];
	};
    class CalculationDist2PointsModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            CalculationDist2PointsModel();

            // Description: Destructor
            virtual ~CalculationDist2PointsModel();
		
			StringC sFile;
			StringC sTitle;
			S_SELECT_POINTS points[MAX_POINTS_IN_FILE];
			AdfFileFunc* pAdfFile;
			short m_iCounts;
    };
};

#endif // DCP_BASEGUI_HPP




