// (c) 2015 David Vyvlečka, AGPLv3

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

/*!
 * \brief Properties window.
 *
 * Used to set values (e.g. inverted mouse, history buffer limit, theme,...) .
 */
class PropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Constructor.
     */
    explicit PropertiesDialog(QWidget *parent = 0);
    /*!
     * \brief Destructor.
     */
    ~PropertiesDialog();
    /*!
     * \brief Set controller to handle operations.
     */
    void setController(admeshController *cnt);

public slots:
    /*!
     * \brief Set changed memory limit value.
     */
    void setMemLimit(int val);
    /*!
     * \brief Set changed theme value.
     */
    void toggleScheme();
    /*!
     * \brief Set changed inverted mouse value.
     */
    void toggleInvertMouse();
    /*!
     * \brief Set changed color value.
     */
    void setColor();
    /*!
     * \brief Set changed bad color value.
     */
    void setBadColor();
    /*!
     * \brief Set default color value.
     */
    void setDefaultColor();
    /*!
     * \brief Set default bad color value.
     */
    void setDefaultBadColor();
    /*!
     * \brief Handle dialog finished.
     *
     * Upon succes (OK selected) change desired values to new values.
     * Changes are based on difference between new values set in dialog and old values read from configuration.
     */
    void finished(int);

signals:
    /*!
     * \brief Send signal to change scheme of main Window.
     */
    void schemeSignal();
    /*!
     * \brief Send signal to invert mouse in RenderingWidget.
     */
    void mouseInvertSignal();

private:
    Ui::PropertiesDialog *ui;       ///< Pointer to iterface of dialog.
    admeshController *controller;   ///< Controller used to process changes.
    QPixmap colMap;                 ///< Color map. Displays color in dialog.
    QPixmap badColMap;              ///< Bad color map. Displays bad color in dialog.
    QColor color;                   ///< Color.
    QColor i_color;                 ///< Color upon initialization.
    QColor badColor;                ///< Bad color.
    QColor i_badColor;              ///< Bad color upon initialization.
    int memLimit;                   ///< Memory limit.
    int i_memLimit;                 ///< Memory limit upon initialization.
    bool invertMouse;               ///< Inverted mouse yes/no.
    bool i_invertMouse;             ///< Inverted mouse yes/no upon initialization.
    bool darkTheme;                 ///< Dark theme yes/no.
    bool i_darkTheme;               ///< Dark theme yes/no upon initialization.
};

#endif // PROPERTIESDIALOG_H
