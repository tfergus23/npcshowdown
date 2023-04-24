#pragma once

/*
    virtual void beforeMove(MoveUse* moveUse,Pokemon* subject) const {}
    virtual int modifySubjectStat(Stat stat,int statVal,Pokemon* subject) const {return statVal;}
    virtual void endOfTurn(Pokemon* subject) const {}
    virtual void priorityEndOfTurn(Pokemon* subject) const {}
    virtual void afterMove(MoveUse* moveUse, Pokemon* subject) const {}
    virtual void onSubjectSwitch(Pokemon* subject) const {}
    virtual void onSubjectEnter(Pokemon* subject) const {}
    virtual void onSubjectDeath(Pokemon* subject) const {}
    virtual void onSubjectAttack(MoveUse* moveUse) const {}
    virtual void onSubjectAttacked(MoveUse* moveUse) const {}
    virtual void onOpponentSwitch(Pokemon* subject,Pokemon* opponent) const {}
    virtual void onOpponentEnter(Pokemon* subject, Pokemon* opponent) const {}
*/
enum Event {BEFORE_MOVE, END_OF_TURN, PRIORITY_END_OF_TURN, AFTER_MOVE, POKEMON_SWITCH, POKEMON_ENTER, POKEMON_DEATH, POKEMON_ATTACKED};