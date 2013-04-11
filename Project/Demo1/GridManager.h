#pragma once
#include "DefinedUtilities.h"
#include "MathLibrary.h"
#include "main.h"
#include "Grid.h"

#define NUM_GRID_WIDTH		10
#define NUM_GRID_HEIGHT		10
#define NUM_GRID			NUM_GRID_HEIGHT * NUM_GRID_WIDTH

#define GRID_WIDTH			40
#define GRID_HEIGHT			40

struct GridCoords
{
	unsigned int x,y;
};

class GridManager
{
	static GridManager *s_theGridManager;

	GridType m_grid[NUM_GRID_WIDTH * NUM_GRID_HEIGHT];
	Grid *m_gridTypes[GridType_Count];

public:
	GridManager(void);
	~GridManager(void);

	STATIC_INSTANCE(GridManager, s_theGridManager);

	void Update(float dt);
	void Draw();

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Index/xy converters
	static void ConvertXYToIndex(const unsigned int x, const unsigned int y, unsigned int &indexOut)			{ indexOut = y * NUM_GRID_WIDTH + x; }
	static void ConvertIndexToXY(const unsigned int index, unsigned int &xOut, unsigned int &yOut)				{ yOut = index / NUM_GRID_WIDTH; xOut = index % NUM_GRID_WIDTH; }
	
	static void ConvertPosToXY(const Vector2 &screenPos, unsigned int &xOut, unsigned int &yOut)				{ xOut = screenPos.x / GRID_WIDTH; yOut = screenPos.y / GRID_HEIGHT; }
	static void ConvertPosToIndex(const Vector2 &screenPos, unsigned int &indexOut)								{ unsigned int x,y; ConvertPosToXY(screenPos,x,y); ConvertXYToIndex(x,y,indexOut); }
	static void ConvertMousePos(const Vector2 &mousePos, Vector2 &mousePosOut)									{ mousePosOut = g_screenSize - mousePos; }
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Grid getters and setters
	void SetGrid(unsigned int x, unsigned int y, GridType gridType)					{ unsigned int index = 0; ConvertXYToIndex(x,y,index); m_grid[index] = gridType; }
	void SetGrid(unsigned int index, GridType gridType)								{ m_grid[index] = gridType; }
	GridType GetGrid(unsigned int x, unsigned int y)								{ unsigned int index = 0; ConvertXYToIndex(x,y,index); return m_grid[index]; }
	GridType GetGrid(unsigned int index)											{ return m_grid[index]; }

	static Vector2 GetGridCenter(const unsigned int x, const unsigned int y)		
	{ 
		float xf,yf;
		xf = GRID_WIDTH * x + GRID_WIDTH / 2.0f;
		yf = -((float)GRID_HEIGHT * y) - (GRID_HEIGHT / 2.0f);
		return Vector2(xf,yf); 
	}
	static Vector2 GetGridCenter(const unsigned int index)							{ unsigned int x,y; ConvertIndexToXY(index,x,y); return GetGridCenter(x,y); }
	///////////////////////////////////////////////////////////////////////////////////////////////////////
};