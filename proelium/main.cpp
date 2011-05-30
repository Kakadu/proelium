#include <QtGui/QApplication>
#include <QtCore>
#include <QBitmap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "unit.h"
#include "GameMap.h"
#include "mapdrawer.h"
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
    //auto traceLambda = [] (int _val) -> int { return _val+1; };

    dirHelper << "unused" << "SW" << "S" << "SE" <<"W" <<
                 "unused"<<"E"<<"NW"<<"N"<<"NE" ;
    QApplication a(argc, argv);

    QGraphicsScene *scene = new MyScene();
    QGraphicsView view(scene,NULL);
    GameMap map(4,4);
    map.init();
    map.getSquare1(5,5)->addUnit(new Unit("Samurai",11, 1));
    map.getSquare1(3,3)->addUnit(new Unit("AfricanChampion",43,2));
    map.getSquare1(3,7)->addUnit(new Unit("AfricanChampion",44,2));
    map.getSquare1(7,3)->addUnit(new Unit("AfricanChampion",45,2));
    map.getSquare1(7,7)->addUnit(new Unit("AfricanChampion",46,2));
    MapDrawer drawer(scene, &map);
    drawer.paintField();
    drawer.repaint();
    MainHyperVisor = new UserActionHyperVisor(&map);
    MainHyperVisor->setDrawer(&drawer);
    view.show();
    return a.exec();
}
