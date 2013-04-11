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

class Job
{
protected:
	JobInfo *m_info;
	unsigned int m_priority;
	// the worker performing the job(initialized to NULL)
	// *m_worker
public:
	Job(void);
	~Job(void);

	GETSET(JobInfo*, m_info, JobInfo);
	GETSET(unsigned int, m_priority, Priority);

	virtual bool IsJobPossible()	{ return true; }
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
	virtual void DebugDraw();
};