#include "State.h"
#include "Brain.h"

State::State(void)
{
}


State::~State(void)
{
}

void State::Init()
{
	m_brain = 0;
	m_nameHash = 0;
}

void State::SetName(const char* newName)
{
	strcpy_s(m_name, newName);
	m_nameHash = StrHash(newName);
}