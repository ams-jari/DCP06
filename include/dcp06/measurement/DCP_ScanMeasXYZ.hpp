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

#ifndef DCP_SCANMEASXYZ_HPP
#define DCP_SCANMEASXYZ_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Defs.hpp>
#include <dcp06/core/DCP_Common.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
#include <GUI_HourGlass.hpp>

#include <TBL_Measurement.hpp>
#include <TBL_DistanceMeasProcedure.hpp>
#include <TBL_MeasErrorHandler.hpp>

#include "dcp06/measurement/DCP_MeasXYZ.hpp"

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
	class DCP06MeasXYZModelC;
	class DCPSurveyModelC;
	class DCP06ScanErrorHandlerC;
	class DCP06AmsLogC;
	class DCP06Log;
	class DCP06DoScanMeasXYZControllerC;

    // Description: Tabbed controller for the Hello World application
    class DCP06ScanMeasXYZControllerC : public GUI::ControllerC, public TBL::MeasurementC
    {
        public:

            // Description: Constructor
            DCP06ScanMeasXYZControllerC(DCP06ModelC *pDCP06Model);
			~DCP06ScanMeasXYZControllerC();


			// Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            //virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );

			virtual void OnControllerActivated(void);
            virtual void OnControllerClosed(int lExitCode);
			virtual void Run();

			short get_xyz_values(double* x, double* y, double* z);

			DCPSurveyModelC*   poSurveyModel;
			DCP06ModelC*   poDCP06Model;
		protected:

		private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP06ScanMeasXYZControllerC( const DCP06ScanMeasXYZControllerC& oDCP05ScanMeasXYZController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06ScanMeasXYZControllerC& operator=( const DCP06ScanMeasXYZControllerC& oDCP05ScanMeasXYZController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

			DCP06MeasXYZModelC *m_pMeasXYZModel;

			double m_dDist;
			double m_dHor;
			double m_dVer;
	
			short m_iAverageDistCount;
			double m_dAveragedDistStdDev;
			float m_fPrismConst;
			float m_fPrismHeight;
			float m_dPPM;
			
			short m_iCount;
			short m_iCount2;

			double dist_tot, ver_tot, hor_tot, dist_tot2, hor_tot2, ver_tot2;
			double dist1, hor1, ver1,dist2, ver2, hor2; 
			
			double x_temp, y_temp, z_temp;
			double x_tot, y_tot, z_tot;
			double x_tot2, y_tot2, z_tot2;
			double x_new, y_new, z_new;
			double x_new1, y_new1, z_new1;
			double x_new2, y_new2, z_new2;
			short m_iUseTool;

			double x_scs1, y_scs1, z_scs1, x_scs2, y_scs2, z_scs2, x_scs_tot, y_scs_tot, z_scs_tot;

			void to_xyz(double dis, double hor, double ver, double *x, double *y, double *z, short tool, double *x_scs, double *y_scs, double *z_scs);
			DCP06CommonC* m_pCommon;
			GUI::HourGlassC* poHourGlass;

    };

	  // Description: Tabbed controller for the Hello World application
    class DCP06DoScanMeasXYZControllerC : public GUI::ControllerC, public TBL::MeasurementC
    {
        public:

            // Description: Constructor
            DCP06DoScanMeasXYZControllerC();
			~DCP06DoScanMeasXYZControllerC();


			// Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );
			//virtual bool OnKeyPress(wchar_t cKey, bool dDown);
			//virtual void OnF1Pressed(); 
		
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
			virtual void OnOperationSearchEvent(int unNotifyCode, int ulOperationId);
			virtual void OnPeriodicInclineValidation(int ulParam1, int ulParam2);
			virtual void OnFinish(void);
			virtual void OnStopDistEvent(int unParam1, int ulParam2);
			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP06DoScanMeasXYZControllerC( const DCP06DoScanMeasXYZControllerC& oDCP05DoScanMeasXYZController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06DoScanMeasXYZControllerC& operator=( const DCP06DoScanMeasXYZControllerC& oDCP05DoScanMeasXYZController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

			DCP06MeasXYZModelC *m_pMeasXYZModel;

			double m_dX;
			double m_dY;
			double m_dZ;
			//TBL::MeasErrorHandlerC* poErrorHandler;
			DCP06ScanErrorHandlerC* poErrorHandler;
	};

	class DCP06ScanMeasXYZModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP06ScanMeasXYZModelC();

            // Description: Destructor
            //
            virtual ~DCP06ScanMeasXYZModelC();
						
			double m_dX;
			double m_dY;
			double m_dZ;
			short tooli;
			char sPointId[POINT_ID_BUFF_LEN];
			
			double m_dD;
			double m_dH;
			double m_dV;
			
			short m_iAverageDistCount;
			double m_dAveragedDistStdDev;
			float m_fPrismConst;
			float m_fPrismHeight;
			float m_dPPM;

			double x_scs, y_scs, z_scs;
			char tool_name[20];
			
	private:		
			
			/*
			S_POINT_BUFF point_table[20];
			S_POINT_BUFF point_table2[20];
			*/
    };


class DCP06ScanErrorHandlerC:public TBL::MeasErrorHandlerC,public GUI::ControllerC, public TBL::MeasurementC //, public OBS::SubjectC
{
public:
	 virtual HandlingKindT HandleMeasError(MeasErrorT eMeasError, MeasErrorSourceT eSource, unsigned int ulErrorCodeSensor);
	 //OBS::SubjectC* GetSubject(void);	
//private:	
//	 OBS::SubjectC m_oCloseSubject;	
};

}
#endif // DCP_BASEGUI_HPP




