#include "brians_brain.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "raylib.h"

#include "cellular_automaton.h"



static const Color briansBraindefaultInitColours [ BRIANS_BRAIN_STATE_COUNT ] = {
    BLANK,
    RED,
    WHITE
};



const CellAutoTypeStaticMems briansBrainTypeParameters = {
    .stateCount = BRIANS_BRAIN_STATE_COUNT,
    .stateColours = briansBraindefaultInitColours,
    .initStateFunc = briansBrainInitStateFunc,
    .updateStateFunc = briansBrainUpdateState,
    .updatePixelDataFunc = briansBrainUpdatePixelData
};



void briansBrainInitStateFunc( CellAuto * ptr, const CellAutoTypeParams * __attribute__((unused)) )
{
    for ( uint32_t i = 0; i < ptr->count; i++ )
    {
        CellState s = rand( ) % BRIANS_BRAIN_STATE_COUNT;
        ptr->newStates[ i ] = s;
        ptr->oldStates[ i ] = s;
    }
}



void briansBrainUpdateState( CellAuto * ptr )
{
    const int rowCount = ptr->rows;
    const int colCount = ptr->cols;

    const CellState * oldStates = ptr->oldStates;
    CellState * newStates = ptr->newStates;

    for ( int row = 0; row < rowCount; row++ )
    {
        const int rowUp   = ( row == 0 ) ? ( rowCount - 1 ) : ( row - 1 );
        const int rowDown = ( row == rowCount - 1 ) ? 0 : ( row + 1 );

        const CellState * rowUpPtr   = oldStates + rowUp * colCount;
        const CellState * rowPtr     = oldStates + row * colCount;
        const CellState * rowDownPtr = oldStates + rowDown * colCount;

        CellState * newRowPtr = newStates + row * colCount;

        for ( int col = 0; col < colCount; col++ )
        {
            switch ( rowPtr[ col ] )
            {
                case BriansBrainState_live:
                    newRowPtr[ col ] = BriansBrainState_dying;
                    break;

                case BriansBrainState_dying:
                    newRowPtr[ col ] = BriansBrainState_dead;
                    break;

                case BriansBrainState_dead:
                default:
                {
                    const int colLeft  = ( col == 0 ) ? ( colCount - 1 ) : ( col - 1 );
                    const int colRight = ( col == colCount - 1 ) ? 0 : ( col + 1 );

                    const int liveNeighbourCount =
                        ( rowUpPtr   [ colLeft  ] == BriansBrainState_live ) +
                        ( rowUpPtr   [ col      ] == BriansBrainState_live ) +
                        ( rowUpPtr   [ colRight ] == BriansBrainState_live ) +
                        ( rowPtr     [ colLeft  ] == BriansBrainState_live ) +
                        ( rowPtr     [ colRight ] == BriansBrainState_live ) +
                        ( rowDownPtr [ colLeft  ] == BriansBrainState_live ) +
                        ( rowDownPtr [ col      ] == BriansBrainState_live ) +
                        ( rowDownPtr [ colRight ] == BriansBrainState_live );

                    newRowPtr[ col ] = ( liveNeighbourCount == 2 )
                        ? BriansBrainState_live
                        : BriansBrainState_dead;
                    break;
                }
            }
        }
    }
}



void briansBrainUpdatePixelData( CellAuto * ptr )
{
    const Color * const stateColours = ptr->staticMems.stateColours;
    const CellState * states = ptr->newStates;

    Color * pixelData = ptr->pixelData;

    for ( uint32_t i = 0; i < ptr->count; i++ )
    {
        *pixelData++ = stateColours[ *states++ ];
    }
}