#ifndef UNIT_H
#define UNIT_H
#include "framecollection.h"
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QtCore>
#include <QTimer>
#include <iostream>
#include "stuff/consts.h"

typedef int IDT;

class Unit : public QObject  {
    Q_OBJECT
    //Q_PROPERTY(int attackFrame WRITE setAttackFrame READ attackFrame);
private:
    Game::Direction _lastDir;
    FrameCollection* _attackFrames;
    FrameCollection* _deathFrames;
    FrameCollection* _moveFrames[8];
    QGraphicsPixmapItem* _view;
    QTimer* _timer;
    int _curframe,;
    int _allPath;
    int _nonEvaledPath;
    bool is_alive;
    int _maxMovePoints;
    int _movePoints;
public:
    int id;
    QString name;
    Game::Nations ownerId;
    int maxMovePoint() { return _maxMovePoints; }
    Game::Direction lastDir() { return _lastDir; }
    void setLastDir(Game::Direction d) { _lastDir = d; }

    /*inline int allPath() { return _allPath; }
    inline int nonEvaledPath() { return _nonEvaledPath; }
    inline void incrPath(int x) {
        _allPath+=x; _nonEvaledPath+=x;
    }
    inline void afterEvalPath(int x) {
        if (x>_nonEvaledPath)
            throw std::exception();
        _nonEvaledPath -=x;
    }*/

    Unit& operator=(const Unit& u) {
        Unit* a = new Unit(u.name, u.id, u.ownerId);
        return *a;
    }
    bool alive (void) { return is_alive; }
    void setAlive(bool b) { is_alive = b; }

	explicit Unit(QString _name, int _id, Game::Nations _ownerId, int maxMP=1,
				  QObject* parent = NULL) : QObject(parent) {
		name = _name;
		id = _id;
		_movePoints = _maxMovePoints = maxMP;
		ownerId = _ownerId;
		is_alive = true;
		_lastDir = Game::Dir_S;
	}
	inline bool canMove() { return _movePoints>0; }
	inline bool decMove() {
		if (_movePoints>0)
			_movePoints--;
		return canMove();
	}
	inline void setMaxMovePoints() { _movePoints = _maxMovePoints; }
};

class UnitVisitor {
public:
	virtual void visit(Unit*)=0;
};

#endif // UNIT_H
