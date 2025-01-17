#include "MainWindow.h"

#include "Database/SqliteSchema.hpp"
#include "ElaContentDialog.h"
#include "ElaEventBus.h"
#include "ElaMessageBar.h"
#include <QMessageBox>
#include "Config/Config.hpp"
#include "Database/CellInformationRepository.hpp"
#include "Pages/ConfigDialog.h"
#include "Pages/DataImportPage.h"

MainWindow::MainWindow(QWidget* parent) : ElaWindow(parent) {
    initWindow();

    // 额外布局
    initEdgeLayout();

    // 中心窗口
    initContent();

    // 拦截默认关闭事件
    m_CloseDialog = new ElaContentDialog(this);
    connect(m_CloseDialog, &ElaContentDialog::rightButtonClicked, this,
            &MainWindow::closeWindow);
    connect(m_CloseDialog, &ElaContentDialog::middleButtonClicked, this,
            &MainWindow::showMinimized);
    this->setIsDefaultClosed(false);
    connect(this, &MainWindow::closeButtonClicked, this,
            [=]() { m_CloseDialog->exec(); });

    // 移动到中心
    moveToCenter();

    auto&configMgr = ConfigManager::GetInstance();
    configMgr.ReadConfigFromFile();
    auto config = configMgr.GetConfig();

    do {
        ConfigDialog configPage;
        configPage.SetConfig(config);
        configPage.exec();
        config = configMgr.GetConfig();
    }
    while (!std::filesystem::exists(config.DataStoragePath));

    if (!std::filesystem::exists(CellInformationRepository::GetCellInformationRepositoryPath())) {
        std::filesystem::create_directories(CellInformationRepository::GetCellInformationRepositoryPath());
    }
}

MainWindow::~MainWindow() {
}

void MainWindow::initWindow() { setUserInfoCardVisible(false); }

void MainWindow::initEdgeLayout() {
}

void MainWindow::initContent() {
    QString _settingKey{""};

    m_DataImportPage = new DataImportPage(this);
    addPageNode("数据导入页面", m_DataImportPage, ElaIconType::Database);

    m_OverviewPage = new OverviewPage(this);
    addPageNode("总览页面", m_OverviewPage, ElaIconType::House);

    m_QueryPage = new QueryByTimePage(this);
    addPageNode("查询页面", m_QueryPage, ElaIconType::Browser);

    addFooterNode("About", nullptr, _settingKey, 0, ElaIconType::Info);
}
