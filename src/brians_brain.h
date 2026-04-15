#ifndef BRIANS_BRAIN_H
#define BRIANS_BRAIN_H

#include <stddef.h>
#include <stdint.h>

#include "raylib.h"

#include "cellular_automaton.h"



#define BRIANS_BRAIN_STR "Brians Brain"


typedef enum _BriansBrainState {

    BriansBrainState_dead,
    BriansBrainState_dying,
    BriansBrainState_live,

    BRIANS_BRAIN_STATE_COUNT

} BriansBrainState;



extern const CellAutoTypeStaticMems briansBrainTypeParameters;



void briansBrainInitStateFunc( CellAuto * ptr, const CellAutoTypeParams * __attribute__((unused)) );

void briansBrainUpdateState( CellAuto * ptr );

void briansBrainUpdatePixelData( CellAuto * ptr );



#endif // #ifndef BRIANS_BRAIN_H