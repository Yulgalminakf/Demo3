#pragma once
#include "Utility.h"

class Grid
{
	unsigned int m_textureHandle;

public:
	Grid(char *textureFilePath);
	~Grid(void);

	virtual void Update(float dt);
	virtual void Draw(const Rect &rect);

	void LoadTexture(char* textureFilePath);
};