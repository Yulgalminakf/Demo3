#pragma once
#include "Utility.h"

enum GridType
{
	GridType_Brick,
	GridType_Empty,

	GridType_Count
};

class Grid
{
	unsigned int m_textureHandle;

public:
	Grid(char *textureFilePath);
	~Grid(void);

	virtual void Update(float dt, unsigned int index);
	virtual void Draw(const Rect &rect);

	void LoadTexture(char* textureFilePath);
};