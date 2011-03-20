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

extern QMap<QString, SpritesPack*> Sprites;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    srand(time(NULL));
    MainWindow w;
    w.setWindowState(Qt::WindowMaximized);
    //GameMap m(9,8);

    GameMap m(11,7);
    m.init();
    QGraphicsScene* sc = w.getScene();
    MapDrawer drawer(sc,&m);

    drawer.paintField();
    drawer.placeArmies();
    drawer.repaint();

    ModelParam param;
//    SimpleFightingModel model(&m);
    MainFightingModel model(&m, &param);

    QObject::connect(&model,SIGNAL(action(AbstractUnitAction*)),
		     &drawer,SLOT(applyAction(AbstractUnitAction*)) );

    QObject::connect(&drawer, SIGNAL(continueModel()),
		     &model, SLOT(next()) );

    model.next();
    w.show();
    return a.exec();
}
