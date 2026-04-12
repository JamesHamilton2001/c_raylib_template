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

extern const Color briansBraindefaultInitColours [ BRIANS_BRAIN_STATE_COUNT ];



void briansBrainInit(
    CellularAutomaton * ptr,
    const Color * initialStateColours,
    int32_t rows,
    int32_t cols,
    uint32_t id,
    uint32_t seed,
    CellState * initialStates
);

void briansBrainDenit( CellularAutomaton * ptr );

void briansBrainUpdateState( CellularAutomaton * ptr );

void briansBrainUpdatePixelData( CellularAutomaton * ptr );




#endif // #ifndef BRIANS_BRAIN_H