#include "cellular_automaton.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

#include "game_of_life.h"
#include "brians_brain.h"
#include "langtons_ant.h"



static CellAutomTypeStaticMems getTypeParameters( CellAutomType type );

static void ( *getTypeDynamicMemDestroyer( CellAutomType type ) )( CellAutomTypeDynamicMems * );



void CellAutomInit(
    CellAutom * ptr,
    CellAutomType type,
    CellAutomTypeDynamicMems * dynamicArgsPtr,
    int32_t rows,
    int32_t cols,
    const char * name,
    uint32_t id,
    uint32_t seed,
    CellState * initialStates
) {
    memset( ptr, 0x00, sizeof( *ptr ) );

    ptr->staticMems = getTypeParameters( type );

    ptr->type = type;

    ptr->rows = rows;
    ptr->cols = cols;
    ptr->count = rows * cols;

    ptr->stateBuffer[ 0 ] = calloc( ptr->count, sizeof( CellState ) );
    ptr->stateBuffer[ 1 ] = calloc( ptr->count, sizeof( CellState ) );

    ptr->newStates = ptr->stateBuffer[ 0 ];
    ptr->oldStates = ptr->stateBuffer[ 1 ];

    ptr->pixelData = calloc( ptr->count, sizeof( Color ) );
    ptr->image.data = ptr->pixelData;
    ptr->image.width = cols;
    ptr->image.height = rows;
    ptr->image.mipmaps = 1;
    ptr->image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

    ptr->texture = LoadTextureFromImage( ptr->image );
    SetTextureFilter( ptr->texture, TEXTURE_FILTER_POINT );

    ptr->iterationCount = 0;

    strncpy( ptr->name, name, sizeof( ptr->name ) - 1 );
    ptr->name[ sizeof( ptr->name ) - 1 ] = '\0';

    ptr->id = id;

    ptr->seed = seed;

    if ( initialStates == NULL )
    {
        ptr->initialStates = NULL;
    }
    else
    {
        ptr->initialStates = malloc( ptr->count * sizeof( CellState ) );
        memcpy( ptr->initialStates, initialStates, ptr->count * sizeof( CellState ) );
    }

    srand( seed );

    ptr->staticMems.initStateFunc( ptr, dynamicArgsPtr );
}



void CellAutomDeinit( CellAutom * ptr )
{
    getTypeDynamicMemDestroyer( ptr->type )( ptr->dynamicMemsPtr );
    free( ptr->dynamicMemsPtr );

    UnloadTexture( ptr->texture );

    free( ptr->stateBuffer[ 0 ] );
    free( ptr->stateBuffer[ 1 ] );
    free( ptr->pixelData );
    free( ptr->initialStates );

    memset( ptr, 0x00, sizeof( *ptr ) );
}



void CellAutomUpdate( CellAutom * ptr )
{
    CellState * temp = ptr->oldStates;
    ptr->oldStates = ptr->newStates;
    ptr->newStates = temp;

    ptr->staticMems.updateStateFunc( ptr );
    ptr->staticMems.updatePixelDataFunc( ptr );

    ptr->iterationCount++;
}



void CellUpdateTexture( const CellAutom * ptr )
{
    UpdateTexture( ptr->texture, ptr->pixelData );
}













static const uint32_t errorTypeStateCount = 2;

static const Color errorTypeStateColours [ 2 ] = { BLACK, MAGENTA };

static void errorTypeInitStateFunc( CellAutom * ptr, const CellAutomTypeDynamicMems * __attribute__((unused)) )
{
    CellState * newStates = ptr->newStates;
    CellState * oldStates = ptr->oldStates;
    Color     * pixelData = ptr->pixelData;

    const int32_t cols = ptr->cols;

    for ( uint32_t i = 0; i < ptr->count; i++ )
    {
        const CellState s = ( ( i % cols ) + ( i / cols ) ) % 2;
        *newStates++ = s;
        *oldStates++ = s;
        *pixelData++ = *(errorTypeStateColours + s);
    }
}

static void errorTypeStateAndPixelUpdateFunc( CellAutom * ptr __attribute__((unused)) ) { }

static const CellAutomTypeStaticMems errorStateStaticMems = {
    .stateCount = errorTypeStateCount,
    .stateColours = errorTypeStateColours,
    .initStateFunc = errorTypeInitStateFunc,
    .updateStateFunc = errorTypeStateAndPixelUpdateFunc,
    .updatePixelDataFunc = errorTypeStateAndPixelUpdateFunc
};



static CellAutomTypeStaticMems getTypeParameters( CellAutomType type )
{
    switch ( type )
    {
        case CellularAutomatonType_gameOfLife:  return gameOfLifeStaticMems;
        case CellularAutomatonType_briansBrain: return briansBrainTypeParameters;
        case CellularAutomatonType_langtonsAnt: return langtonsAntTypeParameters;

        default: return errorStateStaticMems;
    }
}



static void noDynamicMemsToFree( CellAutomTypeDynamicMems * ptr __attribute__((unused)) ) { };

static void ( *getTypeDynamicMemDestroyer( CellAutomType type ) )( CellAutomTypeDynamicMems * )
{
    switch ( type )
    {
        case CellularAutomatonType_gameOfLife:  return noDynamicMemsToFree;
        case CellularAutomatonType_briansBrain: return noDynamicMemsToFree;
        case CellularAutomatonType_langtonsAnt: return LangtonsAntParamsDestroy;

        default: return noDynamicMemsToFree;
    }
}
