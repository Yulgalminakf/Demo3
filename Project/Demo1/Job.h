#pragma once
#include "Global.h"
#include "DefinedUtilities.h"

enum JobType
{
	JobType_MoveBlock,
	JobType_Count
};

struct JobInfo
{
	JobType m_jobType;
};

enum Priority
{
	High,
	Medium,
	Low
};

#define MAX_WORKER_PER_JOB 10

class Job
{
protected:
	JobInfo *m_info;
	// the lower the number is, the higher the priority
	Priority m_priority;
	unsigned int m_index;
	unsigned int m_frameStamp;
	unsigned int m_numWorkers,m_numWorkersRecommended;
	// the workers performing the job(initialized to NULL)
	// *m_worker[MAX_WORKER_PER_JOB]
public:
	Job(void);
	~Job(void);

	GETSET(JobInfo*, m_info, JobInfo);
	GETSET(Priority, m_priority, Priority);
	GETSET(unsigned int, m_index, Index);
	GETSET(unsigned int, m_frameStamp, FrameStamp);

	virtual bool IsJobPossible()	{ return true; }
	virtual void Draw()				{}
	virtual void DebugDraw() = 0;
};

struct JobInfo_MoveBlock : public JobInfo
{
	GridCoord m_to, m_from;
};

class Job_MoveBlock : public Job
{
public:
	Job_MoveBlock();
	~Job_MoveBlock();
	virtual bool IsJobPossible();
	virtual void Draw();
	virtual void DebugDraw(){};
};