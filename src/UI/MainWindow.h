#pragma once

#include "ElaWindow.h"
#include "Pages/OverviewPage.h"

class ElaContentDialog;class MainWindow : public ElaWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void initWindow();
    void initEdgeLayout();
    void initContent();

private:
    ElaContentDialog* m_CloseDialog{nullptr};

    OverviewPage* m_OverviewPage{nullptr};
};
