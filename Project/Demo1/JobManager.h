#pragma once
#include "Utility.h"
#include "Job.h"
#include "DefinedUtilities.h"

class JobManager
{
	static JobManager *s_theJobManager;
	Queue <Job*>m_jobQueue;

public:
	JobManager(void);
	~JobManager(void);

	// FindBestWorker
	STATIC_INSTANCE(JobManager, s_theJobManager);
	void Update(float dt);

	bool AddJob(JobInfo *info, void *pData);
};