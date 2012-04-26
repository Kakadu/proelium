#include <QtGui/QApplication>
#include <QtCore>
#include <QBitmap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "forms/mainwindow.h"
#include "unit.h"
#include "GameMap.h"
#include "mapdrawer.h"
#include "action/UserActionHyperVisor.h"

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
    MainWindow m;
    m.setGeometry(100,100,900,600);
    m.setWindowState(Qt::WindowMaximized);
    m.show();

    QGraphicsScene* scene;
    m.setScene(scene = new MyScene());

    GameMap map(4,4);
    map.init();
    map.getSquare1(5,5)->addUnit(new Unit("Samurai",11, Game::Russia));

    map.getSquare1(3,3)->addUnit(new Unit("AfricanChampion",43,Game::Barbarians) );
    map.getSquare1(3,7)->addUnit(new Unit("AfricanChampion",44,Game::Barbarians) );
    map.getSquare1(7,3)->addUnit(new Unit("AfricanChampion",45,Game::Barbarians) );
    map.getSquare1(7,7)->addUnit(new Unit("AfricanChampion",46,Game::Barbarians) );

    MapDrawer drawer(scene, &map);
    drawer.paintField();
    drawer.repaint();

    drawer.showCursorSprite(5,5);
    MainHyperVisor = new UserActionHyperVisor(&map);
    MainHyperVisor->setDrawer(&drawer);
    m.connectHyperVisor(*MainHyperVisor);


    return a.exec();
}
