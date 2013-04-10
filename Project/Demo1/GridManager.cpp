#include "GridManager.h"
#include "Utility.h"
#include "Graphics.h"
GridManager *GridManager::s_theGridManager = 0;


GridManager::GridManager(void)
{
	memset(m_gridTypes, 0, sizeof(Grid*) * GridType_Count);
	Graphics* g = Graphics::Get();

	m_gridTypes[GridType_Brick] = new Grid("Textures/TilingBrickTexture.tga");
	
	m_gridTypes[GridType_Empty] = new Grid("Textures/Player.tga");

	for(int i = 0; i < NUM_GRID; ++i)
	{
		SetGrid(i, GridType_Empty);
	}


	unsigned int i = 0;
	unsigned int max = 10;
	while(i < max)
	{
		unsigned int x,y;
		x = rand() % NUM_GRID_WIDTH;
		y = rand() % NUM_GRID_HEIGHT;

		if(GetGrid(x,y) != GridType_Brick)
		{
			SetGrid(x,y, GridType_Brick);
			++i;
		}
	}
}

GridManager::~GridManager(void)
{
	for(int i = 0; i < GridType_Count; ++i)
	{
		if(m_gridTypes[i])
		{
			delete m_gridTypes[i];
		}
	}
}

void GridManager::Update(float dt)
{
	for(int i = 0; i < NUM_GRID; ++i)
	{
		GridType type = GetGrid(i);
		if(type != GridType_Count)
		{
			m_gridTypes[type]->Update(dt, i);
		}
	}
}

void GridManager::Draw()
{
	Rect rect;
	rect.width = GRID_WIDTH;
	rect.height = GRID_HEIGHT;
	for(int x = 0; x < NUM_GRID_WIDTH; ++x)
	{
		for(int y = 0; y < NUM_GRID_HEIGHT; ++y)
		{
			rect.x = x * GRID_WIDTH;
			rect.y = -(y + 1) * GRID_HEIGHT;
			unsigned int index;
			ConvertXYToIndex(x,y,index);
			GridType gridType = GetGrid(index);
			if(gridType < GridType_Count)
			{
				m_gridTypes[gridType]->Draw(rect);
			}
		}
	}
}