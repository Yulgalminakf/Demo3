#include "Grid.h"
#include "GridManager.h"
#include "Graphics.h"

Grid::Grid(char *textureFilePath)
{
	LoadTexture(textureFilePath);
}

Grid::~Grid(void)
{
}

void Grid::Update(float dt, unsigned int index)
{
}

void Grid::Draw(const Rect &rect)
{
	Graphics* g = Graphics::Get();

	g->DrawRectWithTexture(rect, 0, m_textureHandle);
}

void Grid::LoadTexture(char* textureFilePath)
{
	m_textureHandle = Graphics::Get()->LoadTexture(textureFilePath);
}