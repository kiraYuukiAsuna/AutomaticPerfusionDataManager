#include "DataImportPage.h"

#include <QVBoxLayout>

#include "ElaLineEdit.h"
#include "ElaPushButton.h"
#include "ElaText.h"
#include <QFileDialog>
#include <future>
#include <QFormLayout>
#include <asio/post.hpp>

#include "ElaComboBox.h"
#include "ElaMessageBar.h"
#include "ElaMessageButton.h"
#include "ElaProgressBar.h"
#include "DataImport/DataImporter.hpp"

DataImportPage::DataImportPage(QWidget* parent) : BasePage(parent, "数据导入") {
    auto* centerLayout = new QVBoxLayout;

    auto* filePathSelectorLayout = new QHBoxLayout(this);
    auto* filePathInput = new ElaLineEdit(this);
    filePathSelectorLayout->addWidget(filePathInput);
    auto* fileSelectorButton = new ElaPushButton("选择TissueRecord文件夹", this);
    connect(fileSelectorButton, &ElaPushButton::clicked, this, [this, filePathInput](bool checked) {
        auto folderPath = QFileDialog::getExistingDirectory(this, "选择TissueRecord文件夹", QDir::homePath(),
                                                            QFileDialog::ShowDirsOnly);

        filePathInput->setText(folderPath);
    });
    filePathSelectorLayout->addWidget(fileSelectorButton);

    m_ImportEngineComboBox = new ElaComboBox(this);
    m_ImportEngineComboBox->addItem("Default");

    auto* importButton = new ElaPushButton("导入数据", this);
    importButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(importButton, &ElaPushButton::clicked, this, [this, filePathInput, centerLayout](bool checked) {
        auto folderPath = filePathInput->text();
        std::filesystem::path path(folderPath.toStdString());
        if (!std::filesystem::exists(path)) {
            QString errorTitle = "错误";
            QString errorMessage = "选择的TissueRecord文件夹不存在！";
            ElaMessageBar::error(ElaMessageBarType::BottomRight, errorTitle,
                                 errorMessage, 3000, this);
            return;
        }

        m_ProcessBar->setValue(0);
        m_ImportStatus->setText("正在导入数据，请稍后...");

        ElaMessageBar::information(ElaMessageBarType::BottomRight, "提示",
                                   "正在导入数据，请稍后...", 3000, this);


        asio::post([folderPath,this]() {
            DataImporter importer;
            importer.importRecordFromFolder(folderPath.toStdString(),
                                            [this](bool finished, float progress, bool status, const std::string&message) {
                                                if (finished) {
                                                    QMetaObject::invokeMethod(this, [status, message, this]() {
                                                        if (status) {
                                                            ElaMessageBar::success(
                                                                ElaMessageBarType::BottomRight, "成功",
                                                                QString::fromStdString(message), 3000);
                                                        }
                                                        else {
                                                            ElaMessageBar::error(
                                                                ElaMessageBarType::BottomRight, "错误",
                                                                QString::fromStdString(message), 3000);
                                                        }

                                                        m_ImportStatus->setText(QString::fromStdString(message));
                                                    });
                                                }else {
                                                    QMetaObject::invokeMethod(this, [status, message, this, progress]() {
                                                        m_ProcessBar->setValue((floor(progress*100)));
                                                        m_ImportStatus->setText(QString::fromStdString(message));
                                                    });
                                                }
                                            });
        });
    });

    m_ProcessBar = new ElaProgressBar(this);
    m_ProcessBar->setMinimum(0);
    m_ProcessBar->setMaximum(100);
    m_ProcessBar->setValue(0);

    m_ImportStatus = new ElaText("等待导入", this);

    auto formLayout = new QFormLayout(this);
    formLayout->addRow(new ElaText("选择TissueRecord文件夹导入数据，已经导入的数据会自动忽略。", this));
    formLayout->addRow(new ElaText("TissueRecord路径:", this), filePathSelectorLayout);
    formLayout->addRow(new ElaText("数据导入引擎", this), m_ImportEngineComboBox);
    formLayout->addRow(new ElaText("操作:", this), importButton);
    formLayout->addRow(new ElaText("导入进度:", this), m_ProcessBar);
    formLayout->addRow(new ElaText("导入状态:", this), m_ImportStatus);

    centerLayout->addLayout(formLayout);

    auto* centralWidget = this->centralWidget();
    centralWidget->setLayout(centerLayout);
}

DataImportPage::~DataImportPage() {
}
