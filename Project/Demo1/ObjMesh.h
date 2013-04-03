#ifndef __OBJMESH_H_
#define __OBJMESH_H_
#include "MathLibrary.h"
#include <vector>
#include "glew.h"

using namespace std;

class CObjMesh
{
public:

	CObjMesh();

	// Read the contents of the OBJ file into memory
	bool LoadFromFile( const char *sFileName );

	void Parse( const char* pBuffer );

	// Virtual functions called from base class
	void OnVertexCoordinate( float fX, float fY, float fZ );
	void OnVertexTextureCoordinate( float fU, float fV );
	void OnVertexNormal( float fX, float fY, float fZ );
	void OnFace( int *pVertexCoords, int *pTexCoords, int *pNormals, int nNumCoords );

	void Render( bool bUseFixedFunctions = true, GLenum drawType = GL_LINE);
	
	Vector3 GetVertex(unsigned int index)			{ return m_pVertexCoordinates[index]; }
	Vector2 GetTexCoord(unsigned int index)			{ return m_pTexCoordinates[index]; }
	Vector3 GetNormal(unsigned int index)			{ return m_pNormals[index]; }
	unsigned int GetIndex(unsigned int index)		{ return m_pIndices[index]; }
	
	unsigned int GetVertexSize()					{ return m_pVertexCoordinates.size(); }
	unsigned int GetTexCoordSize()					{ return m_pTexCoordinates.size(); }
	unsigned int GetNormalSize()					{ return m_pNormals.size(); }
	unsigned int GetIndicesSize()					{ return m_pIndices.size(); }

	Vector3 *GetVertexPointer()						{ return &m_pVertexCoordinates[0]; }
	Vector2 *GetTexCoordPointer()					{ return &m_pTexCoordinates[0]; }
	Vector3 *GetNormalPointer()						{ return &m_pNormals[0]; }
	unsigned int *GetIndicesPointer()				{ return &m_pIndices[0]; }

	void SetIndicies(const unsigned int *ptr, const unsigned int size);
	void SetVertecies(const Vector3 *ptr, const unsigned int size);
	void SetTexCoord(const Vector2 *ptr, const unsigned int size);
	void SetNormals(const Vector3 *ptr, const unsigned int size);

private:

	// Warning: Do not change the order of this enum!  See ParseCommandString() definition.
	enum EOb2Command
	{
		eTextureCoord, 
		eNormalCoord, 
		eVertexCoord, 
		eFace, 
		eNumKeywords
	};

	struct SFace
	{
		int m_nVertexCoordIdx;
		int m_nTexCoordIdx;
		int m_nNormalIdx;
		SFace(int nVertexCoordIdx, int nTexCoordIdx, int nNormalIdx)
		{
			m_nVertexCoordIdx = nVertexCoordIdx;
			m_nTexCoordIdx = nTexCoordIdx;
			m_nNormalIdx = nNormalIdx;
		}
		bool operator==(const SFace &oRHS)
		{
			if ((m_nVertexCoordIdx == oRHS.m_nVertexCoordIdx) &&
				(m_nTexCoordIdx == oRHS.m_nTexCoordIdx) &&
				(m_nNormalIdx == oRHS.m_nNormalIdx))
				return true;
			return false;
		};
	};

	struct SCompareFace
	{
		// Operators needed for storage in STL map 
		bool operator()( const SFace &oLHS, const SFace &oRHS ) const
		{
			if (oLHS.m_nVertexCoordIdx < oRHS.m_nVertexCoordIdx)
				return true;
			else if (oLHS.m_nVertexCoordIdx > oRHS.m_nVertexCoordIdx)
				return false;
			else
			{
				if (oLHS.m_nTexCoordIdx < oRHS.m_nTexCoordIdx)
					return true;
				else if (oLHS.m_nTexCoordIdx > oRHS.m_nTexCoordIdx)
					return false;
				else 
				{
					if (oLHS.m_nNormalIdx < oRHS.m_nNormalIdx)
						return true;
					else
						return false;
				}
			}
		}
	};

	void OnLine();

	EOb2Command ParseCommandString( char *sToken );

	char *GetToken();
	float GetFloatToken();
	int GetIntegerToken();

	// Vertex coordinates read in are stored in this buffer
	vector<Vector3> m_pVertexCoordinates;
	vector<Vector2> m_pTexCoordinates;
	vector<Vector3> m_pNormals;

	vector<unsigned int> m_pIndices;

	vector<SFace> m_pFaces;

	template<class T>
	void ArrayToVector(vector<T> &vecOut, const T *ptr, const unsigned int count)
	{
		unsigned int i = 0;
		vecOut.clear();
		vecOut.resize(count);

		for(const T* iPtr = ptr; i < count; ++iPtr, ++i)
		{
			vecOut[i] = *iPtr;
			//vecOut->push_back(*iPtr);
		}
	}

	// Current read position in the buffer
	char *m_pCurPos;

	// Current line number
	unsigned int m_nLine;

	// Read position of next line in buffer
	char *m_pNextLine;
};

#endif