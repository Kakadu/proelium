#include <QtGui/QApplication>
#include "mainwindow.h"
#include "framecollection.h"
#include "unit.h"
#include "GameMap.h"
#include <QtCore>
#include <QBitmap>
#include <QPainter>
#include "mapdrawer.h"
#include "action/action.h"
#include "mainwindow.h"
#include "fightingmodel.h"
#include "reshelpers/rescontainer.h"
#include <ctime>
#include "beforeWar.h"
#include "mainhelper.h"

extern QMap<QString, SpritesPack*> Sprites;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    srand(time(NULL));
    MainHelper m;
    return a.exec();
}
