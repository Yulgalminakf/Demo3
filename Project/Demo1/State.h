#pragma once
#include "main.h"
#include "Utility.h"
#include "DefinedUtilities.h"
class Brain;

class State
{
private:
	char m_name[MAX_NAME_SIZE];
	unsigned int m_nameHash;

protected:
	Brain *m_brain;

public:
	State(void);
	~State(void);

	virtual void Init();
	virtual void Update(float dt)		{}
	virtual void Start()				{}
	virtual void Stop()					{}
	virtual void Reset()				{}

	GETSET(Brain*, m_brain, Brain);

	void SetName(const char* newName);
	unsigned int GetNameHash()	{ return m_nameHash; }
	const char* GetName()		{ return m_name; }
};