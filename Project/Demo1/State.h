#pragma once
#include "main.h"
#include "Utility.h"
class Brain;

class State
{
	Brain *m_brain;
	char m_name[MAX_NAME_SIZE];
	unsigned int m_nameHash;

public:
	State(void);
	~State(void);

	virtual void Init();
	virtual void Update(float dt)		{}
	virtual void Start()				{}
	virtual void Stop()					{}
	virtual void Reset()				{}

	void SetName(const char* newName);
	unsigned int GetNameHash()	{ return m_nameHash; }
	const char* GetName()		{ return m_name; }
};

