#pragma once

#include "ElaComboBox.h"
#include "ElaContentDialog.h"
#include "ElaLineEdit.h"
#include "Config/Config.hpp"
#include "UI/Pages/BasePage.h"

class ConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget* parent = nullptr);

    ~ConfigDialog();

    Config GetConfig();

    void SetConfig(const Config&config);

private:
    ElaLineEdit* m_FilePathInput{nullptr};
};
