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

extern QMap<QString, SpritesPack*> Sprites;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowState(Qt::WindowMaximized);
    //GameMap m(9,8);

    GameMap m(7,6);
    m.init();
    QGraphicsScene* sc = w.getScene();
    MapDrawer drawer(sc,&m);

    drawer.paintField();
    drawer.placeArmies();
    drawer.repaint();

    SimpleFightingModel2 model(&m);


    QObject::connect(&model,SIGNAL(action(AbstractUnitAction*)),
		     &drawer,SLOT(applyAction(AbstractUnitAction*)) );

    QObject::connect(&drawer, SIGNAL(continueModel()),
		     &model, SLOT(next()) );

    model.next();
    w.show();
    return a.exec();
}
