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

#ifndef DCP_MEASXYZ_HPP
#define DCP_MEASXYZ_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Defs.hpp>
#include <dcp06/core/Common.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>
#include <GUI_HourGlass.hpp>

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
	class MeasXYZModel;
	class DCPSurveyModelC;
	class MeasErrorHandler;
	class AmsLog;

    // Description: Tabbed controller for the Hello World application
    class MeasXYZController : public GUI::ControllerC, public TBL::MeasurementC
    {
        public:

            // Description: Constructor
            MeasXYZController(Model *pModel);
			~MeasXYZController();


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
			Model*   poModel;
		protected:

		private:

            // Description: Copy constructor
            // Remarks    : not implemented
            MeasXYZController( const MeasXYZController& oMeasXYZController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            MeasXYZController& operator=( const MeasXYZController& oMeasXYZController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

			MeasXYZModel *m_pMeasXYZModel;

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
			Common* m_pCommon;
			GUI::HourGlassC* poHourGlass;

    };

	  // Description: Tabbed controller for the Hello World application
    class DoMeasXYZController : public GUI::ControllerC, public TBL::MeasurementC
    {
        public:

            // Description: Constructor
            DoMeasXYZController();
			~DoMeasXYZController();


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
			Model*   poModel;
		protected:
			virtual void OnOperationDistEvent(int unNotifyCode,  int ulOperationId);
			virtual void OnOperationSearchEvent(int unNotifyCode, int ulOperationId);
			virtual void OnPeriodicInclineValidation(int ulParam1, int ulParam2);
			virtual void OnFinish(void);
			virtual void OnStopDistEvent(int unParam1, int ulParam2);
			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DoMeasXYZController( const DoMeasXYZController& oDoMeasXYZController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DoMeasXYZController& operator=( const DoMeasXYZController& oDoMeasXYZController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

			MeasXYZModel *m_pMeasXYZModel;

			double m_dX;
			double m_dY;
			double m_dZ;
			//TBL::MeasErrorHandlerC* poErrorHandler;
			MeasErrorHandler* poErrorHandler;

			
			
    };

    class DCPSurveyModelC : public TBL::SurveyModelC
    {
        public:
			
			struct CoordinateT
			{
				/*
				double dX;
				double dY;
				double dZ;
				*/
				double dD,dH, dV;
			};

			std::vector<CoordinateT>::iterator m_CoordinateIterator;
			std::vector<CoordinateT> m_CoordinateVector;

            // Description: Constructor
            // Input      : poConfigModel - pointer to configuration model
            DCPSurveyModelC(/*HEWConfigModelC* poConfigModel*/);

            // Description: Destructor
            //
            virtual ~DCPSurveyModelC();

            // Description: Get configuration model
            // Return     : Pointer to configuration model
            //HEWConfigModelC* GetConfigModel(void);
            
            // Description: Get height difference
            // Return     : Height difference
            double GetDeltaHt(void) const;

            // Description: Set height difference
            // Input      : dDeltaHt - Height difference to set
            void SetDeltaHt(double dDeltaHt);

        private:
        
            // Description: Pointer to configuration model
            //HEWConfigModelC* m_poConfigModel;
        
            // Description: Calculated delta height
            double           m_dDeltaHt;
    };

	class MeasXYZModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            MeasXYZModel();

            // Description: Destructor
            //
            virtual ~MeasXYZModel();
						
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

class Log
{
	public:
		Log(char* sMsg);
		Log(char* sMsg, int ret);
		Log(char* sMsg, int ctrl, int exit_code);

		~Log();

	private:
		FILE* m_pFile;
};

class AmsLog
{
	public:

		AmsLog(MeasXYZModel* pMeasModel, Model* pModel);
	    ~AmsLog();

	private:
		FILE* m_pFile;
		Model* m_pModel;
		char* remove_dot(double dAngle, char* dest);
		char* get_distance(double dDist, char* dest, Model* pModel);
		char* get_xyz_distance(double dDist, char* dest, Model* pModel);
};

class AmsToolLog
{
	public:

		AmsToolLog(char* pFileName, char* pointid, Common* pCommon, Model* pModel); 
	    ~AmsToolLog();

	private:
		FILE* m_pFile;
		//Model* m_pModel;
};


class MeasErrorHandler:public TBL::MeasErrorHandlerC,public GUI::ControllerC, public TBL::MeasurementC //, public OBS::SubjectC
{
public:
	 virtual HandlingKindT HandleMeasError(MeasErrorT eMeasError, MeasErrorSourceT eSource, unsigned int ulErrorCodeSensor);
	 //OBS::SubjectC* GetSubject(void);	
//private:	
//	 OBS::SubjectC m_oCloseSubject;	
};

}
#endif // DCP_BASEGUI_HPP




