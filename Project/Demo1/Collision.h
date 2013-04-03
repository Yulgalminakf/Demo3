#pragma once

#include "MathLibrary.h"

struct Plane
{
	Vector3 m_normal, m_pos;

	Plane()
	{
		m_pos = m_normal = Vector3::zero;
	}

	Plane(Vector3 normal, Vector3 pos) : m_normal(normal), m_pos(pos) {}

	void Set(Vector3 normal, Vector3 pos)
	{
		m_normal = normal;
		m_pos = pos;
	}
};

struct Line
{
	Vector3 m_p1, m_p2;

	Line()
	{
		m_p1 = m_p2 = Vector3::zero;
	}

	Line(Vector3 p1, Vector3 p2) : m_p1(p1), m_p2(p2) {}

	void Set(Vector3 p1, Vector3 p2)
	{
		m_p1 = p1;
		m_p2 = p2;
	}
};

class CollisionInfo
{
public:

	CollisionInfo()
	{
		m_normal = Vector3::zero;
		m_depth = 0.0f;
		m_bHit = false;
	}

	Vector3 m_normal;
	Vector3 m_point;
	float m_depth;
	bool m_bHit;
};

class CollisionInfo_LinePlane : public CollisionInfo
{
public:
	CollisionInfo m_p1,m_p2;
};

class Collision
{
public:
	Collision(void);
	~Collision(void);
	
	static bool TestCollision(const Vector3 point, const Plane plane, CollisionInfo* infoOut = 0)
	{
		Vector3 v = point - plane.m_pos;
		//Vector3 vNormal = Vector3::Normalize(v);
		Vector3 normal = plane.m_normal;
		float depth = plane.m_normal.Dot(v);
		bool bHit = depth == 0.0f;
		Vector3 collPoint = (plane.m_normal * -depth) + point;

		if(infoOut)
		{
			infoOut->m_normal = normal;
			infoOut->m_depth = depth;
			infoOut->m_bHit = bHit;
			infoOut->m_point = collPoint;
		}

		return bHit;
	}

	static bool TestCollision(const Line line, const Plane plane, CollisionInfo_LinePlane* infoOut = 0)
	{
		Vector3 collPoint;
		bool hit = false;
		float depth = 0.0f;

		CollisionInfo info1,info2;
		TestCollision(line.m_p1, plane, &info1);
		TestCollision(line.m_p2, plane, &info2);

		infoOut->m_p1 = info1;
		infoOut->m_p2 = info2;

		if((info1.m_depth > 0 && info2.m_depth < 0) || (info1.m_depth < 0 && info2.m_depth > 0))
		{
			hit = true;
			if(info1.m_depth < 0)
			{
				Vector3 v = line.m_p2 - line.m_p1;
				float vLen = v.Length();
				v /= vLen;
				float combinedLength = fabs(info1.m_depth) + fabs(info2.m_depth);
				float percent = fabs(info1.m_depth) / combinedLength;
				depth = percent * vLen;
				collPoint = v * depth + line.m_p1;
			}
			else	
			{
				Vector3 v = line.m_p1 - line.m_p2;
				float vLen = v.Length();
				v /= vLen;
				float combinedLength = fabs(info1.m_depth) + fabs(info2.m_depth);
				float percent = fabs(info2.m_depth) / combinedLength;
				depth = percent * vLen;
				collPoint = v * depth + line.m_p2;
			}
		}

		if(infoOut)
		{
			infoOut->m_bHit = hit;
			infoOut->m_point = collPoint;
			infoOut->m_normal = plane.m_normal;
			infoOut->m_depth = depth;
		}

		return hit;
	}
};