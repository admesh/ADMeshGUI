#ifndef ADMESHCONTROLLER_H
#define ADMESHCONTROLLER_H

#include <QObject>
#include "data.h"

class admeshController : public QObject
{
    Q_OBJECT
public:
    explicit admeshController(QObject *parent = 0);
    ~admeshController();
    stl_file* getSTLPointer();

signals:
    void reDrawSignal();

public slots:
    void openSTL();
    void openSTLbyName(const char* file);

private:
    stl_file* stl;
    stl_file* active;
};

#endif // ADMESHCONTROLLER_H
