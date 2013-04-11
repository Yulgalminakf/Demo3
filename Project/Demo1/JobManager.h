#pragma once
#include "Utility.h"
#include "Job.h"
#include "DefinedUtilities.h"

#define MAX_JOBS 100

class JobManager
{
	static JobManager *s_theJobManager;
	Job *m_jobs[MAX_JOBS];
	unsigned int m_numJobs;

	bool AddJob(Job *job);
	void RemoveJob(Job *job);

public:
	JobManager(void);
	~JobManager(void);

	// FindBestWorker
	STATIC_INSTANCE(JobManager, s_theJobManager);
	void Update(float dt);
	void Draw();
	void DebugDraw();

	bool AddJob(JobInfo *info, void *pData);
	Job *GetHighestPriorityJob();
};