#pragma once
#include "State.h"

#define MAX_NUM_STATES 10

class Brain
{
	unsigned int m_numStates;
	State *m_states[MAX_NUM_STATES];
	State *m_nextState, *m_currState, *m_lastState;

public:
	Brain(void);
	~Brain(void);

	virtual void Init();
	virtual void SetState(const char* stateName);
	virtual void Update(float dt);
	virtual State* GetState(const char* stateName);
	virtual bool AddState(State *state);
};