#include "propertiesdialog.h"
#include "ui_propertiesdialog.h"

PropertiesDialog::PropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PropertiesDialog)
{
    ui->setupUi(this);
    controller = NULL;
    QSettings settings;
    i_darkTheme = darkTheme = settings.value("colorScheme",0).toBool();
    if(i_darkTheme) ui->ThemeBox->setChecked(true);
    i_invertMouse = invertMouse = settings.value("invertMouse",false).toBool();
    if(i_invertMouse) ui->InvertMouseBox->setChecked(true);
    i_color = color = settings.value("color",QColor(Qt::green)).value<QColor>();
    i_badColor = badColor = settings.value("badColor",QColor(Qt::red)).value<QColor>();
    i_memLimit = memLimit = settings.value("sizeLimit", HISTORY_LIMIT).toInt();

    ui->MemLimBox->setValue(i_memLimit);
    ui->MemLimBox->setSuffix(" MB");

    colMap = QPixmap(32,32);
    colMap.fill(i_color);
    ui->colorLabel->setPixmap(colMap);

    badColMap = QPixmap(32,32);
    badColMap.fill(i_badColor);
    ui->badColorLabel->setPixmap(badColMap);

    connect(ui->ThemeBox, SIGNAL(stateChanged(int)), this, SLOT(toggleScheme()));
    connect(ui->InvertMouseBox, SIGNAL(stateChanged(int)), this, SLOT(toggleInvertMouse()));
    connect(ui->MemLimBox, SIGNAL(valueChanged(int)), this, SLOT(setMemLimit(int)));
    connect(ui->colorButton, SIGNAL(clicked()), this, SLOT(setColor()));
    connect(ui->badColorButton, SIGNAL(clicked()), this, SLOT(setBadColor()));

    connect(this, SIGNAL(schemeSignal()), parent, SLOT(toggleColorScheme()));
    connect(this, SIGNAL(mouseInvertSignal()), parent, SLOT(toggleMouseInvert()));
    connect(this, SIGNAL(finished (int)), this, SLOT(finished(int)));
}

PropertiesDialog::~PropertiesDialog()
{
    delete ui;
}

void PropertiesDialog::setController(admeshController *cnt)
{
    controller = cnt;
}

void PropertiesDialog::setMemLimit(int val)
{
    memLimit = val;
}

void PropertiesDialog::toggleScheme()
{
    darkTheme = !darkTheme;
}

void PropertiesDialog::toggleInvertMouse()
{
    invertMouse = !invertMouse;
}

void PropertiesDialog::setColor()
{    
    QColor tmp  = QColorDialog::getColor(color, this);
    if(tmp.isValid()) color = tmp;
    colMap.fill(color);
    ui->colorLabel->setPixmap(colMap);
}

void PropertiesDialog::setBadColor()
{
    QColor tmp  = QColorDialog::getColor(badColor, this);
    if(tmp.isValid()) badColor = tmp;
    badColMap.fill(badColor);
    ui->badColorLabel->setPixmap(badColMap);
}

void PropertiesDialog::finished(int val)
{
    if(val == QDialog::Accepted){
        QSettings settings;
        if(darkTheme != i_darkTheme) {
            schemeSignal();
            settings.setValue("colorScheme", darkTheme);
        }
        if((color != i_color || badColor != i_badColor) && controller) {
            settings.setValue("color", color);
            settings.setValue("badColor", badColor);
            controller->setDrawColor(color, badColor);
        }
        if((memLimit != i_memLimit) && controller){
            controller->setHistoryLimit(memLimit);
            settings.setValue("sizeLimit", memLimit);
        }
        if(invertMouse != i_invertMouse){
            mouseInvertSignal();
            settings.setValue("invertMouse", invertMouse);
        }
    }
}
