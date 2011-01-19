#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Qt3Support/q3canvas.h>
#include <QMovie>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QGraphicsPixmapItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->scene = new QGraphicsScene;
    this->view  = (ui->graphicsView);
    view->setScene(scene);
}

QGraphicsPixmapItem* MainWindow::createBattleItem() {
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem();
    scene->addItem(item);
    return item;
}
inline QGraphicsScene* MainWindow::getScene() {
    return scene;
}

MainWindow::~MainWindow() {    
    delete ui;
}

void MainWindow::changeEvent(QEvent *e) {
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
