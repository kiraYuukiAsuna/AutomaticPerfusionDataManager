#pragma once

#include "ElaComboBox.h"
#include "ElaContentDialog.h"
#include "ElaLineEdit.h"
#include "Config/Config.hpp"
#include "UI/Pages/BasePage.h"

class ConfigPage : public ElaContentDialog {
    Q_OBJECT

public:
    explicit ConfigPage(QWidget* parent = nullptr);

    ~ConfigPage();

    Config GetConfig();

    void SetConfig(const Config&config);

private:
    ElaLineEdit* m_FilePathInput{nullptr};
};
