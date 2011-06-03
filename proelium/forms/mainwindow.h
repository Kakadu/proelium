#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include "action/UserActionHypervisor.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setScene(QGraphicsScene* sc);
    void connectHyperVisor(UserActionHyperVisor &v);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
