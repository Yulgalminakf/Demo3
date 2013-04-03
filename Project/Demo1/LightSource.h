#pragma once
#include "MathLibrary.h"

struct LightSource
{
	Matrix m_matrix;
	Vector3 m_lightIntensity;

	Vector3 GetPos()				{ return m_matrix.GetTranslation(); }
	void SetPos(Vector3 pos)		{ m_matrix.SetTranslation(pos); }
};