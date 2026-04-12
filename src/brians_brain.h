#ifndef BRIANS_BRAIN_H
#define BRIANS_BRAIN_H

#include <stddef.h>
#include <stdint.h>

#include "raylib.h"

#include "cellular_automaton.h"



#define BRIANS_BRAIN_STR "Brians Brain"


typedef enum _BriansBrainState {

    BriansBrainStateDead,
    BriansBrainStateDying,
    BriansBrainStateLive,

    BRIANS_BRAIN_STATE_COUNT

} BriansBrainState;



extern const CellularAutomatonTypeParameters briansBrainTypeParameters;



void briansBrainInitStateFunc( CellularAutomaton * ptr );

void briansBrainUpdateState( CellularAutomaton * ptr );

void briansBrainUpdatePixelData( CellularAutomaton * ptr );




#endif // #ifndef BRIANS_BRAIN_H