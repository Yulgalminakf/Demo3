#include "Job.h"
#include "GridManager.h"
#include "Graphics.h"

Job::Job(void)
{
	m_info = 0;
	m_priority = Lowest;
	m_index = -1;
	m_frameStamp = 0;
	m_numWorkers = m_numWorkersRecommended = 1;
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
	if(info->m_to.x == info->m_from.x && info->m_to.y == info->m_from.y)
	{
		return false;
	}

	return true;
}

void Job_MoveBlock::Draw()
{
	const static int layer = 1;
	JobInfo_MoveBlock *info = (JobInfo_MoveBlock*)m_info;
	Graphics *g = Graphics::Get();
	Vector2 fromCenter = GridManager::GetGridCenter(info->m_from.x, info->m_from.y);
	Vector2 toCenter = GridManager::GetGridCenter(info->m_to.x, info->m_to.y);
	Vector2 size(GRID_WIDTH - 4,GRID_HEIGHT - 4);
	Vector2 borderSize(GRID_WIDTH,GRID_HEIGHT);
	g->DrawSquareWithColor(fromCenter, borderSize, layer,	Vector4(0,0,0,0.5f));
	g->DrawSquareWithColor(toCenter, borderSize, layer,		Vector4(0,0,0,0.5f));
	g->DrawSquareWithColor(fromCenter, size, layer + 1, Vector4(1,0,0.5f,0.3f));
	g->DrawSquareWithColor(toCenter, size, layer + 1, Vector4(0,1,0.5f,0.3f));

	glBegin(GL_LINES);
	g->SetValue("color",Vector4(0,0,0,1.0f));
	glVertex3f(fromCenter.x, fromCenter.y, layer);
	glVertex3f(toCenter.x, toCenter.y, layer);
	glEnd();
}