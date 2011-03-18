#include "anigroup.h"

AniGroup::AniGroup(QObject *parent) : QObject(parent) {
    seq = new QSequentialAnimationGroup(this);
    par = new QParallelAnimationGroup(this);
    _timer = NULL;
    QObject::connect(seq,SIGNAL(finished()), this, SLOT(aniEnded()));
    QObject::connect(par,SIGNAL(finished()), this, SLOT(aniEnded()));
}
