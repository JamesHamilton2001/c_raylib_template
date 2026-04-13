#include "game_of_life.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "raylib.h"

#include "cellular_automaton.h"



static const Color gameOfLifeDefaultInitColours [ GAME_OF_LIFE_STATE_COUNT ] = {
    BLANK,
    WHITE
};



const CellularAutomatonTypeParameters gameOfLifeTypeParameters = {
    .stateCount = GAME_OF_LIFE_STATE_COUNT,
    .initStateColours = gameOfLifeDefaultInitColours,
    .initStateFunc = gameOfLifeInitStateFunc,
    .updateStateFunc = gameOfLifeUpdateState,
    .updatePixelDataFunc = gameOfLifeUpdatePixelData
};



void gameOfLifeInitStateFunc( CellularAutomaton * ptr )
{
    srand( ptr->seed );

    for ( uint32_t i = 0; i < ptr->count; i++ )
    {
        CellState s = rand( ) % GAME_OF_LIFE_STATE_COUNT;
        ptr->newStates[ i ] = s;
        ptr->oldStates[ i ] = s;
    }
}



void gameOfLifeUpdateState( CellularAutomaton * ptr )
{
    const int rowCount = ptr->rows;
    const int colCount = ptr->cols;

    const CellState * oldStates = ptr->oldStates;

    CellState * newStates = ptr->newStates;

    for ( int row = 0; row < rowCount; row++ )
    {
        const int rowUp    = ( row == 0 ) ? ( rowCount - 1 ) : ( row - 1 );
        const int rowDown  = ( row == rowCount - 1 ) ? ( 0 ) : ( row + 1 );

        const CellState * rowUpPtr   = oldStates + rowUp * colCount;
        const CellState * rowPtr     = oldStates + row * colCount;
        const CellState * rowDownPtr = oldStates + rowDown * colCount;

        CellState *newRowPtr = newStates + row * colCount;

        for ( int col = 0; col < colCount; col++ )
        {
            const int colLeft  = ( col == 0 ) ? ( colCount - 1 ) : ( col - 1 );
            const int colRight = ( col == colCount - 1 ) ? ( 0 ) : ( col + 1 );

            int liveNeighbourCount = 0;

            liveNeighbourCount += ( rowUpPtr   [ colLeft  ] );
            liveNeighbourCount += ( rowUpPtr   [ col      ] );
            liveNeighbourCount += ( rowUpPtr   [ colRight ] );
            liveNeighbourCount += ( rowPtr     [ colLeft  ] );
            liveNeighbourCount += ( rowPtr     [ colRight ] );
            liveNeighbourCount += ( rowDownPtr [ colLeft  ] );
            liveNeighbourCount += ( rowDownPtr [ col      ] );
            liveNeighbourCount += ( rowDownPtr [ colRight ] );

            const int selfIsLive = ( rowPtr[ col ] == GameOfLifeStateLive );

            newRowPtr[col] = ( liveNeighbourCount == 3 || ( selfIsLive && liveNeighbourCount == 2 ) )
                ? GameOfLifeStateLive
                : GameOfLifeStateDead;
        }
    }
}



void gameOfLifeUpdatePixelData( CellularAutomaton * ptr )
{
    for ( uint32_t i = 0; i < ptr->count; i++ )
    {
        ptr->pixelData[ i ] = ptr->stateColours[ ptr->newStates[ i ] ];
    }
}