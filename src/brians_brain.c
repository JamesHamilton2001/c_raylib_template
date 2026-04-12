#include "brians_brain.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>



void briansBrainInit( Grid * gridPtr,
                      int rows,
                      int cols,
                      Color initDeadColour,
                      Color initDyingColour,
                      Color initLiveColour )
{
    gridPtr->rows = rows;
    gridPtr->cols = cols;
    gridPtr->count = rows * cols;

    gridPtr->stateColours[ CellStateDead ] = initDeadColour;
    gridPtr->stateColours[ CellStateDying ] = initDyingColour;
    gridPtr->stateColours[ CellStateLive ] = initLiveColour;

    gridPtr->stateBuffer[ 0 ] = calloc( gridPtr->count, sizeof( CellState ) );
    gridPtr->stateBuffer[ 1 ] = calloc( gridPtr->count, sizeof( CellState ) );

    gridPtr->newStates = gridPtr->stateBuffer[ 0 ];
    gridPtr->oldStates = gridPtr->stateBuffer[ 1 ];

    for ( size_t i = 0; i < gridPtr->count; i++ )
    {
        if ( rand( ) % 3 == 0 )
        {
            gridPtr->newStates[ i ] = CellStateLive;
            gridPtr->oldStates[ i ] = CellStateLive;
        }
    }
}



void briansBrainDenit( Grid * gridPtr )
{
    free( gridPtr->stateBuffer[ 0 ] );
    free( gridPtr->stateBuffer[ 1 ] );
    memset( gridPtr, 0x00, sizeof( *gridPtr ) );
}



void briansBrainUpdate( Grid * gridPtr )
{
    CellState * temp = gridPtr->oldStates;
    gridPtr->oldStates = gridPtr->newStates;
    gridPtr->newStates = temp;

    for ( int row = 0; row < gridPtr->rows; row++ )
    {
        for ( int col = 0; col < gridPtr->cols; col++ )
        {
            size_t cellIdx = row * gridPtr->cols + col;

            switch ( gridPtr->oldStates[ cellIdx ] )
            {
                case CellStateDead:
                {
                    size_t liveNeighbourCount = 0;

                    for ( int r = -1; r <= 1; r++ )
                    {
                        for ( int c = -1; c <= 1; c++ )
                        {
                            if ( r == 0 && c == 0 )
                            {
                                continue;
                            }

                            int neighbourRow = ( row + r + gridPtr->rows ) % gridPtr->rows;
                            int neighbourCol = ( col + c + gridPtr->cols ) % gridPtr->cols;
                            size_t neighbourIdx = neighbourRow * gridPtr->cols + neighbourCol;

                            if ( gridPtr->oldStates[ neighbourIdx ] == CellStateLive )
                            {
                                liveNeighbourCount++;
                            }
                        }
                    }
                    gridPtr->newStates[ cellIdx ] = ( liveNeighbourCount == 2 )
                                                  ? CellStateLive
                                                  : CellStateDead;
                    break;
                }
                case CellStateDying:
                {
                    gridPtr->newStates[ cellIdx ] = CellStateDead;
                    break;
                }
                case CellStateLive:
                {
                    gridPtr->newStates[ cellIdx ] = CellStateDying;
                    break;
                }
                default:
                {
                    gridPtr->newStates[ cellIdx ] = CellStateDead;
                    break;
                }
            }
        }
    }
}