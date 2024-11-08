#include "OverviewPage.h"

#include <QVBoxLayout>

#include "Database/SqliteSchema.hpp"
 OverviewPage::OverviewPage(QWidget* parent) : BasePage(parent) {
     // 预览窗口标题
     setWindowTitle("Overview");

     getFluorescenceResult();

     QWidget* centralWidget = new QWidget(this);
     centralWidget->setWindowTitle("Overview");
     auto* centerLayout = new QVBoxLayout(centralWidget);
     centerLayout->setContentsMargins(0, 0, 0, 0);
     centerLayout->addWidget(m_StatusChartView);
     centerLayout->addWidget(m_FluorescenceResultChartView);

     addCentralWidget(centralWidget);
}

OverviewPage::~OverviewPage(){

}

void OverviewPage::getStatus(){
     using namespace sqlite_orm;
     auto db = SqliteSchema::getDBStorage();
     db.sync_schema();

     auto results = db.select(columns(&CellTissueInfo::Status,
                                           count(&CellTissueInfo::Status)),
                                   group_by(&CellTissueInfo::Status));

     auto series = new QPieSeries;

     int idx = 0;
     for(const auto &row : results) {
         series->append(QString::fromStdString(std::get<0>(row)), std::get<1>(row));
         std::cout << "Status: " << std::get<0>(row) << ", Count: " << std::get<1>(row) << std::endl;

         QPieSlice *slice = series->slices().at(idx);
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

     auto chart = new QChart;
     chart->addSeries(series);
     chart->setTitle("Fluorescence Result");
     chart->legend()->hide();

     m_StatusChartView = new QChartView(chart, this);
     m_StatusChartView->setRenderHint(QPainter::Antialiasing);
}

void OverviewPage::getFluorescenceResult(){
     using namespace sqlite_orm;
     auto db = SqliteSchema::getDBStorage();
     db.sync_schema();

     auto results = db.select(columns(&CellTissueInfo::FluorescenceResult,
                                           count(&CellTissueInfo::FluorescenceResult)),
                                   group_by(&CellTissueInfo::FluorescenceResult));

     auto series = new QPieSeries;

     int idx = 0;
     for(const auto &row : results) {
         series->append(QString::fromStdString(std::get<0>(row)), std::get<1>(row));
         std::cout << "FluorescenceResult: " << std::get<0>(row) << ", Count: " << std::get<1>(row) << std::endl;

         QPieSlice *slice = series->slices().at(idx);
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

     auto chart = new QChart;
     chart->addSeries(series);
     chart->setTitle("Fluorescence Result");
     chart->legend()->hide();

     m_FluorescenceResultChartView = new QChartView(chart, this);
     m_FluorescenceResultChartView->setRenderHint(QPainter::Antialiasing);
}
