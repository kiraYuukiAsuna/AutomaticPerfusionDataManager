#pragma once

#include "ElaCalendar.h"
#include "ElaMultiSelectComboBox.h"
#include "ElaPushButton.h"

#include <QChart>
#include <QChartView>
#include <QPieSlice>
#include <QTimeEdit>

#include "ElaCalendarPicker.h"
#include "ElaCheckBox.h"
#include "UI/Pages/BasePage.h"

class QueryByTimePage : public BasePage {
    Q_OBJECT

public:
    explicit QueryByTimePage(QWidget* parent = nullptr);

    ~QueryByTimePage();

public:
    void RefreshGlobalData();

    void ExecuteQuery();

    void GetAllUsers();

    std::pair<std::tm, std::tm> calculateTimeRange();

    void calculateSuccessRate();

    void plotStatus();

    void plotFluorescenceResult();

    void plotPerfusionResultsByDay();

private:
    QWidget* m_CentralWidget = nullptr;

    ElaCalendarPicker* m_StartDate = nullptr;
    ElaCalendarPicker* m_EndDate = nullptr;

    QTimeEdit* m_StartTime = nullptr;
    QTimeEdit* m_EndTime = nullptr;

    ElaText* m_EnableTimeRangeQuery = nullptr;
    ElaCheckBox* m_EnableTimeRangeQueryCheckBox = nullptr;

    ElaPushButton* m_RefreshButton = nullptr;
    ElaPushButton* m_QueryButton = nullptr;

    ElaMultiSelectComboBox* m_UserComboBox = nullptr;

    ElaText* m_TimeRange = nullptr;

    QChart* m_StatusChart = nullptr;
    QChartView* m_StatusChartView = nullptr;

    QChart* m_FluorescenceResultChart = nullptr;
    QChartView* m_FluorescenceResultChartView = nullptr;

    QChart* m_PerfusionChart = nullptr;
    QChartView* m_PerfusionChartView = nullptr;

    ElaText* m_SuccessRateText = nullptr;

private:
    void onSliceHovered(QPieSlice* slice, bool state);

    void GetCurrentSelectedTimeRange(std::time_t&start, std::time_t&end);
};
