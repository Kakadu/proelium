#ifndef ABSTRACTIONS_H
#define ABSTRACTIONS_H

#include <QDebug>

class AbstractDrawer
{
    //Q_OBJECT
public:
     virtual void repaint() {
        qDebug() << "AbstractDrawer::repaint\nIf u see this message You'll find bug in code";
     }
};

#endif // ABSTRACTIONS_H
