#include "JobManager.h"
#include "GridManager.h"
#include "main.h"
JobManager *JobManager::s_theJobManager = 0;

bool g_doJobs = false;
bool g_debugDraw = false;

JobManager::JobManager(void)
{
	m_numJobs = 0;
#if TWEAK_MENU
	//	TwAddVarRW(TwkBar(), "Draw Neighbors", TW_TYPE_BOOL8, &boid->m_bDrawNeighbors, "");
	TwAddVarRW(TwkBar(), "Do Jobs", TW_TYPE_BOOL8, &g_doJobs, "");
	TwAddVarRW(TwkBar(), "Debug Draw", TW_TYPE_BOOL8, &g_debugDraw, "");
	TwAddVarRO(TwkBar(), "Number of Jobs Queued", TW_TYPE_INT32, &m_numJobs, "");
#endif //TWEAK_MENU
}

JobManager::~JobManager(void)
{
}

void JobManager::Update(float dt)
{
	if(m_numJobs && g_doJobs)
	{
		Job *job = GetHighestPriorityJob();
		JobInfo *info = job->GetJobInfo();
		switch(info->m_jobType)
		{
		case JobType_MoveBlock:
			{
				GridManager *gm = GridManager::Get();
				JobInfo_MoveBlock *moveBlockInfo = (JobInfo_MoveBlock*)info;
				GridType toType = gm->GetGrid(moveBlockInfo->m_to.x, moveBlockInfo->m_to.y);
				GridType fromType = gm->GetGrid(moveBlockInfo->m_from.x, moveBlockInfo->m_from.y);
				gm->SetGrid(moveBlockInfo->m_to.x, moveBlockInfo->m_to.y, fromType);
				gm->SetGrid(moveBlockInfo->m_from.x, moveBlockInfo->m_from.y, toType);
			}
			break;
		default:
			break;
		}

		RemoveJob(job);

		delete job;
	}
}

void JobManager::Draw()
{
	for(int i = 0; i < m_numJobs; ++i)
	{
		Job *job = m_jobs[i];
		job->Draw();
	}

	if(g_debugDraw)
	{
		DebugDraw();
	}
}

void JobManager::DebugDraw()
{
	for(int i = 0, iMax = m_numJobs; i < iMax; ++i)
	{
		Job *job = m_jobs[i];
		job->DebugDraw();
	}
}

void JobManager::RemoveJob(Job *job)
{
	unsigned int index = job->GetIndex();
	assert(m_jobs[index] == job);
	m_jobs[index] = m_jobs[--m_numJobs];
	m_jobs[index]->SetIndex(index);
}

bool JobManager::AddJob(Job *job)
{
	if(m_numJobs < MAX_JOBS)
	{
		m_jobs[m_numJobs] = job;
		job->SetIndex(m_numJobs);
		job->SetFrameStamp(g_frameNumber);
		m_numJobs++;
		return true;
	}

	return false;
}

bool JobManager::AddJob(JobInfo *info, void *pData)
{
	switch(info->m_jobType)
	{
	case JobType_MoveBlock:
		{
			Job* newJob = new Job_MoveBlock();
			newJob->SetJobInfo(info);
			if(newJob->IsJobPossible())
			{
				if(!AddJob(newJob))
				{
					delete newJob;
				}
			}
			else
			{
				delete newJob;
			}
		}
		return true;
	default:
		return false;
	}
}

// the lower the number is, the higher the priority
Job *JobManager::GetHighestPriorityJob()
{
	if(m_numJobs == 0)
	{
		return NULL;
	}

	Job *highestPriorityJob = NULL;
	unsigned int highestPriority = 100000;
	unsigned int lowestFrameStamp = 100000;
	for(int i = 0; i < m_numJobs; ++i)
	{
		Job *iJob = m_jobs[i];
		if(iJob->GetPriority() == highestPriority)
		{
			if(iJob->GetFrameStamp() < lowestFrameStamp)
			{
				highestPriority = iJob->GetPriority();
				lowestFrameStamp = iJob->GetFrameStamp();
				highestPriorityJob = iJob;
			}
		}
		else if(iJob->GetPriority() < highestPriority)
		{
			highestPriority = iJob->GetPriority();
			lowestFrameStamp = iJob->GetFrameStamp();
			highestPriorityJob = iJob;
		}
	}

	return highestPriorityJob;
}