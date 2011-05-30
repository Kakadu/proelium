#ifndef RESLOADER1_H
#define RESLOADER1_H

#include <QObject>
#include <QPixmap>
#include "reshelpers/rescontainer.h"

class ResLoader1 : public QObject
{
Q_OBJECT
public:
    static void load1(QVector<QPixmap>&, int&, int&, QString,int,int, QColor);
    static QVector<QPixmap>* load2(QString,int,int, QColor);
    static void load3(QVector<QPixmap> &, QString, int, int , QColor);
    static UnitPack* addSprites(QString prefixRun, int runN,
                                   QString prefixDeath, int deathN,
                                   QString prefixAttack, int attackN);
};

#endif // RESLOADER1_H
