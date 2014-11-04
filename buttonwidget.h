#ifndef BUTTONWIDGET_H
#define BUTTONWIDGET_H

#include <QWidget>

class ButtonWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ButtonWidget(QWidget *parent = 0);

signals:

public slots:
    void toggleVisible(bool val);

};

#endif // BUTTONWIDGET_H
