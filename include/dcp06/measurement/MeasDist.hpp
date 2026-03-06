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

#ifndef DCP_MEASDIST_HPP
#define DCP_MEASDIST_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/measurement/MeasXYZ.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>

#include <TBL_Measurement.hpp>
#include <TBL_DistanceMeasProcedure.hpp>
#include <TBL_MeasErrorHandler.hpp>

using namespace TBL;

namespace ABL
{
    // Description: Forward declaration
    class SurveyModelC;
}

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class Model;
	//class MeasureDialog;
	class MeasDistModel;
	class DCPSurveyModelC;
	class DistErrorHandler;

    // Description: Tabbed controller for the Hello World application
    class MeasDistController : public GUI::ControllerC, public TBL::MeasurementC
    {
        public:

            // Description: Constructor
            MeasDistController(Model *pModel);
			~MeasDistController();


			// Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            //virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			virtual void Run();
			virtual void OnControllerActivated(void);
            virtual void OnControllerClosed(int lExitCode);

			short get_xyz_values(double* x, double* y, double* z);

			DCPSurveyModelC*   poSurveyModel;
			Model*   poModel;
		protected:

		private:

            // Description: Copy constructor
            // Remarks    : not implemented
            MeasDistController( const MeasDistController& oMeasDistController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            MeasDistController& operator=( const MeasDistController& oMeasDistController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

			MeasDistModel *m_pMeasDistModel;

			double m_dDist;
			double m_dHor;
			double m_dVer;
	    };

	  // Description: Tabbed controller for the Hello World application
    class DoMeasDistController : public GUI::ControllerC, public TBL::MeasurementC
    {
        public:

            // Description: Constructor
            DoMeasDistController();
			~DoMeasDistController();


			// Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            //virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			
			virtual void OnControllerActivated(void);
            virtual void OnControllerClosed(int lExitCode);

			short get_xyz_values(double* x, double* y, double* z);

			DCPSurveyModelC*   poSurveyModel;
			Model*   poModel;
		protected:
			virtual void OnOperationDistEvent(int unNotifyCode,  int ulOperationId);
			virtual void OnPeriodicInclineValidation(int ulParam1, int ulParam2);
			virtual void OnStopDistEvent(int unParam1, int ulParam2);
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DoMeasDistController( const DoMeasDistController& oDoMeasDistController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DoMeasDistController& operator=( const DoMeasDistController& oDoMeasDistController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

			MeasDistModel *m_pMeasDistModel;

			double m_dDist;
			double m_dH;
			double m_dV;
			DistErrorHandler* poErrorHandler;

    };

	class MeasDistModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            MeasDistModel();

            // Description: Destructor
            //
            virtual ~MeasDistModel();
						
			double m_dD;
			double m_dH;
			double m_dV;
    };

	class DistErrorHandler:public TBL::MeasErrorHandlerC,public GUI::ControllerC, public TBL::MeasurementC //, public OBS::SubjectC
	{
	public:
		 virtual HandlingKindT HandleMeasError(MeasErrorT eMeasError, MeasErrorSourceT eSource, unsigned int ulErrorCodeSensor);
		 //OBS::SubjectC* GetSubject(void);	
	//private:	
	//	 OBS::SubjectC m_oCloseSubject;	
	};
};

#endif // DCP_BASEGUI_HPP




