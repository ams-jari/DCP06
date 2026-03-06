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
	class DCP06ModelC;
	//class DCP06MeasDlgC;
	class DCP06MeasDistModelC;
	class DCPSurveyModelC;
	class DCP06DistErrorHandlerC;

    // Description: Tabbed controller for the Hello World application
    class DCP06MeasDistControllerC : public GUI::ControllerC, public TBL::MeasurementC
    {
        public:

            // Description: Constructor
            DCP06MeasDistControllerC(DCP06ModelC *pDCP06Model);
			~DCP06MeasDistControllerC();


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
			DCP06ModelC*   poDCP06Model;
		protected:

		private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP06MeasDistControllerC( const DCP06MeasDistControllerC& oDCP06MeasDistController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06MeasDistControllerC& operator=( const DCP06MeasDistControllerC& oDCP06MeasDistController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

			DCP06MeasDistModelC *m_pMeasDistModel;

			double m_dDist;
			double m_dHor;
			double m_dVer;
	    };

	  // Description: Tabbed controller for the Hello World application
    class DCP06DoMeasDistControllerC : public GUI::ControllerC, public TBL::MeasurementC
    {
        public:

            // Description: Constructor
            DCP06DoMeasDistControllerC();
			~DCP06DoMeasDistControllerC();


			// Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            //virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			
			virtual void OnControllerActivated(void);
            virtual void OnControllerClosed(int lExitCode);

			short get_xyz_values(double* x, double* y, double* z);

			DCPSurveyModelC*   poSurveyModel;
			DCP06ModelC*   poDCP06Model;
		protected:
			virtual void OnOperationDistEvent(int unNotifyCode,  int ulOperationId);
			virtual void OnPeriodicInclineValidation(int ulParam1, int ulParam2);
			virtual void OnStopDistEvent(int unParam1, int ulParam2);
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP06DoMeasDistControllerC( const DCP06DoMeasDistControllerC& oDCP06DoMeasDistController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06DoMeasDistControllerC& operator=( const DCP06DoMeasDistControllerC& oDCP06DoMeasDistController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

			DCP06MeasDistModelC *m_pMeasDistModel;

			double m_dDist;
			double m_dH;
			double m_dV;
			DCP06DistErrorHandlerC* poErrorHandler;

    };

	class DCP06MeasDistModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06MeasDistModelC();

            // Description: Destructor
            //
            virtual ~DCP06MeasDistModelC();
						
			double m_dD;
			double m_dH;
			double m_dV;
    };

	class DCP06DistErrorHandlerC:public TBL::MeasErrorHandlerC,public GUI::ControllerC, public TBL::MeasurementC //, public OBS::SubjectC
	{
	public:
		 virtual HandlingKindT HandleMeasError(MeasErrorT eMeasError, MeasErrorSourceT eSource, unsigned int ulErrorCodeSensor);
		 //OBS::SubjectC* GetSubject(void);	
	//private:	
	//	 OBS::SubjectC m_oCloseSubject;	
	};
};

#endif // DCP_BASEGUI_HPP




