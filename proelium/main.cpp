#include <QtGui/QApplication>
#include <QtCore>
#include <QBitmap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "unit.h"
#include "GameMap.h"
#include "mapdrawer.h"
//#include "fightingmodel.h"
#include "action/UserActionHypervisor.h"

extern QMap<QString, SpritesPack*> Sprites;
extern UserActionHyperVisor* MainHyperVisor;
extern QVector<QString> dirHelper;

class MyScene : public QGraphicsScene {
public:
    void keyPressEvent(QKeyEvent *event) {
        qDebug() << event->key();
        MainHyperVisor->keyPressEvent(event);
    }
};

int main(int argc, char *argv[])
{
    dirHelper << "unused" << "SW" << "S" << "SE" <<"W" <<
                 "unused"<<"E"<<"NW"<<"N"<<"NE" ;
    QApplication a(argc, argv);

    QGraphicsScene *scene = new MyScene();
    QGraphicsView view(scene,NULL);
    GameMap map(4,4);
    map.init();
    map.getSquare1(5,5)->addUnit(new Unit("Samurai",11));
    MapDrawer drawer(scene, &map);
    drawer.paintField();
    drawer.repaint();
    MainHyperVisor = new UserActionHyperVisor(&map);
    MainHyperVisor->setDrawer(&drawer);
    view.show();
    return a.exec();
}
