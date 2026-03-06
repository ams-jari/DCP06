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

#ifndef DCP_OFFSV_DLG_HPP
#define DCP_OFFSV_DLG_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <GUI_ComboLineCtrl.hpp>
#include <GUI_StandardDialog.hpp>
#include <GUI_TextCtrl.hpp>
#include <GUI_AppBase.hpp>

#include <dcp06/file/AdfFileFunc.hpp>

// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	class Model;
	class OffsetVDialog;
	class OffsetVModel;

    // Description: Tabbed controller for the Hello World application
    class OffsetVController : public GUI::ControllerC
    {
        public:

            // Description: Constructor
			OffsetVController(Model* pModel, short display);
			~OffsetVController();

            // Description: Handle change of position values
            virtual void OnF1Pressed();
			virtual void OnF2Pressed();
			virtual void OnF5Pressed();
			virtual void OnF6Pressed();
			virtual void OnSHF2Pressed();
			
			// Description: Route model to everybody else
            virtual bool SetModel( GUI::ModelC* pModel );

		
            // Description: React on close of tabbed dialog
            virtual void OnActiveDialogClosed( int lDlgID, int lExitCode );
			virtual void OnActiveControllerClosed( int lCtrlID, int lExitCode );
			
			
        private:

            // Description: Copy constructor
            // Remarks    : not implemented
            OffsetVController( const OffsetVController& oOffsetVController )
            {
                USER_APP_VERIFY( false );
            }

            // Description: Assignment operator
            // Remarks    : not implemented
            OffsetVController& operator=( const OffsetVController& oOffsetVController )
            {
                USER_APP_VERIFY( false );
                return *this;
            }
			//AdfFileFunc* adf;
            OffsetVDialog* m_pDlg;
			Model* m_pModel;
    };


	class OffsetVDialog:public GUI::StandardDialogC, public GUI::ModelHandlerC
	{
		 public:

			enum eCtrlId
			{
				eFile=1,
				ePointId,
				eX,
				eY,
				eZ
			};

		   OffsetVDialog(Model* pModel);

            // Description: Destructor
            virtual ~OffsetVDialog();

			virtual void OnInitDialog(void);

			virtual void OnDialogActivated();
    
			// Description: update the Hello World model with the new values
            virtual void UpdateData();
			virtual void RefreshControls();
			
  		    // Description: only accept Hello World Model objects
            virtual bool SetModel( GUI::ModelC* pModel );
			OffsetVModel* GetDataModel() const;

			virtual void delete_point();
			virtual void SelectFile(StringC sFile);

			virtual StringC getfileName();

	protected:
  			
			GUI::ComboLineCtrlC* m_pFile;
			GUI::ComboLineCtrlC* m_pPointId;
			GUI::ComboLineCtrlC* m_pX;
			GUI::ComboLineCtrlC* m_pY;
			GUI::ComboLineCtrlC* m_pZ;

			Model* m_pModel;
	private:
			StringC m_sFile;
	};

    class OffsetVModel : public GUI::ModelC
    {
        public:

            // Description: Constructor
            //
            OffsetVModel();

            // Description: Destructor
            //
            virtual ~OffsetVModel();

			S_POINT_BUFF	ovalues_buff;
			S_POINT_BUFF	ovalues_tool_buff;
			S_POINT_BUFF	ref_point_buff;
			short display;
    };

};

#endif // DCP_BASEGUI_HPP




