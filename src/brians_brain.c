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



const CellularAutomatonTypeParameters briansBrainTypeParameters = {
    .stateCount = BRIANS_BRAIN_STATE_COUNT,
    .initStateColours = briansBraindefaultInitColours,
    .initStateFunc = briansBrainInitStateFunc,
    .updateStateFunc = briansBrainUpdateState,
    .updatePixelDataFunc = briansBrainUpdatePixelData
};



void briansBrainInitStateFunc( CellularAutomaton * ptr )
{
    srand( ptr->seed );

    for ( uint32_t i = 0; i < ptr->count; i++ )
    {
        CellState s = rand( ) % BRIANS_BRAIN_STATE_COUNT;
        ptr->newStates[ i ] = s;
        ptr->oldStates[ i ] = s;
    }
}



// void briansBrainUpdateState( CellularAutomaton * ptr )
// {
//     for ( int row = 0; row < ptr->rows; row++ )
//     {
//         int cellRowIdx = row * ptr->cols;
//
//         for ( int col = 0; col < ptr->cols; col++ )
//         {
//             int cellIdx = cellRowIdx + col;
//
//             switch ( ptr->oldStates[ cellIdx ] )
//             {
//                 case BriansBrainStateDead:
//                 {
//                     int liveNeighbourCount = 0;
//
//                     for ( int rowOff = -1; rowOff <= 1; rowOff++ )
//                     {
//                         int nRow = ( row + rowOff + ptr->rows ) % ptr->rows;
//
//                         for ( int colOff = -1; colOff <= 1; colOff++ )
//                         {
//                             if ( rowOff == 0 && colOff == 0 )
//                             {
//                                 continue;
//                             }
//
//                             int nCol = ( col + colOff + ptr->cols ) % ptr->cols;
//
//                             int nIdx = nRow * ptr->cols + nCol;
//
//                             if ( ptr->oldStates[ nIdx ] == BriansBrainStateLive )
//                             {
//                                 liveNeighbourCount++;
//                             }
//                         }
//                     }
//
//                     ptr->newStates[ cellIdx ] = ( liveNeighbourCount == 2 )
//                         ? BriansBrainStateLive
//                         : BriansBrainStateDead;
//
//                     break;
//                 }
//                 case BriansBrainStateDying:
//                 {
//                     ptr->newStates[ cellIdx ] = BriansBrainStateDead;
//                     break;
//                 }
//                 case BriansBrainStateLive:
//                 {
//                     ptr->newStates[ cellIdx ] = BriansBrainStateDying;
//                     break;
//                 }
//                 default:
//                 {
//                     ptr->newStates[ cellIdx ] = BriansBrainStateDead;
//                     break;
//                 }
//             }
//         }
//     }
// }
void briansBrainUpdateState( CellularAutomaton * ptr )
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
                case BriansBrainStateLive:
                    newRowPtr[ col ] = BriansBrainStateDying;
                    break;

                case BriansBrainStateDying:
                    newRowPtr[ col ] = BriansBrainStateDead;
                    break;

                case BriansBrainStateDead:
                default:
                {
                    const int colLeft  = ( col == 0 ) ? ( colCount - 1 ) : ( col - 1 );
                    const int colRight = ( col == colCount - 1 ) ? 0 : ( col + 1 );

                    const int liveNeighbourCount =
                        ( rowUpPtr   [ colLeft  ] == BriansBrainStateLive ) +
                        ( rowUpPtr   [ col      ] == BriansBrainStateLive ) +
                        ( rowUpPtr   [ colRight ] == BriansBrainStateLive ) +
                        ( rowPtr     [ colLeft  ] == BriansBrainStateLive ) +
                        ( rowPtr     [ colRight ] == BriansBrainStateLive ) +
                        ( rowDownPtr [ colLeft  ] == BriansBrainStateLive ) +
                        ( rowDownPtr [ col      ] == BriansBrainStateLive ) +
                        ( rowDownPtr [ colRight ] == BriansBrainStateLive );

                    newRowPtr[ col ] = ( liveNeighbourCount == 2 )
                        ? BriansBrainStateLive
                        : BriansBrainStateDead;
                    break;
                }
            }
        }
    }
}



void briansBrainUpdatePixelData( CellularAutomaton * ptr )
{
    for ( uint32_t i = 0; i < ptr->count; i++ )
    {
        ptr->pixelData[ i ] = ptr->stateColours[ ptr->newStates[ i ] ];
    }
}