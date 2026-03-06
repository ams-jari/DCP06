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
    class DCP06CalcLineC
    {
    public:
        DCP06CalcLineC();
        ~DCP06CalcLineC();
        short calc(S_LINE_BUFF* line, short actdes);
        short defined_points_count_in_line(S_LINE_BUFF* line, short* lastpoint);
        short points_count_in_line(S_LINE_BUFF* line);
    private:
    };

    class DCP06CalcLineControllerC : public GUI::ControllerC
    {
    public:
        DCP06CalcLineControllerC(S_LINE_BUFF* oLine, short actdes, short iAskId);
        ~DCP06CalcLineControllerC();
        virtual bool SetModel(GUI::ModelC* pModel);
        virtual void OnActiveDialogClosed(int lDlgID, int lExitCode);
        virtual void OnActiveControllerClosed(int lCtrlID, int lExitCode);
        virtual void OnControllerActivated(void);
        virtual void Run(void);
    private:
        DCP06CalcLineControllerC(const DCP06CalcLineControllerC&) { USER_APP_VERIFY(false); }
        DCP06CalcLineControllerC& operator=(const DCP06CalcLineControllerC&) { USER_APP_VERIFY(false); return *this; }
        DCP06CommonC* m_pCommon;
        DCP06ModelC* m_pDCP06Model;
        S_LINE_BUFF* m_pLineBuff;
        short m_iActDes;
        short m_iAskId;
    };
}

#endif // DCP_CALCULATION_LINE_HPP
