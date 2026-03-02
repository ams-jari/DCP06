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

#ifndef DCP_AIM_HPP
#define DCP_AIM_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/DCP_Types.hpp>
#include <dcp06/core/DCP_Common.hpp>

#include <GUI_AppBase.hpp>
#include <TBL_SurveyModel.hpp>
#include <TBL_Measurement.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration


    // Description: Tabbed controller for the Hello World application
    class DCP06AimControllerC : public GUI::ControllerC, public TBL::MeasurementC
    {
        public:

            // Description: Constructor
            DCP06AimControllerC(double x, double y, double z, short cds);
			~DCP06AimControllerC();


            // Description: Handle change of position values
           // virtual void OnF1Pressed(); 
			//virtual void OnF2Pressed();
			//virtual void OnF3Pressed();
			//virtual void OnF4Pressed();
			//virtual void OnF5Pressed();
			//virtual void OnF6Pressed();
			//virtual void OnSHF1Pressed();
			//virtual void OnSHF2Pressed();
			//virtual void OnSHF3Pressed();
			// Description: Route model to everybody else

			virtual bool SetModel( GUI::ModelC* pModel );
		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			virtual void OnControllerActivated(void);
			virtual void Run(void);
			short set_aim();
			void set_values(double x, double y, double z, short cds);
			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            DCP06AimControllerC( const DCP06AimControllerC& oDCP05AimController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            DCP06AimControllerC& operator=( const DCP06AimControllerC& oDCP05AimController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }

			

			DCP06CommonC* m_pCommon;
			DCP06ModelC* m_pDCP06Model;

			double m_dX,m_dY, m_dZ;
			double theta1, fii1, dist1;
			double point1[4], point2[4];
			char thetastr[15], fiistr[15];
			double inv_matrix[4][4];
			short lockin_done;
			short m_iCds;
    };



};

#endif // DCP_BASEGUI_HPP




