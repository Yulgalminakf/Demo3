#include "Job.h"
#include "GridManager.h"

Job::Job(void)
{
	m_info = 0;
	m_priority = 0;
}


Job::~Job(void)
{
	if(m_info)
	{
		delete m_info;
	}
}

Job_MoveBlock::Job_MoveBlock() : Job()
{
}

Job_MoveBlock::~Job_MoveBlock()
{
}

bool Job_MoveBlock::IsJobPossible()
{
	if(Job::IsJobPossible() == false)
	{
		return false;
	}

	JobInfo_MoveBlock* info = (JobInfo_MoveBlock*)m_info;

	if(info->m_from.x >= NUM_GRID_WIDTH || info->m_from.y >= NUM_GRID_HEIGHT)
	{
		return false;
	}
	if(info->m_to.x >= NUM_GRID_WIDTH || info->m_to.y >= NUM_GRID_HEIGHT)
	{
		return false;
	}

	return true;
}