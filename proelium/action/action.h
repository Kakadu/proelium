#ifndef ACTION_H
#define ACTION_H
#include <QObject>
#include "unit.h"

class FireUnitAction;
class MoveUnitAction;
class NoAction;
class EndWarAction;

class UnitVisitor {
public:
    virtual void visit(FireUnitAction&)=0;
    virtual void visit(MoveUnitAction&)=0;
    virtual void visit(EndWarAction&)=0;
    virtual void visit(NoAction&)=0;
};
class AbstractUnitAction
{
public:
    AbstractUnitAction() {}
    virtual void accept(UnitVisitor&)=0;
};

class MoveUnitAction : public AbstractUnitAction {
    Unit* _u;
    int _x, _y;
public:
    Unit* unit() { return _u; }
    int x() { return _x; }
    int y() { return _y; }
    MoveUnitAction(int x, int y,Unit* u) : _x(x),_y(y),_u(u) {}
    MoveUnitAction(MoveUnitAction& act) {
	_u = act.unit();
	_x = act.x();
	_y = act.y();
    }

    virtual void accept(UnitVisitor &v) {
	v.visit(*this);
    }
};
class FireUnitAction : public AbstractUnitAction {    
public:
    const int attackerID;
    const int victimID;
    QString attackerName;
    QString victimName;
    const int result;
    FireUnitAction(int id, QString aName, int tid, QString vName, bool r) :
	    attackerID(id), victimID(tid),
	    attackerName(aName), victimName(vName), result(r) {}

    virtual void accept(UnitVisitor &v) {
	v.visit(*this);
    }
};

class EndWarAction : public AbstractUnitAction {
public:
    EndWarAction() {}
    virtual void accept(UnitVisitor &v) {
	v.visit(*this);
    }
};

class NoAction : public AbstractUnitAction {
public:
    NoAction() {}
    virtual void accept(UnitVisitor &v) {
	v.visit(*this);
    }
};

#endif // ACTION_H
