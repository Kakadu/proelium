#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "action/UserActionHypervisor.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::setScene(QGraphicsScene *sc) {
    ui->canvas->setScene(sc);
}
void MainWindow::connectHyperVisor(UserActionHyperVisor &v) {
    QObject::connect(ui->endMoveButton, SIGNAL(clicked()),
                     &v, SLOT(endMove()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
