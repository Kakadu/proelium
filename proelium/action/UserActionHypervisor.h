#ifndef USERACTIONHYPERVISOR_H
#define USERACTIONHYPERVISOR_H
#include <QKeyEvent>
#include "GameMap.h"
#include "unit.h"
#include "stuff/abstractions.h"

/**
 * This class will dispatch user keyboard and mouse actions to game model
 */
class UserActionHyperVisor : public QObject
{
    Q_OBJECT
private:
    GameMap* _map;
    Unit* _curUnit;
    AbstractDrawer* _drawer;
public:
    UserActionHyperVisor(GameMap* map) : _map(map) {

    }
    inline void setDrawer(AbstractDrawer* d) { _drawer = d; }
    void setCurUnit(Unit* u) {
        _curUnit = u;
    }
    Unit* getCurUnit() {
        return _curUnit;
    }
    void keyPressEvent(QKeyEvent *event) {
        qDebug() << event->key();
        int curkey = event->key();
        if ((curkey >= Qt::Key_1) && (curkey <= Qt::Key_9) && (_curUnit != nullptr)) {
            int dir = curkey - 0x30;

            qDebug() << "dir = " << dir;
            qDebug() << "_map = " << _map << " _curUnit = " << _curUnit;
            Game::Direction d = (Game::Direction)dir;
            QString n = _map->className();
            bool b = _map->f();
            if (_map->tryMove(_curUnit, d)) {
                qDebug() << "call repaint";
                _drawer->repaint();
            }
        }

    }
    virtual ~UserActionHyperVisor() {}
};



#endif // USERACTIONHYPERVISOR_H
