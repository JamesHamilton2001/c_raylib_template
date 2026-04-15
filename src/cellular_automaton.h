#ifndef GRID_H
#define GRID_H

#include <stddef.h>
#include <stdint.h>

#include "raylib.h"



typedef enum _CellAutomType CellAutomType;

typedef struct _CellAutom CellAutom;

typedef uint32_t CellState;

typedef struct _CellAutomTypeStaticMems CellAutomTypeStaticMems;

typedef void CellAutomTypeDynamicMems;



typedef enum _CellAutomType {

    CellularAutomatonType_gameOfLife,
    CellularAutomatonType_briansBrain,
    CellularAutomatonType_langtonsAnt,

    CELLULAR_AUTOMATON_TYPE_COUNT

} CellAutomType;



typedef struct _CellAutomTypeStaticMems {

    uint32_t stateCount;
    const Color * stateColours;
    void ( *initStateFunc )( CellAutom *, const CellAutomTypeDynamicMems * );
    void ( *updateStateFunc )( CellAutom * );
    void ( *updatePixelDataFunc )( CellAutom * );

} CellAutomTypeStaticMems;



typedef struct _CellAutom {

    CellAutomType type;
    CellAutomTypeStaticMems staticMems;
    CellAutomTypeDynamicMems * dynamicMemsPtr;

    int32_t rows;
    int32_t cols;
    uint32_t count;

    CellState * newStates;
    CellState * oldStates;
    CellState * stateBuffer [ 2 ];

    Color * pixelData;
    Image image;
    Texture2D texture;

    uint64_t iterationCount;

    char name [ 128 ];
    uint32_t id;
    int32_t seed;
    CellState * initialStates;

} CellAutom;



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
);

void CellAutomDeinit( CellAutom * ptr );

void CellAutomUpdate( CellAutom * ptr );

void CellUpdateTexture( const CellAutom * ptr );



#endif // #ifndef GRID_H