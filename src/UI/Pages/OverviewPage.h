#pragma once

#include <QChart>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>

#include "BasePage.h"
#include "FileReader/CellTissueInfo.hpp"

class OverviewPage : public BasePage {
    Q_OBJECT

public:
    explicit OverviewPage(QWidget* parent = nullptr);

    ~OverviewPage();

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

    std::pair<std::tm, std::tm> calculateTimeRange();

    void plotPerfusionResults();

    void onSliceHovered(QPieSlice* slice, bool state);
};
