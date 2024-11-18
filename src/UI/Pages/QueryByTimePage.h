#pragma once

#include <QChart>
#include <QChartView>
#include <QPieSlice>

#include "UI/Pages/BasePage.h"

class QueryByTimePage : public BasePage {
    Q_OBJECT

public:
    explicit QueryByTimePage(QWidget* parent = nullptr);

    ~QueryByTimePage();

private:
    ElaText* m_TimeRange = nullptr;

    QChart* m_StatusChart = nullptr;
    QChartView* m_StatusChartView = nullptr;

    QChart* m_FluorescenceResultChart = nullptr;
    QChartView* m_FluorescenceResultChartView = nullptr;

    QChart* m_PerfusionChart = nullptr;
    QChartView* m_PerfusionChartView = nullptr;

    ElaText* m_SuccessRateText = nullptr;

    void RefreshGlobalData();

    void getStatus();

    void getFluorescenceResult();

    void calculateSuccessRate();

    void calculateTimeRange();

    void plotPerfusionResults();

    void onSliceHovered(QPieSlice* slice, bool state);
};
