#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <stddef.h>
#include <stdint.h>

#include "raylib.h"

#include "cellular_automaton.h"



#define GAME_OF_LIFE_STR "Brians Brain"


typedef enum _GameOfLifeState {

    GameOfLifeState_dead,
    GameOfLifeState_live,

    GAME_OF_LIFE_STATE_COUNT

} GameOfLifeState;



extern const CellAutomTypeStaticMems gameOfLifeStaticMems;



void gameOfLifeInitStateFunc( CellAutom * ptr, const CellAutomTypeDynamicMems * __attribute__((unused)) );

void gameOfLifeUpdateState( CellAutom * ptr );

void gameOfLifeUpdatePixelData( CellAutom * ptr );



#endif // #ifndef GAME_OF_LIFE_H