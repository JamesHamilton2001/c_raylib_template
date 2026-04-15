#include "langtons_ant.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

#include "cellular_automaton.h"



static const Color langtonsAntDefaultInitColours [ LANGTONS_ANT_STATE_COUNT ] = {
    { 255, 0, 0, 15 },
    { 255, 0, 0, 23 }
};



const CellAutomTypeStaticMems langtonsAntTypeParameters = {
    .stateCount = LANGTONS_ANT_STATE_COUNT,
    .stateColours = langtonsAntDefaultInitColours,
    .initStateFunc = langtonsAntInitStateFunc,
    .updateStateFunc = langtonsAntUpdateState,
    .updatePixelDataFunc = langtonsAntUpdatePixelData
};



void langtonsAntInitStateFunc( CellAutom * ptr, const CellAutomTypeDynamicMems * argsPtr )
{
    const LangtonsAntParams * src = argsPtr;

    ptr->dynamicMemsPtr = malloc( sizeof( *src ) );

    LangtonsAntParams * dst = ptr->dynamicMemsPtr;

    memset( dst, 0x00, sizeof( *dst ) );

    dst->antCount = src->antCount;

    dst->antRows = malloc( src->antCount * sizeof( *src->antRows ) );
    dst->antCols = malloc( src->antCount * sizeof( *src->antCols ) );
    dst->antDirections     = malloc( src->antCount * 2 * sizeof( *src->antDirections ) );
    dst->antInitDirections = malloc( src->antCount * 2 * sizeof( *src->antDirections ) );

    if ( src->antInitDirections == NULL )
    {
        for ( uint32_t i = 0; i < src->antCount; i++ )
        {
            const uint32_t roc = rand( ) % 2;
            dst->antInitDirections[ 2 * i +  roc ] = 1 - 2 * ( rand() % 2 );
            dst->antInitDirections[ 2 * i + !roc ] = 0;
        }
        memcpy( dst->antDirections, dst->antInitDirections, src->antCount * 2 * sizeof( *src->antDirections ) );
    }
    else
    {
        memcpy( dst->antInitDirections, src->antInitDirections, src->antCount * 2 * sizeof( *src->antDirections ) );
        memcpy( dst->antDirections,     src->antInitDirections, src->antCount * 2 * sizeof( *src->antDirections ) );
    }

    if ( src->antInitPositions == NULL )
    {
        dst->antInitPositions = NULL;

        for ( uint32_t i = 0; i < src->antCount; i++ )
        {
            dst->antRows[ i ] = rand( ) % ptr->rows;
            dst->antCols[ i ] = rand( ) % ptr->cols;
        }
    }
    else
    {
        dst->antInitPositions = malloc( src->antCount * sizeof( *src->antInitPositions ) );

        memcpy( dst->antInitPositions, src->antInitPositions, src->antCount * 2 * sizeof( *src->antInitPositions ) );

        for ( uint32_t i = 0; i < src->antCount; i++ )
        {
            dst->antRows[ i ] = src->antInitPositions[ i ] / ptr->cols;
            dst->antCols[ i ] = src->antInitPositions[ i ] % ptr->cols;
        }
    }
}



void langtonsAntUpdateState( CellAutom * ptr )
{
    const uint32_t count = ptr->count;
    const int32_t rowCount = ptr->rows;
    const int32_t colCount = ptr->cols;

    LangtonsAntParams *p = (LangtonsAntParams *)ptr->dynamicMemsPtr;

    const uint32_t antCount = p->antCount;
    int32_t *antRows = p->antRows;
    int32_t *antCols = p->antCols;
    int8_t *antDirections = p->antDirections;

    const CellState *oldStates = ptr->oldStates;
    CellState *newStates = ptr->newStates;

    memcpy(newStates, oldStates, count * sizeof(*newStates));

    for ( uint32_t i = 0; i < antCount; i++ )
    {
        int32_t row = antRows[i];
        int32_t col = antCols[i];

        const uint32_t pos = row * colCount + col;

        CellState s = oldStates[ pos ];

        int8_t dr = antDirections[ 2 * i ];
        int8_t dc = antDirections[ 2 * i + 1 ];

        newStates[ pos ] = !s;

        if ( s == 0 )
        {
            const int8_t newDr = dc;
            const int8_t newDc = -dr;
            dr = newDr;
            dc = newDc;
        }
        else
        {
            const int8_t newDr = -dc;
            const int8_t newDc = dr;
            dr = newDr;
            dc = newDc;
        }

        antDirections[ 2 * i + 0 ] = dr;
        antDirections[ 2 * i + 1 ] = dc;

        row += dr;
        col += dc;

        if ( row < 0 )
        {
            row = rowCount - 1;
        }
        else if ( row >= rowCount )
        {
            row = 0;
        }

        if ( col < 0 )
        {
            col = colCount - 1;
        }
        else if ( col >= colCount )
        {
            col = 0;
        }

        antRows[ i ] = row;
        antCols[ i ] = col;
    }
}



void langtonsAntUpdatePixelData( CellAutom * ptr )
{
    Color * pixelData = ptr->pixelData;


    const uint32_t count = ptr->count;
    const Color * const stateColours = ptr->staticMems.stateColours;
    const CellState * states = ptr->newStates;

    for ( uint32_t i = 0; i < count; i++ )
    {
        *pixelData++ = stateColours[ *states++ ];
    }


    pixelData = ptr->pixelData;


    const uint32_t antCount = ((LangtonsAntParams *)ptr->dynamicMemsPtr)->antCount;
    const int32_t cols = ptr->cols;

    int32_t * antRows = ((LangtonsAntParams *)ptr->dynamicMemsPtr)->antRows;
    int32_t * antCols = ((LangtonsAntParams *)ptr->dynamicMemsPtr)->antCols;

    for ( uint32_t i = 0; i < antCount; i++ )
    {
        pixelData[ *antRows++ * cols + *antCols++ ] = RED;
    }
}



void LangtonsAntParamsDestroy( CellAutomTypeDynamicMems * vPtr )
{
    LangtonsAntParams * ptr = (LangtonsAntParams *)vPtr;

    free( ptr->antInitPositions );
    free( ptr->antInitDirections );
    free( ptr->antRows );
    free( ptr->antCols );
    free( ptr->antDirections );

    memset( ptr, 0x00, sizeof( *ptr ) );
}
