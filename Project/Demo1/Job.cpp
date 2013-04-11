#include "Job.h"
#include "GridManager.h"
#include "Graphics.h"

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

void Job_MoveBlock::DebugDraw()
{
	const static int layer = 1;
	JobInfo_MoveBlock *info = (JobInfo_MoveBlock*)m_info;
	Graphics *g = Graphics::Get();
	Vector2 fromCenter = GridManager::GetGridCenter(info->m_from.x, info->m_from.y);
	Vector2 toCenter = GridManager::GetGridCenter(info->m_to.x, info->m_to.y);
	Vector2 size(10,10);

	g->DrawSquareWithColor(fromCenter, size, layer, Vector4(1,0,0.5f,1));
	g->DrawSquareWithColor(toCenter, size, layer, Vector4(0,1,0.5f,1));

	glBegin(GL_LINES);
	glColor3f(0.5f,0.5f,0);
	glVertex3f(fromCenter.x, fromCenter.y, layer);
	glVertex3f(toCenter.x, toCenter.y, layer);
	glEnd();
}