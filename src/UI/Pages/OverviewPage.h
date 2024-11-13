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
	explicit OverviewPage(QWidget *parent = nullptr);
	~OverviewPage();

private:
    ElaText *m_TimeRange = nullptr;

	QChartView *m_StatusChartView = nullptr;
	QChartView *m_FluorescenceResultChartView = nullptr;
    QChartView *m_PerfusionChartView = nullptr;

    ElaText *m_SuccessRateText = nullptr;

	void getStatus();
	void getFluorescenceResult();
    void calculateSuccessRate();
    void calculateTimeRange();
    void plotPerfusionResults();

    void onSliceHovered(QPieSlice *slice, bool state);
};
