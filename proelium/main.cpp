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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowState(Qt::WindowMaximized);
/*    QGraphicsPixmapItem* item = w.createBattleItem();
    QPixmap* p = new QPixmap("irrigation_plains2.bmp");
    //const QBitmap mask = p-> createHeuristicMask();
    const QBitmap mask = p-> createMaskFromColor(QColor(255,0,255));

    p->setMask(mask);

    QPixmap p2 = p->copy(0,0,128,64);
    //const QBitmap mask = p-> createMaskFromColor(QColor(0,255,0));
    item->setPixmap(p2);


    FrameCollection tankAttackSprites(QObject::tr("./tank_att/TankAttackA_E_"),20);
    Unit tankUnit(&tankAttackSprites, w.createBattleItem() );
    tankUnit.setAttackFrame(0);
    tankUnit.animate(0);
*/
    //GameMap m(9,8);

    GameMap m(7,6);
    m.init();
    QGraphicsScene* sc = w.getScene();
    MapDrawer drawer(sc,&m);

    drawer.paintField();
    drawer.placeArmies();
    drawer.repaint();

    SimpleFightingModel model(&m);

    //sc->invalidate(0,0,50,50);
    QObject::connect(&model,SIGNAL(action(AbstractUnitAction*)),
		     &drawer,SLOT(applyAction(AbstractUnitAction*)) );

    QObject::connect(&drawer, SIGNAL(continueModel()),
		     &model, SLOT(next()) );

    model.next();
    w.show();
    return a.exec();
}
