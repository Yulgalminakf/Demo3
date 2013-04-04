
#pragma once
#include "MathLibrary.h"

namespace Color
{
	enum CommonColor
	{
		White,
		Silver,
		Gray,
		Black,
		Red,
		Marron,
		Yellow,
		Olive,
		Lime,
		Green,
		Aqua,
		Teal,
		Blue,
		Navy,
		Fuchsia,
		Purple,

		Count
	};

	Vector3 GetColor(CommonColor color);
};