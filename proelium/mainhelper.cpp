#include "mainhelper.h"
#include "beforeWar.h"
#include "mainwindow.h"

MainHelper::MainHelper(QObject *parent) :
    QObject(parent)
{
    dialog = new BeforeWar(NULL);
    w = new MainWindow(NULL);
    w->setWindowState(Qt::WindowMaximized);

    QObject::connect(dialog,SIGNAL(accepted()),this,SLOT(showMainWindow()));
    dialog->show();

}
