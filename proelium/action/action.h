#ifndef ACTION_H
#define ACTION_H

class FireUnitAction;
class MoveUnitAction;

class UnitVisitor {
public:
    virtual void visit(FireUnitAction&)=0;
    virtual void visit(MoveUnitAction&)=0;
};
class AbstractUnitAction
{
public:
    AbstractUnitAction() {}
    virtual void accept(UnitVisitor&)=0;
};

class MoveUnitAction : public AbstractUnitAction {
public:
    const int &unitID;
    const int &depth;
    MoveUnitAction(int id, int d=0) : unitID(id), depth(d) {}
    virtual void accept(UnitVisitor &v) {
	v.visit(*this);
    }
};
class FireUnitAction : public AbstractUnitAction {    
public:
    const int &unitID;
    const int &targetID;
    const int &result;
    FireUnitAction(int id, int tid, bool r) : unitID(id), targetID(tid), result(r) {}
    virtual void accept(UnitVisitor &v) {
	v.visit(*this);
    }
};
#endif // ACTION_H
