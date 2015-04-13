#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QColorDialog>
#include "data.h"
#include "admeshcontroller.h"

namespace Ui {
class PropertiesDialog;
}

class PropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PropertiesDialog(QWidget *parent = 0);
    ~PropertiesDialog();
    void setController(admeshController *cnt);

public slots:
    void setMemLimit(int val);
    void toggleScheme();
    void toggleInvertMouse();
    void setColor();
    void setBadColor();
    void finished(int);

signals:
    void schemeSignal();
    void mouseInvertSignal();

private:
    Ui::PropertiesDialog *ui;
    admeshController *controller;
    QPixmap colMap;
    QPixmap badColMap;
    QColor color;
    QColor i_color;
    QColor badColor;
    QColor i_badColor;
    int memLimit;
    int i_memLimit;
    bool invertMouse;
    bool i_invertMouse;
    bool darkTheme;
    bool i_darkTheme;
};

#endif // PROPERTIESDIALOG_H
