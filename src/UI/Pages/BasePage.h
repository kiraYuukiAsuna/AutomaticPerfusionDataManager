#pragma once

#include <ElaScrollPage.h>

class BasePage : public ElaScrollPage {
	Q_OBJECT
public:
	explicit BasePage(QWidget* parent = nullptr, const QString& title = "");
	~BasePage();

    QWidget* centralWidget() const { return m_CentralWidget; }

private:
    QString m_Title;
    QWidget *m_CentralWidget=nullptr;
};