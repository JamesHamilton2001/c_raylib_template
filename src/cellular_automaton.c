#include "cellular_automaton.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

#include "brians_brain.h"



static const CellularAutomatonTypeParameters * getTypeParametersPtr( CellularAutomatonType type )
{
    switch ( type )
    {
        case CellularAutomatonTypeBriansBrain:
            return &briansBrainTypeParameters;

        default:
            return NULL;
    }
}



void CellularAutomatonInit(
    CellularAutomaton * ptr,
    CellularAutomatonType type,
    int32_t rows,
    int32_t cols,
    const char * name,
    uint32_t id,
    uint32_t seed,
    CellState * initialStates
) {
    const CellularAutomatonTypeParameters * typeParameters = getTypeParametersPtr( type );

    ptr->type = type;

    strncpy( ptr->name, name, sizeof( ptr->name ) - 1 );
    ptr->name[ sizeof( ptr->name ) - 1 ] = '\0';

    ptr->stateCount = typeParameters->stateCount;
    ptr->initStateColours = malloc( ptr->stateCount * sizeof( Color ) );
    ptr->stateColours = malloc( ptr->stateCount * sizeof( Color ) );
    memcpy( ptr->initStateColours, typeParameters->initStateColours, ptr->stateCount * sizeof( Color ) );
    memcpy( ptr->stateColours, typeParameters->initStateColours, ptr->stateCount * sizeof( Color ) );

    ptr->rows = rows;
    ptr->cols = cols;
    ptr->count = rows * cols;

    ptr->stateBuffer[ 0 ] = calloc( ptr->count, sizeof( CellState ) );
    ptr->stateBuffer[ 1 ] = calloc( ptr->count, sizeof( CellState ) );

    ptr->newStates = ptr->stateBuffer[ 0 ];
    ptr->oldStates = ptr->stateBuffer[ 1 ];

    ptr->initStateFunc = typeParameters->initStateFunc;
    ptr->updateStateFunc = typeParameters->updateStateFunc;
    ptr->updatePixelDataFunc = typeParameters->updatePixelDataFunc;

    ptr->pixelData = calloc( ptr->count, sizeof( Color ) );
    ptr->image.data = ptr->pixelData;
    ptr->image.width = cols;
    ptr->image.height = rows;
    ptr->image.mipmaps = 1;
    ptr->image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

    ptr->texture = LoadTextureFromImage( ptr->image );

    ptr->iterationCount = 0;

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

    ptr->initStateFunc( ptr );
}



void CellularAutomatonDenit( CellularAutomaton * ptr )
{
    UnloadTexture( ptr->texture );

    free( ptr->initStateColours );
    free( ptr->stateColours );
    free( ptr->stateBuffer[ 0 ] );
    free( ptr->stateBuffer[ 1 ] );
    free( ptr->initialStates );
    free( ptr->pixelData );

    memset( ptr, 0x00, sizeof( *ptr ) );
}



void CellularAutomatonUpdate( CellularAutomaton * ptr )
{
    CellState * temp = ptr->oldStates;
    ptr->oldStates = ptr->newStates;
    ptr->newStates = temp;

    ptr->updateStateFunc( ptr );
    ptr->updatePixelDataFunc( ptr );

    ptr->iterationCount++;
}



void CellularAutomatonDraw( const CellularAutomaton * ptr )
{
    float screenWidth = (float)GetScreenWidth( );
    float screenHeight = (float)GetScreenHeight( );

    float texWidth = (float)ptr->cols;
    float texHeight = (float)ptr->rows;

    float xScale = screenWidth / texWidth;
    float yScale = screenHeight / texHeight;
    float scale = ( xScale < yScale ) ? xScale : yScale;

    float drawWidth = texWidth * scale;
    float drawHeight = texHeight * scale;


    Rectangle src = { 0.0f, 0.0f, texWidth, texHeight };

    Rectangle dst = {
        ( screenWidth - drawWidth ) * 0.5f,
        ( screenHeight - drawHeight ) * 0.5f,
        drawWidth,
        drawHeight
    };

    Vector2 origin = { 0.0f, 0.0f };

    float rotation = 0.0f;

    UpdateTexture( ptr->texture, ptr->pixelData );

    DrawTexturePro( ptr->texture, src, dst, origin, rotation, WHITE );
}
