#include "ConfigPage.h"

#include <QFileDialog>
#include <QFormLayout>
#include <QVBoxLayout>

#include "ElaLineEdit.h"
#include "ElaPushButton.h"
#include "ElaText.h"


ConfigPage::ConfigPage(QWidget* parent) : ElaContentDialog(parent) {
    setLeftButtonText("检查路径有效性");
    setMiddleButtonText("取消");
    setRightButtonText("确定");

    auto* filePathSelectorLayout = new QHBoxLayout(this);
    m_FilePathInput = new ElaLineEdit(this);
    filePathSelectorLayout->addWidget(m_FilePathInput);
    auto* fileSelectorButton = new ElaPushButton("选择数据存储路径", this);
    connect(fileSelectorButton, &ElaPushButton::clicked, this, [this](bool checked) {
        auto folderPath = QFileDialog::getExistingDirectory(this, "选择数据存储路径", QDir::homePath(), QFileDialog::ShowDirsOnly);
        m_FilePathInput->setText(folderPath);
    });
    filePathSelectorLayout->addWidget(fileSelectorButton);

    auto formLayout = new QFormLayout;
    formLayout->addRow(new ElaText("数据存储路径", this), filePathSelectorLayout);

    auto *centerLayout = new QVBoxLayout;
    centerLayout->addLayout(formLayout);

    auto *centralWidget = new QWidget(this);
    centralWidget->setLayout(centerLayout);
    setCentralWidget(centralWidget);
}

ConfigPage::~ConfigPage() {
}

Config ConfigPage::GetConfig() {
    Config config;
    config.DataStoragePath = m_FilePathInput->text().toStdString();
    return config;
}

void ConfigPage::SetConfig(const Config& config) {
    m_FilePathInput->setText(QString::fromStdString(config.DataStoragePath.string()));
}
