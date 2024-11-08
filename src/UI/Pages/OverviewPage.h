#pragma once

#include "BasePage.h"

#include <QChartView>
#include <QChart>
#include <QPieSeries>
#include <QPieSlice>


class OverviewPage : public BasePage{
Q_OBJECT
public:
    explicit OverviewPage(QWidget* parent = nullptr);
    ~OverviewPage();


private:
    QChartView *m_StatusChartView = nullptr;
    QChartView *m_FluorescenceResultChartView = nullptr;

    void getStatus();
    void getFluorescenceResult();


};
