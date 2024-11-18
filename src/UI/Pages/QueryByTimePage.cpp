#include "QueryByTimePage.h"

#include <QChart>
#include <QChartView>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QLineSeries>
#include <QPieSeries>
#include <QToolTip>
#include <QValueAxis>
#include <QVBoxLayout>

#include "ElaPushButton.h"
#include "ElaText.h"
#include "Analysis/AnalysisBase.hpp"

QueryByTimePage::QueryByTimePage(QWidget* parent) : BasePage(parent, "查询") {
    auto controlLayout = new QHBoxLayout(this);

    auto m_RefreshButton = new ElaPushButton("刷新", this);
    connect(m_RefreshButton, &ElaPushButton::clicked, this, [this]() {
        RefreshGlobalData();
    });
    controlLayout->addWidget(m_RefreshButton);

    m_StatusChart = new QChart;
    m_StatusChartView = new QChartView(m_StatusChart, this);

    m_FluorescenceResultChart = new QChart;
    m_FluorescenceResultChartView = new QChartView(m_FluorescenceResultChart, this);

    m_SuccessRateText = new ElaText("有效细胞数量：xxx，成功灌注细胞数量：xxx | 成功率：xx%", this);

    m_TimeRange =
            new ElaText("已统计数据时间范围：xxxx.xx.xx - xxxx.xx.xx", this);

    m_PerfusionChart = new QChart;
    m_PerfusionChartView = new QChartView(m_PerfusionChart, this);

    RefreshGlobalData();

    auto* centralWidget = this->centralWidget();
    auto* centerLayout = new QVBoxLayout(centralWidget);

    auto* chartLayout = new QHBoxLayout;
    chartLayout->addWidget(m_StatusChartView);
    chartLayout->addWidget(m_FluorescenceResultChartView);

    centerLayout->addLayout(controlLayout);
    centerLayout->addWidget(m_TimeRange);
    centerLayout->addWidget(m_SuccessRateText);
    centerLayout->addLayout(chartLayout);
    centerLayout->addWidget(m_PerfusionChartView);
}

QueryByTimePage::~QueryByTimePage() {
}

void QueryByTimePage::RefreshGlobalData() {
    getStatus();
    getFluorescenceResult();
    calculateSuccessRate();
    calculateTimeRange();
    plotPerfusionResults();
}

void QueryByTimePage::getStatus() {
    auto results = AnalysisBase::getInstance().GetStatusGlobal();

    auto series = new QPieSeries;

    int idx = 0;
    for (const auto&row: results) {
        series->append(QString::fromStdString(std::get<0>(row)), std::get<1>(row));
        std::cout << "Status: " << std::get<0>(row)
                << ", Count: " << std::get<1>(row) << std::endl;

        QPieSlice* slice = series->slices().at(idx);
        slice->setLabelVisible();
        slice->setPen(QPen(Qt::darkBlue, 2));

        auto hash = std::hash<std::string>{}(std::get<0>(row));
        QColor color;
        int red = static_cast<unsigned char>(hash >> 8 % 255);
        int green = static_cast<unsigned char>(hash >> 16 % 255);
        int blue = static_cast<unsigned char>(hash >> 32 % 255);
        color.setRgb(red, green, blue);

        slice->setBrush(color);
        slice->setLabel(QString::fromStdString(std::get<0>(row)));
        slice->setLabelVisible();
        idx++;
    }

    m_StatusChart->removeAllSeries();
    m_StatusChart->addSeries(series);
    m_StatusChart->setTitle("Status Result");
    m_StatusChart->legend()->hide();

    m_StatusChartView->setRenderHint(QPainter::Antialiasing, true);
    m_StatusChartView->chart()->setAnimationOptions(QChart::AllAnimations);
    m_StatusChartView->chart()->legend()->show();

    connect(series, &QPieSeries::hovered, this, &QueryByTimePage::onSliceHovered);
}

void QueryByTimePage::getFluorescenceResult() {
    auto results = AnalysisBase::getInstance().GetFluorescenceResultGlobal();

    auto series = new QPieSeries;

    int idx = 0;
    for (const auto&row: results) {
        series->append(QString::fromStdString(std::get<0>(row)), std::get<1>(row));
        std::cout << "FluorescenceResult: " << std::get<0>(row)
                << ", Count: " << std::get<1>(row) << std::endl;

        QPieSlice* slice = series->slices().at(idx);
        // slice->setExploded();
        slice->setLabelVisible();
        slice->setPen(QPen(Qt::darkBlue, 2));
        // 使用哈希值生成颜色
        auto hash = std::hash<std::string>{}(std::get<0>(row));
        QColor color;
        int red = static_cast<unsigned char>(hash >> 8 % 255);
        int green = static_cast<unsigned char>(hash >> 16 % 255);
        int blue = static_cast<unsigned char>(hash >> 32 % 255);
        color.setRgb(red, green, blue);

        slice->setBrush(color);
        slice->setLabel(QString::fromStdString(std::get<0>(row)));
        slice->setLabelVisible();
        idx++;
    }

    m_FluorescenceResultChart->removeAllSeries();
    m_FluorescenceResultChart->addSeries(series);
    m_FluorescenceResultChart->setTitle("Fluorescence Result");
    m_FluorescenceResultChart->legend()->hide();

    m_FluorescenceResultChartView->setRenderHint(QPainter::Antialiasing, true);
    m_FluorescenceResultChartView->chart()->setAnimationOptions(
        QChart::AllAnimations);
    m_FluorescenceResultChartView->chart()->legend()->show();
    connect(series, &QPieSeries::hovered, this, &QueryByTimePage::onSliceHovered);
}

void QueryByTimePage::calculateSuccessRate() {
    auto [fluorescenceResults, statusResults] = AnalysisBase::getInstance().GetSuccessRateGlobal();

    int successCount = 0;
    int totalFluorescenceCount = 0;
    int missingCount = 0;

    // Calculate the total count and success count for FluorescenceResult
    for (const auto&row: fluorescenceResults) {
        if (std::get<0>(row) == "Success") {
            successCount = std::get<1>(row);
        }
        totalFluorescenceCount += std::get<1>(row);
    }

    // Calculate the missing count for Status
    for (const auto&row: statusResults) {
        if (std::get<0>(row) == "Missing") {
            missingCount = std::get<1>(row);
        }
    }

    // Calculate the success rate
    int validTotalCount = totalFluorescenceCount - missingCount;
    double successRate = validTotalCount > 0
                             ? static_cast<double>(successCount) / validTotalCount
                             : 0.0;

    std::string successRateText =
            "有效细胞数量：" + std::to_string(validTotalCount) +
            "，成功灌注细胞数量：" + std::to_string(successCount) + " | 成功率：" +
            std::to_string(successRate * 100) + "%";

    successRateText = "（测试数据）" + successRateText;

    m_SuccessRateText->setText(QString::fromStdString(successRateText));
}

void QueryByTimePage::onSliceHovered(QPieSlice* slice, bool state) {
    if (state) {
        // Get the value of the hovered slice
        QString value = QString::number(slice->value());

        // Show the tooltip at the mouse position
        QToolTip::showText(QCursor::pos(), slice->label() + ": " + value);
    }
    else {
        // Hide the tooltip when not hovering
        QToolTip::hideText();
    }

    if (state) {
        // Highlight and explode the slice when hovering
        slice->setExploded(true);
        slice->setPen(QPen(Qt::red, 2));
        slice->setBrush(Qt::green);
    }
    else {
        // Reset the slice appearance when not hovering
        slice->setExploded(false);
        QColor color;
        auto hash = std::hash<std::string>{}(slice->label().toStdString());
        int red = static_cast<unsigned char>(hash >> 8 % 255);
        int green = static_cast<unsigned char>(hash >> 16 % 255);
        int blue = static_cast<unsigned char>(hash >> 32 % 255);
        color.setRgb(red, green, blue);
        slice->setPen(QPen(Qt::darkBlue, 2));
        slice->setBrush(color);
    }
}

void QueryByTimePage::calculateTimeRange() {
    auto results = AnalysisBase::getInstance().GetTimeRangeGlobal();

    std::tm minTimePoint = {}, maxTimePoint = {};
    bool first = true;

    if (results.empty()) {
        m_TimeRange->setText(
            QString::fromStdString("（测试数据）已统计数据时间范围：无"));
        return;
    }

    for (const auto&row: results) {
        std::tm timepoint;
        auto timeString = std::get<0>(row) + " " + std::get<1>(row);
        std::istringstream timeStream(timeString);
        timeStream >> std::get_time(&timepoint, "%Y-%m-%d %H:%M:%S");

        if (first) {
            minTimePoint = timepoint;
            maxTimePoint = timepoint;
            first = false;
        }
        else {
            if (std::mktime(&timepoint) < std::mktime(&minTimePoint)) {
                minTimePoint = timepoint;
            }
            if (std::mktime(&timepoint) > std::mktime(&maxTimePoint)) {
                maxTimePoint = timepoint;
            }
        }
    }

    std::ostringstream timeStream;
    timeStream << "从 " << std::put_time(&minTimePoint, "%Y-%m-%d %H:%M:%S")
            << " 至 " << std::put_time(&maxTimePoint, "%Y-%m-%d %H:%M:%S");
    std::string perfusionTime = timeStream.str();

    m_TimeRange->setText(QString::fromStdString(
        "（测试数据）已统计数据时间范围：" + perfusionTime));
}

void QueryByTimePage::plotPerfusionResults() {
    auto results = AnalysisBase::getInstance().GetPerfusionResultsGlobal();

    std::map<time_t, int> successCounts;
    std::map<time_t, int> failureCounts;

    for (const auto&row: results) {
        std::tm timepoint;
        auto timeString = std::get<0>(row) + "00:00:00";
        std::istringstream timeStream(timeString);
        timeStream >> std::get_time(&timepoint, "%Y-%m-%d %H:%M:%S");

        std::string result = std::get<1>(row);
        if (result == "Success") {
            successCounts[std::mktime(&timepoint)]++;
        }
        else {
            failureCounts[std::mktime(&timepoint)]++;
        }
    }

    auto* successSeries = new QLineSeries();
    auto* failureSeries = new QLineSeries();
    successSeries->setName("Success");
    failureSeries->setName("Failure");

    for (const auto&entry: successCounts) {
        QDateTime dateTime = QDateTime::fromSecsSinceEpoch(entry.first);
        successSeries->append(dateTime.toMSecsSinceEpoch(), entry.second);
    }

    for (const auto&entry: failureCounts) {
        QDateTime dateTime = QDateTime::fromSecsSinceEpoch(entry.first);
        failureSeries->append(dateTime.toMSecsSinceEpoch(), entry.second);
    }

    m_PerfusionChart->removeAllSeries();
    m_PerfusionChart->addSeries(successSeries);
    m_PerfusionChart->addSeries(failureSeries);
    m_PerfusionChart->setTitle("Daily Perfusion Success and Failure Counts");

    QDateTimeAxis* axisX = new QDateTimeAxis;
    axisX->setFormat("yyyy-MM-dd");
    axisX->setTitleText("Date");
    m_PerfusionChart->addAxis(axisX, Qt::AlignBottom);
    successSeries->attachAxis(axisX);
    failureSeries->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis;
    axisY->setTitleText("Count");
    axisY->setLabelFormat("%d");
    m_PerfusionChart->addAxis(axisY, Qt::AlignLeft);
    successSeries->attachAxis(axisY);
    failureSeries->attachAxis(axisY);

    m_PerfusionChartView->setRenderHint(QPainter::Antialiasing, true);
    m_PerfusionChartView->chart()->setAnimationOptions(QChart::AllAnimations);
    m_PerfusionChartView->chart()->legend()->show();
}