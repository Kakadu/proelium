#ifndef USERACTIONHYPERVISOR_H
#define USERACTIONHYPERVISOR_H

#include <QObject>
#include <QKeyEvent>
#include <QLinkedList>
#include "GameMap.h"
#include "stuff/abstractions.h"
#include "stuff/consts.h"

extern QVector<QString> dirHelper;

/**
 * This class will dispatch user keyboard and mouse actions to game model
 */
class UserActionHyperVisor : public QObject, public Move::MoveVisitor
{
    Q_OBJECT
private:
    class SetMaxPointsVisitor: public UnitVisitor {
    public:
        void visit(Unit* u) {
            u->setMaxMovePoints();
        }
    };

    QVector<Triple<Unit*,int,int> > _unitsList;
    GameMap* _map;
    Unit* _curUnit;
    AbstractDrawer* _drawer;
    void extractUnits(int nationId);
    QLinkedList<Game::Nations> _playableOwners;
    int curPlayer;

public:
    UserActionHyperVisor(GameMap* map, QObject* parent = NULL);
    void nextPlayer() {
        curPlayer++;
        curPlayer %= _playableOwners.count();
    }

    inline void setDrawer(AbstractDrawer* d) { _drawer = d; }
    void setCurUnit(Unit* u) {
        _curUnit = u;
    }
    Unit* getCurUnit() {
        return _curUnit;
    }
    void keyPressEvent(QKeyEvent *event) {
        Q_ASSERT(_unitsList[0].a() == _curUnit);
        qDebug() << event->key();
        int curkey = event->key();
        if ((curkey >= Qt::Key_1) && (curkey <= Qt::Key_9) && (_curUnit != NULL)) {
            int dir = curkey - 0x30;

            Game::Direction d = (Game::Direction)dir;
            Move::MoveResult* ans = _map->tryMove(_curUnit, d);
            ans->accept(*this);
            if (ans->targetX!=-1 && ans->targetY!=-1 && _curUnit->canMove()) {
                _drawer->showCursorSprite(ans->targetX,ans->targetY);
            }
            delete ans;

            if (! _curUnit->canMove()) {
                Q_ASSERT(_unitsList[0].a() == _curUnit);
                _unitsList.remove(0);
                if (_unitsList.count() == 0) {
                    qDebug() << "next move";
                    nextPlayer();
                    extractUnits(curPlayer);
                    Q_ASSERT(_unitsList.count()>0);
                    for (int i=0; i<_unitsList.count(); ++i)
                        _unitsList[i].a()->setMaxMovePoints();
                    _curUnit = _unitsList[0].a();
                }
            }

        }
    }
    virtual ~UserActionHyperVisor() {}

    virtual void visit(Move::FightMove &) {
        qDebug() << "fight!";
        _drawer->repaint();
    }
    virtual void visit(Move::SimpleMove &) {
        qDebug() << "move!";
        _drawer->repaint();
    }
    virtual void visit(Move::NoMove &) {
        qDebug() << "no move!";
    }
public slots:
    void endMove() {
        SetMaxPointsVisitor v;
        _map->visitUnits(v);
    }
};

#endif // USERACTIONHYPERVISOR_H
