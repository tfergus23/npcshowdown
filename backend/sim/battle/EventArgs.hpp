#pragma once

class Pokemon;
class MoveUse;

class EventArgs{
public:
    Pokemon* const eventSubject; // Subject of event
    MoveUse* const moveUse; //MoveUse if applicable
    EventArgs(Pokemon* eventSubject, MoveUse* moveUse) : eventSubject{eventSubject}, moveUse{moveUse}{}
};