#ifndef GRID_H
#define GRID_H

#include <stddef.h>
#include <stdint.h>

#include "raylib.h"



typedef uint32_t CellState;

typedef struct _CellularAutomaton CellularAutomaton;

typedef struct _CellularAutomaton {

    char name [ 128 ];
    size_t stateCount;
    Color * initStateColours;
    void ( *updateFunc )( CellularAutomaton * );

    int32_t rows;
    int32_t cols;

    CellState * newStates;
    CellState * oldStates;
    CellState * stateBuffer [ 2 ];
    Color * stateColours;

    uint64_t iterationCount;

    uint32_t id;
    int32_t seed;
    CellState * initialStates;

} CellularAutomaton;



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

);

void CellularAutomatonDenit( CellularAutomaton * ptr );

void CellularAutomatonUpdate( CellularAutomaton * ptr );

void CellularAutomatonDraw( const CellularAutomaton * ptr );



#endif // #ifndef GRID_H