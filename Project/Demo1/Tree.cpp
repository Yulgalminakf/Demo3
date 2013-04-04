#include "Tree.h"
#include <windows.h>
#include "main.h"
#include "glew.h"

Tree::Tree(void)
{
	memset(m_branches, 0, sizeof(Branch) * MAX_BRANCHES);
	
	for(int i = 0; i < MAX_BRANCHES; ++i)
	{
		m_branches[i].m_index = i;
	}

	m_root = 0;
	m_maxPitchAngle = 80.0f;
	m_maxRollAngle = 50.0f;
	m_maxYawAngle = 50.0f;

	m_minSize = 1.0f;
	m_maxSize = 10.0f;
	m_sizeFalloff = 1.0f;

#if TWEAK_MENU
	TwAddVarRW(TwkBar(), "Max Pitch Angle", TW_TYPE_FLOAT, &m_maxPitchAngle, "min=0 max=180 precision=4");
	TwAddVarRW(TwkBar(), "Max Roll Angle", TW_TYPE_FLOAT, &m_maxRollAngle, "min=0 max=180 precision=4");
	TwAddVarRW(TwkBar(), "Max Yaw Angle", TW_TYPE_FLOAT, &m_maxYawAngle, "min=0 max=180 precision=4");

	TwAddVarRW(TwkBar(), "Min Size", TW_TYPE_FLOAT, &m_minSize, "min=0 max=180 precision=4");
	TwAddVarRW(TwkBar(), "Max Size", TW_TYPE_FLOAT, &m_maxSize, "min=0 max=180 precision=4");
	TwAddVarRW(TwkBar(), "Size Falloff", TW_TYPE_FLOAT, &m_sizeFalloff, "min=0 max=180 precision=4");
#endif //TWEAK_MENU
}


Tree::~Tree(void)
{
#if TWEAK_MENU
	TwRemoveVar(TwkBar(), "Max Pitch Angle");
	TwRemoveVar(TwkBar(), "Max Roll Angle");
	TwRemoveVar(TwkBar(), "Max Yaw Angle");
	TwRemoveVar(TwkBar(), "Min Size");
	TwRemoveVar(TwkBar(), "Max Size");
	TwRemoveVar(TwkBar(), "Size Falloff");
#endif //TWEAK_MENU
}

void Tree::DrawBranch(Branch *branch)
{
	if(branch == NULL)
	{
		return;
	}

	if(branch->m_parent)
	{
		Vector3 p1 = branch->m_parent->m_base;
		Vector3 p2 = branch->m_base;

		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
	}

	for(unsigned int i = 0; i < branch->m_numConnections; ++i)
	{
		DrawBranch(branch->m_connections[i]);
	}
}

void Tree::Draw()
{
	glBegin(GL_LINES);
	glColor3f(1,1,1);
	DrawBranch(m_root);
	glEnd();
}
