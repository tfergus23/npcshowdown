#pragma once

class Pokemon;
class MoveUse;

class EventArgs{
public:
    Pokemon* const eventSubject;
    MoveUse* const moveUse;
    EventArgs(Pokemon* eventSubject, MoveUse* moveUse) : eventSubject{eventSubject}, moveUse{moveUse}{}
};