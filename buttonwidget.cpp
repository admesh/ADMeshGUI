#include "buttonwidget.h"

ButtonWidget::ButtonWidget(QWidget *parent) :
    QWidget(parent)
{
}


void ButtonWidget::toggleVisible(bool val)
{
    if(val)this->hide();
    else this->show();
}
