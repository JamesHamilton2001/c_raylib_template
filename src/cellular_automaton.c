#include "cellular_automaton.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"



void CellularAutomatonInit(

    CellularAutomaton * ptr,
    const char * name,
    uint32_t stateCount,
    const Color * stateInitColours,
    void ( *updateFunc )( CellularAutomaton * ),

    int32_t rows,
    int32_t cols,
    uint32_t id,
    uint32_t seed,
    CellState * initialStates

) {
    strncpy( ptr->name, name, sizeof( ptr->name - 1 ) );

    ptr->stateCount = stateCount;
    ptr->initStateColours = malloc( stateCount * sizeof( Color ) );
    ptr->stateColours = malloc( stateCount * sizeof( Color ) );
    memcpy( ptr->initStateColours, stateInitColours, stateCount * sizeof( Color ) );
    memcpy( ptr->stateColours, stateInitColours, stateCount * sizeof( Color ) );

    ptr->rows = rows;
    ptr->cols = cols;

    ptr->stateBuffer[ 0 ] = calloc( rows * cols, sizeof( CellState ) );
    ptr->stateBuffer[ 1 ] = calloc( rows * cols, sizeof( CellState ) );

    ptr->newStates = ptr->stateBuffer[ 0 ];
    ptr->oldStates = ptr->stateBuffer[ 1 ];

    ptr->updateFunc = updateFunc;

    ptr->iterationCount = 0;

    ptr->id = id;

    ptr->seed = seed;

    if ( initialStates == NULL )
    {
        ptr->initialStates = NULL;
    }
    else
    {
        ptr->initialStates = malloc( rows * cols * sizeof( CellState ) );
        memcpy( ptr->initialStates, initialStates, rows * cols * sizeof( CellState ) );
    }
}



void CellularAutomatonDenit( CellularAutomaton * ptr )
{
    if ( ptr->initStateColours != NULL ) free( ptr->initStateColours );
    if ( ptr->stateColours != NULL ) free( ptr->stateColours );
    if ( ptr->stateBuffer[ 0 ] != NULL ) free( ptr->stateBuffer[ 0 ] );
    if ( ptr->stateBuffer[ 1 ] != NULL ) free( ptr->stateBuffer[ 1 ] );
    if ( ptr->initialStates != NULL ) free( ptr->initialStates );

    memset( ptr, 0x00, sizeof( *ptr ) );
}



void CellularAutomatonUpdate( CellularAutomaton * ptr )
{
    CellState * temp = ptr->oldStates;
    ptr->oldStates   = ptr->newStates;
    ptr->newStates   = temp;

    ptr->updateFunc( ptr );

    ptr->iterationCount++;
}



extern int screenWidth;
extern int screenHeight;

void CellularAutomatonDraw( const CellularAutomaton * ptr )
{
    int width = screenWidth / ptr->cols;
    int height = screenHeight / ptr->rows;

    for ( int row = 0; row < ptr->rows; row++ )
    {
        int y = row * height;

        for ( int col = 0; col < ptr->cols; col++ )
        {
            int x = col * width;

            CellState state = ptr->newStates[ row * ptr->cols + col ];

            DrawRectangle( x, y, width, height, ptr->stateColours[ state ] );
        }
    }
}
