#include "BasePage.h"

#include <QHBoxLayout>

#include "ElaMenu.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaToolButton.h"

BasePage::BasePage(QWidget* parent, const QString& title) : ElaScrollPage(parent) {
	connect(eTheme, &ElaTheme::themeModeChanged, this, [=]() {
		if (!parent) {
			update();
		}
	});

    m_Title = title;
    if(m_Title.isEmpty()) {
        m_Title = "Unknown Page";
    }

    m_CentralWidget = new QWidget(this);
    m_CentralWidget->setWindowTitle(m_Title);

    addCentralWidget(m_CentralWidget);
}

BasePage::~BasePage() {}
