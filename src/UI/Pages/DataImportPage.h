#pragma once

#include "ElaComboBox.h"
#include "UI/Pages/BasePage.h"

class DataImportPage : public BasePage {
	Q_OBJECT
public:
	explicit DataImportPage(QWidget *parent = nullptr);
	~DataImportPage();

private:
	ElaComboBox* m_ImportEngineComboBox{nullptr};
};