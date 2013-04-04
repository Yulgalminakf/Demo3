#pragma once

#include "MathLibrary.h"

#define MAX_BRANCH_CONNECTIONS 10
#define MAX_BRANCHES 1000

struct Branch
{
	Vector3 m_base;
	unsigned int m_index;
	unsigned int m_numConnections;
	Branch* m_connections[MAX_BRANCH_CONNECTIONS];
	Branch* m_parent;
};

class Tree
{
	Branch *m_root;
	unsigned int m_numBranches;
	Branch m_branches[MAX_BRANCHES];

	void DrawBranch(Branch *branch);

public:
	Tree(void);
	~Tree(void);

	float m_maxPitchAngle;
	float m_maxRollAngle;
	float m_maxYawAngle;

	float m_minSize;
	float m_maxSize;
	float m_sizeFalloff;

	void Draw();
};