#include "Flocking.h"

struct Block
{
	int temp[Flocking::k_numBlocksWidth];
};

__global__ void Temp(int *a)
{
	fmod(10.0f, 3.5f);
	if(fmod(10.0f, 3.5f) == 1.0f)
	{
		fmod(10.0f, 3.5f);
	}
}