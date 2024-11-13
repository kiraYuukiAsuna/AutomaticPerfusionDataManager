#include "DataImportPage.h"

#include <QVBoxLayout>

#include "ElaLineEdit.h"
#include "ElaPushButton.h"
#include "ElaText.h"
#include <QFileDialog>
#include <future>
#include <asio/post.hpp>

#include "ElaMessageBar.h"
#include "ElaMessageButton.h"
#include "DataImport/DataImporter.hpp"

DataImportPage::DataImportPage(QWidget* parent) : BasePage(parent, "数据导入") {
    auto *centralWidget = this->centralWidget();
    auto *centerLayout = new QVBoxLayout(centralWidget);

    auto* filePathSelector = new QHBoxLayout(this);
    filePathSelector->addWidget(new ElaText("路径:", this));
    auto* filePathInput = new ElaLineEdit(this);
    filePathInput->setEnabled(false);
    filePathSelector->addWidget(filePathInput);
    auto* fileSelectorButton = new ElaPushButton("选择TissueRecord文件夹", this);
    connect(fileSelectorButton, &ElaPushButton::clicked, this, [this, filePathInput](bool checked) {
        auto folderPath = QFileDialog::getExistingDirectory(this, "选择TissueRecord文件夹", QDir::homePath(), QFileDialog::ShowDirsOnly);

        filePathInput->setText(folderPath);
    });
    filePathSelector->addWidget(fileSelectorButton);

    auto* importButton = new ElaPushButton("导入数据", this);
    connect(importButton, &ElaPushButton::clicked, this, [this, filePathInput, centerLayout](bool checked) {
        auto folderPath = filePathInput->text();
        std::filesystem::path path(folderPath.toStdString());
        if (!std::filesystem::exists(path)) {
            QString errorTitle = "错误";
            QString errorMessage = "选择的TissueRecord文件夹不存在！";
            ElaMessageBar::error(ElaMessageBarType::BottomRight, errorTitle,
                errorMessage,3000, this);
            return;
        }


        ElaMessageBar::information(ElaMessageBarType::BottomRight, "提示",
            "正在导入数据，请稍后...",3000, this);


        asio::post([folderPath,this]() {
            DataImporter importer;
            importer.importRecordFromFolder(folderPath.toStdString(), [this](bool success, const std::string& message) {
                QMetaObject::invokeMethod(this, [success, message]() {
                    if (success) {
                          ElaMessageBar::success(ElaMessageBarType::BottomRight, "成功", QString::fromStdString(message), 3000);
                      } else {
                          ElaMessageBar::error(ElaMessageBarType::BottomRight, "错误", QString::fromStdString(message), 3000);
                      }
                });
            });
        });

    });

    centerLayout->addLayout(filePathSelector);
    centerLayout->addWidget(importButton);

}

DataImportPage::~DataImportPage() {}