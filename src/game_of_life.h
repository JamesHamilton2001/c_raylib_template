#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <stddef.h>
#include <stdint.h>

#include "raylib.h"

#include "cellular_automaton.h"



#define GAME_OF_LIFE_STR "Brians Brain"


typedef enum _GameOfLifeState {

    GameOfLifeStateDead,
    GameOfLifeStateLive,

    GAME_OF_LIFE_STATE_COUNT

} GameOfLifeState;



extern const CellularAutomatonTypeParameters gameOfLifeTypeParameters;



void gameOfLifeInitStateFunc( CellularAutomaton * ptr );

void gameOfLifeUpdateState( CellularAutomaton * ptr );

void gameOfLifeUpdatePixelData( CellularAutomaton * ptr );




#endif // #ifndef GAME_OF_LIFE_H