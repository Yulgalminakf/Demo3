#include "Brain.h"

Brain::Brain(void)
{
}


Brain::~Brain(void)
{
}

void Brain::Init()
{
	m_numStates;
	memset(m_states, 0, sizeof(State*) * MAX_NUM_STATES);
	m_nextState = m_currState = m_lastState = 0;
}

void Brain::SetState(const char* stateName)
{
	State *state = GetState(stateName);
	if(state)
	{
		m_nextState = state;
	}
}

void Brain::Update(float dt)
{
	if(m_currState != m_nextState)
	{
		if(m_currState)
		{
			m_currState->Stop();
		}
		if(m_nextState)
		{
			m_nextState->Start();
			m_currState = m_nextState;
		}
	}

	if(m_currState)
	{
		m_currState->Update(dt);
	}
}

State* Brain::GetState(const char* stateName)
{
	unsigned int stateNameHash = StrHash(stateName);

	for(int i = 0; i < m_numStates; ++i)
	{
		if(m_states[i])
		{
			if(m_states[i]->GetNameHash() == stateNameHash)
			{
				return m_states[i];
			}
		}
	}

	return 0;
}

bool Brain::AddState(State *state)
{
	if(m_numStates == MAX_NUM_STATES)
	{
		return false;
	}

	if(!state)
	{
		return false;
	}

	m_states[m_numStates++] = state;

	return true;
}