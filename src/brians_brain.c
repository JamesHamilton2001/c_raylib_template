#include "brians_brain.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "raylib.h"

#include "cellular_automaton.h"



const Color briansBraindefaultInitColours [ BRIANS_BRAIN_STATE_COUNT ] = {
    BLACK,
    RED,
    WHITE
};



void briansBrainInit(
    CellularAutomaton * ptr,
    const Color * initialStateColours,
    int32_t rows,
    int32_t cols,
    uint32_t id,
    uint32_t seed,
    CellState * initialStates
) {
    const char * name = BRIANS_BRAIN_STR;

    uint32_t stateCount = BRIANS_BRAIN_STATE_COUNT;

    if ( initialStateColours == NULL )
    {
        initialStateColours = briansBraindefaultInitColours;
    }

    CellularAutomatonInit(
        ptr,
        name,
        stateCount,
        initialStateColours,
        briansBrainUpdate,
        rows,
        cols,
        id,
        seed,
        initialStates
    );

    srand( seed );

    for ( int32_t i = 0; i < rows * cols; i++ )
    {
        ptr->newStates[ i ] = rand( ) & ( BriansBrainStateDying | BriansBrainStateLive );
    }
}



void briansBrainDenit( CellularAutomaton * ptr )
{
    CellularAutomatonDenit( ptr );
}



void briansBrainUpdate( CellularAutomaton * ptr )
{
    for ( int row = 0; row < ptr->rows; row++ )
    {
        const int cellRowIdx = row * ptr->cols;

        for ( int col = 0; col < ptr->cols; col++ )
        {
            const int cellIdx = cellRowIdx + col;

            switch ( ptr->oldStates[ cellIdx ] )
            {
                case BriansBrainStateDead:
                {
                    int liveNeighbourCount = 0;

                    for ( int rowOff = -1; rowOff <= 1; rowOff++ )
                    {
                        const int nRow = ( row + rowOff + ptr->rows ) % ptr->rows;

                        for ( int colOff = -1; colOff <= 1; colOff++ )
                        {
                            if ( rowOff == 0 && colOff == 0 )
                            {
                                continue;
                            }

                            const int nCol = ( col + colOff + ptr->cols ) % ptr->cols;

                            const int nIdx = nRow * ptr->cols + nCol;

                            if ( ptr->oldStates[ nIdx ] == BriansBrainStateLive )
                            {
                                liveNeighbourCount++;
                            }
                        }
                    }
                    ptr->newStates[ cellIdx ] = ( liveNeighbourCount == 2 )
                        ? BriansBrainStateLive
                        : BriansBrainStateDead;
                    break;
                }
                case BriansBrainStateDying:
                {
                    ptr->newStates[ cellIdx ] = BriansBrainStateDead;
                    break;
                }
                case BriansBrainStateLive:
                {
                    ptr->newStates[ cellIdx ] = BriansBrainStateDying;
                    break;
                }
                default:
                {
                    ptr->newStates[ cellIdx ] = BriansBrainStateDead;
                    break;
                }
            }
        }
    }
}