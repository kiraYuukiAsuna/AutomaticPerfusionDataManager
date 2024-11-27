#pragma once

#include "ElaComboBox.h"
#include "ElaProgressBar.h"
#include "UI/Pages/BasePage.h"

class DataImportPage : public BasePage {
	Q_OBJECT
public:
	explicit DataImportPage(QWidget *parent = nullptr);
	~DataImportPage();

private:
	ElaComboBox* m_ImportEngineComboBox{nullptr};

	ElaProgressBar* m_ProcessBar{nullptr};

	ElaText* m_ImportStatus{nullptr};
};