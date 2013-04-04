#include "Color.h"

Vector3 g_commonColors[Color::Count] = 
{
	Vector3(1,1,1),						//White
	Vector3(0.75f,0.75f,0.75f),			//Silver
	Vector3(0.5f,0.5f,0.5f),			//Gray
	Vector3(0,0,0),						//Black
	Vector3(1,0,0),						//Red
	Vector3(0.5f,0,0),					//Maroon
	Vector3(1,1,0),						//Yellow
	Vector3(0.5f,0.5f,0),				//Olive
	Vector3(0,1,0),						//Lime
	Vector3(0,0.5f,0),					//Green
	Vector3(0,1,1),						//Aqua
	Vector3(0,0.5f,0.5f),				//Teal
	Vector3(0,0,1),						//Blue
	Vector3(0,0,0.5f),					//Navy
	Vector3(1,0,1),						//Fuchsia
	Vector3(0.5f,0,0.5f),				//Purple
};

Vector3 Color::GetColor(CommonColor color)
{
	if(color == Count)
	{
		return Vector3::zero;
	}

	return g_commonColors[color];
}