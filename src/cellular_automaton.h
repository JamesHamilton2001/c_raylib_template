#ifndef GRID_H
#define GRID_H

#include <stddef.h>
#include <stdint.h>

#include "raylib.h"



typedef enum _CellularAutomatonType {

    CellularAutomatonTypeBriansBrain,

    CELLULAR_AUTOMATON_TYPE_COUNT

} CellularAutomatonType;


typedef struct _CellularAutomaton CellularAutomaton;

typedef uint32_t CellState;


typedef struct _CellularAutomaton {

    char name [ 128 ];
    CellularAutomatonType type;

    size_t stateCount;
    Color * initStateColours;
    void ( *initStateFunc )( CellularAutomaton * );
    void ( *updateStateFunc )( CellularAutomaton * );
    void ( *updatePixelDataFunc )( CellularAutomaton * );

    int32_t rows;
    int32_t cols;
    uint32_t count;

    CellState * newStates;
    CellState * oldStates;
    CellState * stateBuffer [ 2 ];

    Color * stateColours;

    Color * pixelData;
    Image image;
    Texture2D texture;

    uint64_t iterationCount;

    uint32_t id;
    int32_t seed;
    CellState * initialStates;

} CellularAutomaton;



void CellularAutomatonInit(
    CellularAutomaton * ptr,

    CellularAutomatonType type,
    const char * name,
    const Color * stateInitColours,

    int32_t rows,
    int32_t cols,
    uint32_t id,
    uint32_t seed,
    CellState * initialStates
);

void CellularAutomatonDenit( CellularAutomaton * ptr );

void CellularAutomatonUpdate( CellularAutomaton * ptr );

void CellularAutomatonDraw( const CellularAutomaton * ptr );



#endif // #ifndef GRID_H