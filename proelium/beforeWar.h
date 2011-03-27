#ifndef BEFOREWAR_H
#define BEFOREWAR_H
#include "ui_beforeWar.h"
#include <QGraphicsScene>
#include <QMainWindow>

namespace Ui {
    class BeforeWar;
}

class BeforeWar : public QDialog {
    Q_OBJECT
public:
    BeforeWar(QWidget *parent = 0);
    ~BeforeWar();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::BeforeWar *ui;

private slots:
    void surrenderAtRadioChanged(bool b) {
	ui->surrenderSpin->setEnabled(b);
    }
    void neverSurrenderRadioChanged(bool b) {
	ui->surrenderSpin->setEnabled(!b);
    }

};

#endif
