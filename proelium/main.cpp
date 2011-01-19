#include <QtGui/QApplication>
#include "mainwindow.h"
#include "framecollection.h"
#include "unit.h"
#include <QtCore>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    FrameCollection tankAttackSprites(QObject::tr("./tank_att/TankAttackA_E_"),20);
    qDebug() << "frames created" << endl;
    Unit tankUnit(&tankAttackSprites, w.createBattleItem() );
    tankUnit.setAttackFrame(0);

    w.show();
    tankUnit.animate(0);
    return a.exec();
}
