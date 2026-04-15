#ifndef GRID_H
#define GRID_H

#include <stddef.h>
#include <stdint.h>

#include "raylib.h"



typedef struct _CellAuto CellAuto;

typedef uint32_t CellState;


typedef void CellAutoTypeDynamicMems;

typedef void CellAutoTypeParams;



typedef enum _CellAutoType {

    CellularAutomatonType_gameOfLife,
    CellularAutomatonType_briansBrain,
    CellularAutomatonType_langtonsAnt,

    CELLULAR_AUTOMATON_TYPE_COUNT

} CellAutoType;



typedef struct _CellAutoTypeStaticMems {

    uint32_t stateCount;
    const Color * stateColours;
    void ( *initStateFunc )( CellAuto *, const CellAutoTypeParams * );
    void ( *updateStateFunc )( CellAuto * );
    void ( *updatePixelDataFunc )( CellAuto * );

} CellAutoTypeStaticMems;




typedef struct _CellAuto {

    char name [ 128 ];
    CellAutoType type;
    CellAutoTypeStaticMems staticMems;
    CellAutoTypeParams * dynamicMemsPtr;

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

    uint32_t id;
    int32_t seed;
    CellState * initialStates;

} CellAuto;



void CellularAutomatonInit(
    CellAuto * ptr,
    CellAutoType type,
    CellAutoTypeParams * dynamicArgsPtr,
    int32_t rows,
    int32_t cols,
    const char * name,
    uint32_t id,
    uint32_t seed,
    CellState * initialStates
);

void CellularAutomatonDenit( CellAuto * ptr );

void CellularAutomatonUpdate( CellAuto * ptr );

void CellularAutomatonDraw( const CellAuto * ptr );



#endif // #ifndef GRID_H