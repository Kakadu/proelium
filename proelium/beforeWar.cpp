#include "beforeWar.h"
#include "ui_beforeWar.h"
#include <Qt3Support/q3canvas.h>
#include <QMovie>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QGraphicsPixmapItem>

BeforeWar::BeforeWar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BeforeWar)
{
    ui->setupUi(this);
    QObject::connect(ui->buttonBox, SIGNAL(accepted()),this,SLOT(accept()));
    QObject::connect(ui->SurrenderAtRadio,SIGNAL(clicked(bool)),
		     this,SLOT(surrenderAtRadioChanged(bool)));
    QObject::connect(ui->NeverSurrenderRadio,SIGNAL(clicked(bool)),
		     this,SLOT(neverSurrenderRadioChanged(bool)));
}

BeforeWar::~BeforeWar() {
    delete ui;
}

void BeforeWar::changeEvent(QEvent *e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
