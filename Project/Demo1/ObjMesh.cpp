#include "ObjMesh.h"
#include "glew.h"
#include <map>
#include "Graphics.h"

//----------------------------------------------------------------------------------------------
// Public : CObjMesh
//----------------------------------------------------------------------------------------------
CObjMesh::CObjMesh()
{
	// Vertex coordinates read in are stored in this buffer
	m_pVertexCoordinates.clear();
	m_pTexCoordinates.clear();
	m_pNormals.clear();

	m_pIndices.clear();

	m_pFaces.clear();

	// Current read position in the buffer
	m_pCurPos = 0;

	// Current line number
	m_nLine = 0;

	// Read position of next line in buffer
	m_pNextLine = 0;
}

//----------------------------------------------------------------------------------------------
// Public : LoadFromFile
//----------------------------------------------------------------------------------------------
bool CObjMesh::LoadFromFile( const char *sFileName )
{
	FILE *pFile;
	if ( (fopen_s( &pFile, sFileName, "rb" )) != 0 )
		return false;

	// Get the size of the file
	fseek( pFile, 0L, SEEK_END );			// Position to end of file
	int nBufferSize = ftell( pFile );		// Get file length 
	rewind( pFile );						// Back to start of file

	// Read in the entire file and close the file handle
	char *pBuffer = new char[nBufferSize + 1];
	if ( fread( pBuffer, nBufferSize, 1, pFile ) <= 0 )
	{
		fclose( pFile );
		return false;
	}
	fclose( pFile );

	pBuffer[nBufferSize] = '\0';

	// Parse the contents of the file
	Parse( pBuffer );

	return true;	
}

//----------------------------------------------------------------------------------------------
// Public : Parse
//----------------------------------------------------------------------------------------------
void CObjMesh::Parse( const char* pBuffer )
{
	m_nLine = 0;
	m_pCurPos = strtok_s( (char*)pBuffer, "\r\n", (char**)&m_pNextLine );
	while( m_pCurPos ) 
	{
		m_nLine++;
		OnLine();
		m_pCurPos = strtok_s( NULL, "\r\n", (char**)&m_pNextLine );
	} 

	// Now we have a list of faces as well as vertex/tex/normals
	{
		/*
		// Create an index buffer, and get rid of duplicates.  This works, but it's REALLY slow!
		int nCurIndex = 0;
		for (int i = 0; i < (int)m_pFaces.size(); i++)
		{
		int nIndex = nCurIndex;

		// Search previous "faces" to see if one exists that matches the current one
		for (int nPrev = 0; nPrev <= i; nPrev++)
		{
		if (m_pFaces[i] == m_pFaces[nPrev])
		{
		nIndex = nPrev;
		break;
		}
		}
		m_pIndices.push_back(nIndex);

		if (nIndex == nCurIndex)
		nCurIndex++;
		}
		*/

		// Create an index buffer the stupid way.
		//int nCurIndex = 0;
		//for (int i = 0; i < (int)m_pFaces.size(); i++)
		//{
		//	int nIndex = nCurIndex++;
		//	m_pIndices.push_back(nIndex);
		//}

		if(m_pNormals.empty())
		{
			m_pNormals.resize(m_pVertexCoordinates.size());

			for(unsigned int i = 0; i < m_pNormals.size(); ++i)
			{
				m_pNormals[i].Set(0,0,0);
			}

			for(unsigned int i = 0; i < m_pFaces.size(); i += 3)
			{
				int indexA = m_pFaces[i].m_nVertexCoordIdx;
				int indexB = m_pFaces[i + 1].m_nVertexCoordIdx;
				int indexC = m_pFaces[i + 2].m_nVertexCoordIdx;
				Vector3 a = m_pVertexCoordinates[indexA];
				Vector3 b = m_pVertexCoordinates[indexB];
				Vector3 c = m_pVertexCoordinates[indexC];
				
				Vector3 u,v,normal;
				u = b - a;
				v = c - a;
				
				normal = Vector3::Cross(u,v);
				normal.Normalize();

				m_pNormals[indexA] += normal;
				m_pNormals[indexB] += normal;
				m_pNormals[indexC] += normal;

				m_pFaces[i].m_nNormalIdx = indexA;
				m_pFaces[i + 1].m_nNormalIdx = indexB;
				m_pFaces[i + 2].m_nNormalIdx = indexC;
			}

			for(unsigned int i = 0; i < m_pNormals.size(); ++i)
			{
				m_pNormals[i].Normalize();
			}
		}

		std::map<SFace, int, SCompareFace> pMap;

		// Temporary copy
		vector<Vector3> pVertexCoordinates = m_pVertexCoordinates;
		vector<Vector2> pTexCoordinates = m_pTexCoordinates;
		vector<Vector3> pNormals = m_pNormals;

		// Clear the existing arrays.  We're re-creating them.
		m_pVertexCoordinates.clear(); m_pTexCoordinates.clear(); m_pNormals.clear();

		unsigned int nIndexCounter = 0;
		for (int i = 0; i < (int)m_pFaces.size(); i++)
		{
			std::map<SFace, int, SCompareFace>::iterator oIter = pMap.find(m_pFaces[i]);

			// If found
			if ( oIter != pMap.end() )
			{
				// Add a previously-added index to the index buffer
				m_pIndices.push_back(oIter->second);
			}
			else
			{
				// Add to index buffer
				m_pIndices.push_back(nIndexCounter);

				// Add it to map
				pMap[m_pFaces[i]] = nIndexCounter;

				int nVertexCoordIdx = m_pFaces[i].m_nVertexCoordIdx;
				m_pVertexCoordinates.push_back( pVertexCoordinates[nVertexCoordIdx] );

				int nTexCoordIdx = m_pFaces[i].m_nTexCoordIdx;
				if (nTexCoordIdx >= 0)
					m_pTexCoordinates.push_back( pTexCoordinates[nTexCoordIdx] );

				int nNormalIdx = m_pFaces[i].m_nNormalIdx;
				if (nNormalIdx >= 0)
					m_pNormals.push_back( pNormals[nNormalIdx] );

				nIndexCounter++;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------
// Protected : OnLine
//----------------------------------------------------------------------------------------------
void CObjMesh::OnLine()
{
	char *sToken = GetToken();

	// It's possible to have an integer "modifier" appended to a command
	EOb2Command eCmd = ParseCommandString( sToken );

	switch (eCmd)
	{
	case eVertexCoord:
		{
			float fX = GetFloatToken();
			float fY = GetFloatToken();
			float fZ = GetFloatToken();
			OnVertexCoordinate( fX, fY, fZ );
		}
		break;
	case eTextureCoord:
		{
			float fX = GetFloatToken();
			float fY = GetFloatToken();

			OnVertexTextureCoordinate( fX, fY );
		}
		break;
	case eNormalCoord:
		{
			float fX = GetFloatToken();
			float fY = GetFloatToken();
			float fZ = GetFloatToken();
			OnVertexNormal( fX, fY, fZ );
		}
		break;
	case eFace:
		{
			int pVertexCoords[128];
			int pTexCoords[128];
			int pNormal[128];

			// Initialize to -1
			for (int i = 0; i < 128; i++)
				pNormal[i] = pTexCoords[i] = -1;

			// Tokens can be of the form <vertex>/<texture>/<normal>, or <vertex>/<texture> or <vertex>
			int nNumCoords = 0;
			while ( sToken = GetToken() )
			{
				string sVertexString( sToken );

				int nStart = 0;
				for ( int nNum = 0; nNum < 3; nNum++ )
				{
					unsigned int nEnd = (int)sVertexString.find_first_of("/", nStart);

					bool bExit = false;
					if ( nEnd == string::npos)
					{
						nEnd = sVertexString.size();
						bExit = true;
					}

					string sNumber = sVertexString.substr( nStart, nEnd-nStart );

					if ( nNum == 0 )
						pVertexCoords[nNumCoords] = atoi(sNumber.c_str());
					else if ( nNum == 1 )
						pTexCoords[nNumCoords] = atoi(sNumber.c_str());
					else if ( nNum == 2 )
						pNormal[nNumCoords] = atoi(sNumber.c_str());

					if ( bExit )
						break;

					nStart = nEnd + 1;
				}
				nNumCoords++;
			}
			OnFace( pVertexCoords, pTexCoords, pNormal, nNumCoords );

		}
		break;
	}
}

//----------------------------------------------------------------------------------------------
// Private : ParseCommandString
//----------------------------------------------------------------------------------------------
CObjMesh::EOb2Command CObjMesh::ParseCommandString( char *sToken )
{
	const char *pMap[eNumKeywords] =
	{
		{ "vt" },	// Have to search vt & vn before v!  This is because we use strncmp!
		{ "vn" },
		{ "v" },
		{ "f" },
	};

	// Identify the keyword in the beginning of the line (i.e., vertex? texture?
	for ( int nCmd = 0; nCmd < eNumKeywords; nCmd++ )
	{
		// Compare command prefix
		if ( strncmp( sToken, pMap[nCmd], strlen(pMap[nCmd]) ) == 0 )
		{
			return (EOb2Command)nCmd;
		}
	}

	return eNumKeywords;
}

//----------------------------------------------------------------------------------------------
// Public : OnVertexCoordinate
//----------------------------------------------------------------------------------------------
void CObjMesh::OnVertexCoordinate( float fX, float fY, float fZ ) 
{
	m_pVertexCoordinates.push_back( Vector3(fX, fY, fZ) );
}

//----------------------------------------------------------------------------------------------
// Public : OnVertexTextureCoordinate
//----------------------------------------------------------------------------------------------
void CObjMesh::OnVertexTextureCoordinate( float fU, float fV ) 
{
	m_pTexCoordinates.push_back( Vector2(fU, fV) );
}

//----------------------------------------------------------------------------------------------
// Public : OnVertexNormal
//----------------------------------------------------------------------------------------------
void CObjMesh::OnVertexNormal( float fX, float fY, float fZ ) 
{
	m_pNormals.push_back( Vector3(fX, fY, fZ) );
}

//----------------------------------------------------------------------------------------------
// Public : OnFace
//----------------------------------------------------------------------------------------------
void CObjMesh::OnFace( int *pVertexCoords, int *pTexCoords, int *pNormals, int nNumCoords ) 
{
	for ( int nIndex = 0; nIndex < nNumCoords; nIndex++ )
	{
		// Handle polygons (ie: 4 vertices implies 2 triangles, 5 vertices implies 3 triangles, etc)
		// This part effectively triangulates the face
		if ( nIndex >= 2 )
		{
			//int nVertexCoord, nTexCoord, nNormal;

			// Add a triangle
			m_pFaces.push_back(SFace(pVertexCoords[0] - 1, pTexCoords[0] - 1, pNormals[0] - 1));
			m_pFaces.push_back(SFace(pVertexCoords[nIndex - 1] - 1, pTexCoords[nIndex - 1] - 1, pNormals[nIndex - 1] - 1));
			m_pFaces.push_back(SFace(pVertexCoords[nIndex] - 1, pTexCoords[nIndex] - 1, pNormals[nIndex] - 1));
		}
	}
}

//----------------------------------------------------------------------------------------------
// Public : Render
//----------------------------------------------------------------------------------------------
void CObjMesh::Render(bool bUseFixedFunctions, GLenum drawType)
{
	if(bUseFixedFunctions)
	{
		if(!m_pVertexCoordinates.empty())
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glEnable(GL_CULL_FACE);

			// Wireframe mode
			//if(bWireframeMode)
			{
				glPolygonMode(GL_FRONT_AND_BACK, drawType); 
			}

			// Render grid
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, sizeof(Vector3), &m_pVertexCoordinates[0]);

			if (!m_pTexCoordinates.empty())
			{
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, sizeof(Vector2), &m_pTexCoordinates[0]);
			}

			if (!m_pNormals.empty())
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, sizeof(Vector3), &m_pNormals[0]);
			}

			glDrawElements(GL_TRIANGLES, (int)m_pIndices.size(), GL_UNSIGNED_INT, &m_pIndices[0]);

			// Restore to "fill" mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	else
	{
		Graphics* graphics = Graphics::Get();

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		if(!m_pVertexCoordinates.empty())
		{
			glEnable(GL_CULL_FACE);

			// Wireframe mode
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

			// Render grid
			//glVertexPointer(3, GL_FLOAT, sizeof(Vector3), &m_pVertexCoordinates[0]);
			graphics->SetVertexPositions(&m_pVertexCoordinates[0]);

			if (!m_pTexCoordinates.empty())
			{
				graphics->SetVertexTexCoords(&m_pTexCoordinates[0]);
				//glTexCoordPointer(2, GL_FLOAT, sizeof(Vector2), &m_pTexCoordinates[0]);
			}

			if (!m_pNormals.empty())
			{
				graphics->SetVertexNormals(&m_pNormals[0]);
				//glNormalPointer(GL_FLOAT, sizeof(Vector3), &m_pNormals[0]);
			}

			glDrawElements(GL_TRIANGLES, (int)m_pIndices.size(), GL_UNSIGNED_INT, &m_pIndices[0]);

			// Restore to "fill" mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
		}
	}
}

//----------------------------------------------------------------------------------------------
// Protected : GetToken
//----------------------------------------------------------------------------------------------
char *CObjMesh::GetToken()
{
	return strtok_s( NULL, "\t ", (char**)&m_pCurPos );
}

//----------------------------------------------------------------------------------------------
// Protected : GetFloatToken
//----------------------------------------------------------------------------------------------
float CObjMesh::GetFloatToken()
{
	char *sToken = GetToken();
	return (float)atof(sToken);
}

//----------------------------------------------------------------------------------------------
// Protected : GetIntegerToken
//----------------------------------------------------------------------------------------------
int CObjMesh::GetIntegerToken()
{
	char *sToken = GetToken();
	return (int)atoi(sToken);
}

void CObjMesh::SetIndicies(const unsigned int *ptr, const unsigned int size)
{
	ArrayToVector(m_pIndices, ptr, size);
}

void CObjMesh::SetVertecies(const Vector3 *ptr, const unsigned int size)
{
	ArrayToVector(m_pVertexCoordinates, ptr, size);
}

void CObjMesh::SetTexCoord(const Vector2 *ptr, const unsigned int size)
{
	ArrayToVector(m_pTexCoordinates, ptr, size);
}

void CObjMesh::SetNormals(const Vector3 *ptr, const unsigned int size)
{
	ArrayToVector(m_pNormals, ptr, size);
}