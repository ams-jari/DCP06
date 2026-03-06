// ====================================================================================================================
// CalculationLine.hpp - Line calculation core and controller
// Consolidated from CalcLine.hpp and CalcLineController.hpp
// ====================================================================================================================

#ifndef DCP_CALCULATION_LINE_HPP
#define DCP_CALCULATION_LINE_HPP

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Common.hpp>
#include <dcp06/core/Defs.hpp>
#include <GUI_AppBase.hpp>
#include <TBL_SurveyModel.hpp>
#include <TBL_Measurement.hpp>

namespace DCP
{
    class CalcLine
    {
    public:
        CalcLine();
        ~CalcLine();
        short calc(S_LINE_BUFF* line, short actdes);
        short defined_points_count_in_line(S_LINE_BUFF* line, short* lastpoint);
        short points_count_in_line(S_LINE_BUFF* line);
    private:
    };

    class CalcLineController : public GUI::ControllerC
    {
    public:
        CalcLineController(S_LINE_BUFF* oLine, short actdes, short iAskId);
        ~CalcLineController();
        virtual bool SetModel(GUI::ModelC* pModel);
        virtual void OnActiveDialogClosed(int lDlgID, int lExitCode);
        virtual void OnActiveControllerClosed(int lCtrlID, int lExitCode);
        virtual void OnControllerActivated(void);
        virtual void Run(void);
    private:
        CalcLineController(const CalcLineController&) { USER_APP_VERIFY(false); }
        CalcLineController& operator=(const CalcLineController&) { USER_APP_VERIFY(false); return *this; }
        Common* m_pCommon;
        Model* m_pModel;
        S_LINE_BUFF* m_pLineBuff;
        short m_iActDes;
        short m_iAskId;
    };
}

#endif // DCP_CALCULATION_LINE_HPP
