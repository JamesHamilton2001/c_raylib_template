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
    for ( int row = 0; row < ptr->rows; row++ )
    {
        int cellRowIdx = row * ptr->cols;

        for ( int col = 0; col < ptr->cols; col++ )
        {
            int cellIdx = cellRowIdx + col;

            int liveNeighbourCount = 0;

            for ( int rowOff = -1; rowOff <= 1; rowOff++ )
            {
                int nRow = ( row + rowOff + ptr->rows ) % ptr->rows;

                for ( int colOff = -1; colOff <= 1; colOff++ )
                {
                    if ( rowOff == 0 && colOff == 0 )
                    {
                        continue;
                    }

                    int nCol = ( col + colOff + ptr->cols ) % ptr->cols;

                    int nIdx = nRow * ptr->cols + nCol;

                    if ( ptr->oldStates[ nIdx ] == GameOfLifeStateLive )
                    {
                        liveNeighbourCount++;
                    }
                }
            }

            if ( ptr->oldStates[ cellIdx ] == GameOfLifeStateLive )
            {
                ptr->newStates[ cellIdx ] = ( liveNeighbourCount == 3 || liveNeighbourCount == 2 )
                    ? GameOfLifeStateLive
                    : GameOfLifeStateDead;
            }
            else
            {
                ptr->newStates[ cellIdx ] = ( liveNeighbourCount == 3 )
                    ? GameOfLifeStateLive
                    : GameOfLifeStateDead;
            }
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