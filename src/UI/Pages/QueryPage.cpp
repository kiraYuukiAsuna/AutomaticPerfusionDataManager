#include "QueryPage.h"

#include <QVBoxLayout>


QueryPage::QueryPage(QWidget* parent) : BasePage(parent, "查询"){
    auto *centralWidget = this->centralWidget();
    auto *centerLayout = new QVBoxLayout(centralWidget);

}

QueryPage::~QueryPage(){

}