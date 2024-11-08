#include "BasePage.h"

#include <QHBoxLayout>

#include "ElaMenu.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaToolButton.h"

BasePage::BasePage(QWidget* parent)    : ElaScrollPage(parent){
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=]() {
        if (!parent)
        {
            update();
        }
    });
}

BasePage::~BasePage(){
}
