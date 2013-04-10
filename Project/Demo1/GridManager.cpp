#include "GridManager.h"
#include "Utility.h"
#include "Graphics.h"
GridManager *GridManager::s_theGridManager = 0;


GridManager::GridManager(void)
{
	m_selectionIndex = 0;
	m_bSelected = false;

	m_gridTypeColors[GridType_Brick] = Vector4(1,0,0,1);
	m_gridTypeColors[GridType_Empty] = Vector4(1,1,1,1);

	for(int i = 0; i < NUM_GRID; ++i)
	{
		SetGrid(i, GridType_Empty);
	}

	SetGrid(NUM_GRID_WIDTH / 2.0f, NUM_GRID_HEIGHT / 2.0f, GridType_Brick);
}

GridManager::~GridManager(void)
{
}

void GridManager::Update(float dt)
{
	Input *input = Input::Get();
	if(m_bSelected == false &&		input->IsKeyDown(BUTTON_MOUSE_LEFT) == true)
	{
		m_bSelected = true;
		ConvertPosToIndex(input->GetMousePos(), m_selectionIndex);
	}
	else if(m_bSelected == true &&	input->IsKeyDown(BUTTON_MOUSE_LEFT) == false)
	{
		m_bSelected = false;
		unsigned int index;
		ConvertPosToIndex(input->GetMousePos(), index);
		Print("Attempting to switch %i and %i\n", m_selectionIndex, index);

		if(index < NUM_GRID && m_selectionIndex < NUM_GRID)
		{
			GridType type = GetGrid(index);
			SetGrid(index, GetGrid(m_selectionIndex));
			SetGrid(m_selectionIndex, type);
			Print("Switched!\n");
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
				Graphics::Get()->DrawRectWithColor(rect, 0, m_gridTypeColors[gridType]);
			}
		}
	}
}