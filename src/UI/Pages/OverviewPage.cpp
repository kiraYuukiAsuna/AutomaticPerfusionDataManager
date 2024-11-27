#include "OverviewPage.h"

#include "Database/SqliteSchema.hpp"
#include "ElaText.h"
#include <QDate>
#include <QDateTimeAxis>
#include <QLineSeries>
#include <QToolTip>
#include <QVBoxLayout>
#include <QValueAxis>

#include "ElaPushButton.h"
#include "Analysis/AnalysisBase.hpp"

OverviewPage::OverviewPage(QWidget* parent) : BasePage(parent, "总览") {
    auto controlLayout = new QHBoxLayout(this);

    auto m_RefreshButton = new ElaPushButton("刷新", this);
    m_RefreshButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
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
    auto* centerLayout = new QVBoxLayout;

    auto* chartLayout = new QHBoxLayout;
    chartLayout->addWidget(m_StatusChartView);
    chartLayout->addWidget(m_FluorescenceResultChartView);

    centerLayout->addLayout(controlLayout);
    centerLayout->addWidget(m_TimeRange);
    centerLayout->addWidget(m_SuccessRateText);
    centerLayout->addLayout(chartLayout);
    centerLayout->addWidget(m_PerfusionChartView);

    centralWidget->setLayout(centerLayout);
}

OverviewPage::~OverviewPage() {
}

void OverviewPage::RefreshGlobalData() {
    getStatus();
    getFluorescenceResult();
    calculateSuccessRate();
    calculateTimeRange();
    plotPerfusionResults();
}

void OverviewPage::getStatus() {
    auto results = AnalysisBase::GetStatus();

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

    connect(series, &QPieSeries::hovered, this, &OverviewPage::onSliceHovered);
}

void OverviewPage::getFluorescenceResult() {
    auto results = AnalysisBase::GetFluorescenceResult();

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
    connect(series, &QPieSeries::hovered, this, &OverviewPage::onSliceHovered);
}

void OverviewPage::calculateSuccessRate() {
    auto [fluorescenceResults, statusResults] = AnalysisBase::GetSuccessRate();

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

void OverviewPage::onSliceHovered(QPieSlice* slice, bool state) {
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

std::pair<std::tm, std::tm> OverviewPage::calculateTimeRange() {
    auto results = AnalysisBase::GetTimeRange();

    std::tm minTimePoint = {}, maxTimePoint = {};
    bool first = true;

    if (results.empty()) {
        m_TimeRange->setText(
            QString::fromStdString("（测试数据）已统计数据时间范围：无"));
        return {minTimePoint, maxTimePoint};
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

    return {minTimePoint, maxTimePoint};
}

void OverviewPage::plotPerfusionResults() {
    auto results = AnalysisBase::GetPerfusionResults();

    std::map<time_t, int> successCounts;
    std::map<time_t, int> failureCounts;

    auto [startTime, endTime] = calculateTimeRange();
    for (std::time_t t = std::mktime(&startTime); t <= std::mktime(&endTime); t += 86400) {
        std::tm* timeInfo = std::gmtime(&t);
        timeInfo->tm_hour = 0;
        timeInfo->tm_min = 0;
        timeInfo->tm_sec = 0;
        std::time_t adjustedTime = std::mktime(timeInfo);
        successCounts[adjustedTime] = 0;
        failureCounts[adjustedTime] = 0;
    }

    for (const auto&row: results) {
        std::tm timepoint;
        auto timeString = std::get<0>(row) + "00:00:00";
        std::istringstream timeStream(timeString);
        timeStream >> std::get_time(&timepoint, "%Y-%m-%d %H:%M:%S");

        if (successCounts.find(std::mktime(&timepoint)) == successCounts.end()) {
            successCounts[std::mktime(&timepoint)] = 0;
        }
        if (failureCounts.find(std::mktime(&timepoint)) == failureCounts.end()) {
            failureCounts[std::mktime(&timepoint)] = 0;
        }

        std::string result = std::get<1>(row);
        if (result == "Success") {
            successCounts[std::mktime(&timepoint)]++;
        }
        else {
            failureCounts[std::mktime(&timepoint)]++;
        }
    }

    auto* successSeries = new QLineSeries(this);
    auto* failureSeries = new QLineSeries(this);
    successSeries->setName("Success");
    failureSeries->setName("Failure");

    int minValue = std::numeric_limits<int>::max();
    int maxValue = -std::numeric_limits<int>::max();

    QDateTime minDateTime = QDateTime::currentDateTime();
    QDateTime maxDateTime;

    for (const auto&entry: successCounts) {
        QDateTime dateTime = QDateTime::fromSecsSinceEpoch(entry.first);
        successSeries->append(dateTime.toMSecsSinceEpoch(), entry.second);
        if (entry.second < minValue) {
            minValue = entry.second;
        }
        if (entry.second > maxValue) {
            maxValue = entry.second;
        }
        if(dateTime < minDateTime) {
            minDateTime = dateTime;
        }
        if(dateTime > maxDateTime) {
            maxDateTime = dateTime;
        }
    }

    for (const auto&entry: failureCounts) {
        QDateTime dateTime = QDateTime::fromSecsSinceEpoch(entry.first);
        failureSeries->append(dateTime.toMSecsSinceEpoch(), entry.second);
        if (entry.second < minValue) {
            minValue = entry.second;
        }
        if (entry.second > maxValue) {
            maxValue = entry.second;
        }
        if(dateTime < minDateTime) {
            minDateTime = dateTime;
        }
        if(dateTime > maxDateTime) {
            maxDateTime = dateTime;
        }
    }

    m_PerfusionChart->removeAllSeries();

    // Connect the hovered signal to the custom slot
    connect(successSeries, &QLineSeries::hovered, this, [](const QPointF&point, bool state) {
        if (state) {
            // Show the tooltip at the mouse position
            QToolTip::showText(QCursor::pos(), QString("Value: %1").arg(point.y()));
        }
        else {
            // Hide the tooltip when not hovering
            QToolTip::hideText();
        }
    });
    connect(failureSeries, &QLineSeries::hovered, this, [](const QPointF&point, bool state) {
        if (state) {
            // Show the tooltip at the mouse position
            QToolTip::showText(QCursor::pos(), QString("Value: %1").arg(point.y()));
        }
        else {
            // Hide the tooltip when not hovering
            QToolTip::hideText();
        }
    });


    for (auto&axis: m_PerfusionChart->axes()) {
        m_PerfusionChart->removeAxis(axis);
    }

    m_PerfusionChart->addSeries(successSeries);
    m_PerfusionChart->addSeries(failureSeries);
    m_PerfusionChart->setTitle("Daily Perfusion Success and Failure Counts");

    auto* axisX = new QDateTimeAxis;
    axisX->setFormat("yyyy-MM-dd");
    axisX->setTitleText("Date");
    axisX->setTickCount(successSeries->count());
    m_PerfusionChart->addAxis(axisX, Qt::AlignBottom);
    successSeries->attachAxis(axisX);
    failureSeries->attachAxis(axisX);
    axisX->setRange(minDateTime.addDays(-1), maxDateTime.addDays(1));

    auto* axisY = new QValueAxis;
    axisY->setTitleText("Count");
    axisY->setLabelFormat("%d");
    m_PerfusionChart->addAxis(axisY, Qt::AlignLeft);
    successSeries->attachAxis(axisY);
    failureSeries->attachAxis(axisY);
    axisY->setRange(std::max(0,minValue - 10), maxValue + 10);

    m_PerfusionChartView->setRenderHint(QPainter::Antialiasing, true);
    m_PerfusionChartView->setRenderHint(QPainter::TextAntialiasing, true);
    m_PerfusionChartView->chart()->setAnimationOptions(QChart::SeriesAnimations);
    m_PerfusionChartView->chart()->legend()->show();
}
