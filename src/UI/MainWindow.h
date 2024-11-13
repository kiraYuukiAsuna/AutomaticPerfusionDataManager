#pragma once

#include "ElaWindow.h"
#include "Pages/DataImportPage.h"
#include "Pages/OverviewPage.h"
#include "Pages/QueryPage.h"

class ElaContentDialog;
class MainWindow : public ElaWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow() override;

	void initWindow();
	void initEdgeLayout();
	void initContent();

private:
	ElaContentDialog* m_CloseDialog{nullptr};

	OverviewPage* m_OverviewPage{nullptr};
    DataImportPage* m_DataImportPage{nullptr};
    QueryPage* m_QueryPage{nullptr};
};
