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

#ifndef DCP_MEASXYZ_HPP
#define DCP_MEASXYZ_HPP

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
	class DCP05ModelC;
	//class DCP05MeasDlgC;
	class DCP05MeasXYZModelC;
	class DCPSurveyModelC;
	class DCP05ErrorHandlerC;
	class DCP05AmsLogC;

    // Description: Tabbed controller for the Hello World application
    class DCP05MeasXYZControllerC : public GUI::ControllerC, public TBL::MeasurementC
    {
        public:

            // Description: Constructor
            DCP05MeasXYZControllerC(DCP05ModelC *pDCP05Model);
			~DCP05MeasXYZControllerC();


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
			DCP05ModelC*   poDCP05Model;
		protected:

		private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP05MeasXYZControllerC( const DCP05MeasXYZControllerC& oDCP05MeasXYZController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05MeasXYZControllerC& operator=( const DCP05MeasXYZControllerC& oDCP05MeasXYZController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

			DCP05MeasXYZModelC *m_pMeasXYZModel;

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
			DCP05CommonC* m_pCommon;
			GUI::HourGlassC* poHourGlass;

    };

	  // Description: Tabbed controller for the Hello World application
    class DCP05DoMeasXYZControllerC : public GUI::ControllerC, public TBL::MeasurementC
    {
        public:

            // Description: Constructor
            DCP05DoMeasXYZControllerC();
			~DCP05DoMeasXYZControllerC();


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
			DCP05ModelC*   poDCP05Model;
		protected:
			virtual void OnOperationDistEvent(int unNotifyCode,  int ulOperationId);
			virtual void OnOperationSearchEvent(int unNotifyCode, int ulOperationId);
			virtual void OnPeriodicInclineValidation(int ulParam1, int ulParam2);
			virtual void OnFinish(void);
			virtual void OnStopDistEvent(int unParam1, int ulParam2);
			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP05DoMeasXYZControllerC( const DCP05DoMeasXYZControllerC& oDCP05DoMeasXYZController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP05DoMeasXYZControllerC& operator=( const DCP05DoMeasXYZControllerC& oDCP05DoMeasXYZController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

			DCP05MeasXYZModelC *m_pMeasXYZModel;

			double m_dX;
			double m_dY;
			double m_dZ;
			//TBL::MeasErrorHandlerC* poErrorHandler;
			DCP05ErrorHandlerC* poErrorHandler;

			
			
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

	class DCP05MeasXYZModelC : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            DCP05MeasXYZModelC();

            // Description: Destructor
            //
            virtual ~DCP05MeasXYZModelC();
						
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

class DCP05Log
{
	public:
		DCP05Log(char* sMsg);
		DCP05Log(char* sMsg, int ret);
		DCP05Log(char* sMsg, int ctrl, int exit_code);

		~DCP05Log();

	private:
		FILE* m_pFile;
};

class DCP05AmsLogC
{
	public:

		DCP05AmsLogC(DCP05MeasXYZModelC* pModel, DCP05ModelC* pDCP05Model);
	    ~DCP05AmsLogC();

	private:
		FILE* m_pFile;
		DCP05ModelC* m_pDCP05Model;
		char* remove_dot(double dAngle, char* dest);
		char* get_distance(double dDist, char* dest, DCP05ModelC* pDCP05Model);
		char* get_xyz_distance(double dDist, char* dest, DCP05ModelC* pDCP05Model);
};

class DCP05AmsToolLogC
{
	public:

		DCP05AmsToolLogC(char* pFileName, char* pointid, DCP05CommonC* pCommon, DCP05ModelC* pDCP05Model); 
	    ~DCP05AmsToolLogC();

	private:
		FILE* m_pFile;
		//DCP05ModelC* m_pDCP05Model;
};


class DCP05ErrorHandlerC:public TBL::MeasErrorHandlerC,public GUI::ControllerC, public TBL::MeasurementC //, public OBS::SubjectC
{
public:
	 virtual HandlingKindT HandleMeasError(MeasErrorT eMeasError, MeasErrorSourceT eSource, unsigned int ulErrorCodeSensor);
	 //OBS::SubjectC* GetSubject(void);	
//private:	
//	 OBS::SubjectC m_oCloseSubject;	
};

}
#endif // DCP_BASEGUI_HPP




