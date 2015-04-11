#ifndef DATA_H
#define DATA_H

#include <admesh/stl.h>
#include <stlsplit.h>
#include <QVector3D>
#include <libintl.h>
#include <locale.h>
#include <QString>

/**
This part until the END TAG was taken from OpenSCAD source code. Last entry 16. 3. 2015.
Original source code accessible on:
https://github.com/openscad/openscad/blob/master/src/qtgettext.h
https://github.com/openscad/openscad/blob/master/src/printutils.h#L10
*/
#define N_(String) String
inline char * _( const char * msgid ) { return gettext( msgid ); }
inline QString _( const char *msgid, int category )
{
    Q_UNUSED( category );
    return QString::fromUtf8( _( msgid ) );
}
/** END TAG */


#define DEFAULT_RES_X 960
#define DEFAULT_RES_Y 725

#define AXIS_SIZE 500000.0f
#define GRID_SIZE 10.0f
#define MIN_ZOOM 0.01f
#define MAX_ZOOM 50000.0f
#define ZOOM_SPEED 10.0f
#define PERSPECTIVE 50.0f
#define MIN_VIEW_DISTANCE 0.1f
#define MAX_VIEW_DISTANCE 500000.0f
#define ITEMS_LIMIT 16646655 // 255 + 255*255 + 255*255*255

#define GREEN QVector3D(0.0,1.0,0.0)
#define RED QVector3D(1.0,0.0,0.0)
#define BLUE QVector3D(0.0,0.0,1.0)
#define BLACK QVector3D(0.0,0.0,0.0)
#define GREY QVector3D(0.7,0.7,0.7)

#define SOLID_SHORTCUT 'S'
#define WIREFRAME_SHORTCUT 'W'
#define EDGES_SHORTCUT 'E'
#define AXES_SHORTCUT 'A'
#define GRID_SHORTCUT 'G'
#define INFO_SHORTCUT 'I'
#define RESET_SHORTCUT 'R'
#define EXPORT_SHORTCUT Qt::CTRL + Qt::Key_E
#define FRONT_SHORTCUT Qt::Key_5 + Qt::KeypadModifier
#define BACK_SHORTCUT Qt::Key_0 + Qt::KeypadModifier
#define LEFT_SHORTCUT Qt::Key_4 + Qt::KeypadModifier
#define RIGHT_SHORTCUT Qt::Key_6 + Qt::KeypadModifier
#define TOP_SHORTCUT Qt::Key_8 + Qt::KeypadModifier
#define BOTTOM_SHORTCUT Qt::Key_2 + Qt::KeypadModifier

#define HISTORY_LIMIT 100000000

#endif // DATA_H
