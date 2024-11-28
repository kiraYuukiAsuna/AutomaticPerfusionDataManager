#include "ConfigDialog.h"

#include <QFileDialog>
#include <QFormLayout>
#include <QMessageBox>
#include <QVBoxLayout>

#include "ElaLineEdit.h"
#include "ElaPushButton.h"
#include "ElaText.h"


ConfigDialog::ConfigDialog(QWidget* parent) : QDialog(parent) {
    auto&configMgr = ConfigManager::GetInstance();

    auto leftButton = new ElaPushButton("取消", this);
    connect(leftButton, &ElaPushButton::clicked, this, [this]() {
        reject();
    });

    auto middleButton = new ElaPushButton("检查路径有效性", this);
    connect(middleButton, &ElaPushButton::clicked, this, [this, &configMgr](bool checked) {
        auto config = GetConfig();
        if (!std::filesystem::exists(config.DataStoragePath)) {
            QMessageBox::critical(this, "错误", "选择的数据存储路径不存在！");
        }
        else {
            QMessageBox::information(this, "提示", "选择的数据存储路径有效！");
            configMgr.SetConfig(GetConfig());
            configMgr.WriteConfigToFile();
        }
    });

    auto rightButton = new ElaPushButton("应用", this);
    connect(rightButton, &ElaPushButton::clicked, this, [this, &configMgr](bool checked) {
        configMgr.SetConfig(GetConfig());
        configMgr.WriteConfigToFile();
        accept();
    });

    auto hLayout = new QHBoxLayout;
    hLayout->addWidget(leftButton);
    hLayout->addWidget(middleButton);
    hLayout->addWidget(rightButton);

    auto* filePathSelectorLayout = new QHBoxLayout;
    m_FilePathInput = new ElaLineEdit(this);
    filePathSelectorLayout->addWidget(m_FilePathInput);
    auto* fileSelectorButton = new ElaPushButton("选择数据存储路径", this);
    connect(fileSelectorButton, &ElaPushButton::clicked, this, [this](bool checked) {
        auto folderPath = QFileDialog::getExistingDirectory(this, "选择数据存储路径", QDir::homePath(),
                                                            QFileDialog::ShowDirsOnly);
        m_FilePathInput->setText(folderPath);
    });
    filePathSelectorLayout->addWidget(fileSelectorButton);

    auto formLayout = new QFormLayout;
    formLayout->addRow(new ElaText("数据存储路径", this), filePathSelectorLayout);

    auto* centerLayout = new QVBoxLayout;
    centerLayout->addLayout(formLayout);
    centerLayout->addLayout(hLayout);

    setLayout(centerLayout);
}

ConfigDialog::~ConfigDialog() {
}

Config ConfigDialog::GetConfig() {
    Config config;
    config.DataStoragePath = m_FilePathInput->text().toStdString();
    return config;
}

void ConfigDialog::SetConfig(const Config&config) {
    m_FilePathInput->setText(QString::fromStdString(config.DataStoragePath.string()));
}
