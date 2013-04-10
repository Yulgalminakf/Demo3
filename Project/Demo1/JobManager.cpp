#include "JobManager.h"
#include "GridManager.h"
#include "main.h"
JobManager *JobManager::s_theJobManager = 0;

bool g_doJobs = false;

JobManager::JobManager(void)
{
#if TWEAK_MENU
	//	TwAddVarRW(TwkBar(), "Draw Neighbors", TW_TYPE_BOOL8, &boid->m_bDrawNeighbors, "");
	TwAddVarRW(TwkBar(), "Do Jobs", TW_TYPE_BOOL8, &g_doJobs, "");
#endif //TWEAK_MENU
}

JobManager::~JobManager(void)
{
}

void JobManager::Update(float dt)
{
	if(m_jobQueue.GetNumQueued() > 0 && g_doJobs)
	{
		Job *job = m_jobQueue.Next();
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

		delete job;
	}
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
				m_jobQueue.Add(newJob);
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