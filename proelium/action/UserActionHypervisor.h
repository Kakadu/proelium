#ifndef USERACTIONHYPERVISOR_H
#define USERACTIONHYPERVISOR_H
#include <QKeyEvent>
#include "GameMap.h"
#include "unit.h"

/**
 * This class will dispatch user keyboard and mouse actions to game model
 */
class UserActionHyperVisor : public QObject
{
    Q_OBJECT
private:
    GameMap* _map;
    Unit* _curUnit;
public:
    UserActionHyperVisor(GameMap* map) : _map(map) {

    }
    void setCurUnit(Unit* u) {
        _curUnit = u;
    }
    Unit* getCurUnit() {
        return _curUnit;
    }
    void keyPressEvent(QKeyEvent *event) {
        qDebug() << event->key();
        int curkey = event->key();
        if ((curkey > 0x30) && (curkey < 0x40)) {
            int dir = curkey - 0x30;
            qDebug() << "dir = " << dir;
        }

    }
    virtual ~UserActionHyperVisor() {}
};



#endif // USERACTIONHYPERVISOR_H
