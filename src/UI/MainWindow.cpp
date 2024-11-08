#include "MainWindow.h"
#include "ElaContentDialog.h"
#include "ElaEventBus.h"
#include "ElaText.h"
#include "Database/SqliteSchema.hpp"

MainWindow::MainWindow(QWidget *parent) :
    ElaWindow(parent) {
    initWindow();

    //额外布局
    initEdgeLayout();

    //中心窗口
    initContent();

    // 拦截默认关闭事件
    m_CloseDialog = new ElaContentDialog(this);
    connect(m_CloseDialog, &ElaContentDialog::rightButtonClicked, this, &MainWindow::closeWindow);
    connect(m_CloseDialog, &ElaContentDialog::middleButtonClicked, this, &MainWindow::showMinimized);
    this->setIsDefaultClosed(false);
    connect(this, &MainWindow::closeButtonClicked, this, [=]() {
        m_CloseDialog->exec();
    });

    //移动到中心
    moveToCenter();
}

MainWindow::~MainWindow() {

}

void MainWindow::initWindow(){
    setUserInfoCardVisible(false);
}

void MainWindow::initEdgeLayout(){

}

void MainWindow::initContent(){
    QString _settingKey{""};

    m_OverviewPage = new OverviewPage(this);
    addPageNode("Overview Page", m_OverviewPage, ElaIconType::House);
    addFooterNode("About", nullptr, _settingKey, 0, ElaIconType::User);
    qDebug() << "已注册的事件列表" << ElaEventBus::getInstance()->getRegisteredEventsName();

}
