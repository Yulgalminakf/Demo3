#pragma once
#include <assert.h>
#include "glew.h"
#include <windows.h>
#include <stdio.h>
//#include "3dMath.h"
//#include "MathDefines.h"
#include <vector>
#include "LightSource.h"
#include "ObjMesh.h"
#include "MathLibrary.h"
#include "Array.h"
//#include "LightSource_SpotLight.h"

using namespace std;


#define MAX_NAME_LENGTH 256
#define MAX_HANDLES 512
#define MAX_MESHES 20
#define MAX_LIGHTS 30
#define MAX_LIGHTS_PER_SHADER 4

#define LIGHT_INTENSITY 10000
//
//enum ValueType
//{
//	VALUE_TYPE_FLOAT,
//	VALUE_TYPE_VECTOR3,
//	VALUE_TYPE_VECTOR4,
//	VALUE_TYPE_MATRIX,
//
//	VALUE_TYPE_NONE,
//	VALUE_TYPE_COUNT = VALUE_TYPE_NONE
//};
//
//struct Value
//{
//	ValueType valueType;
//	char variableName[256];
//	float fValue;
//	Vector3 v3Value;
//	Vector4 v4Value;
//	Matrix matValue;
//
//	Value() : valueType(VALUE_TYPE_NONE), fValue(0.0f), v3Value(Vector3::zero) {}
//	Value(float value, char* varName) : valueType(VALUE_TYPE_FLOAT), fValue(value) { strcpy(variableName, varName); }
//	Value(Vector3 value, char* varName) : valueType(VALUE_TYPE_VECTOR3), v3Value(value) { strcpy(variableName, varName); }
//	Value(Vector4 value, char* varName) : valueType(VALUE_TYPE_VECTOR4), v4Value(value) { strcpy(variableName, varName); }
//	Value(Matrix value, char* varName) : valueType(VALUE_TYPE_MATRIX), matValue(value) { strcpy(variableName, varName); }
//};

struct LightDistance
{
	LightSource light;
	float distSqr;
};

class Graphics
{
	enum HandleType
	{
		HANDLE_TYPE_SHADER_PROGRAM,
		HANDLE_TYPE_TEXTURE,
		HANDLE_TYPE_MESH,

		HANDLE_TYPE_COUNT
	};

	struct HandleHolder
	{
		HandleType handleType;
		char filename[MAX_NAME_LENGTH];
		char secondaryFilename[MAX_NAME_LENGTH];
		unsigned int handle;

		HandleHolder()	: handle((unsigned int)-1) { filename[MAX_NAME_LENGTH - 2] = '\0'; secondaryFilename[MAX_NAME_LENGTH - 2] = '\0'; }
		HandleHolder(HandleType _handleType, char* _filename, unsigned int _handle) : handleType(_handleType), handle(_handle) { strcpy_s(filename, _filename); }
		HandleHolder(HandleType _handleType, char* _filename, char* _secondaryFilename, unsigned int _handle) : handleType(_handleType), handle(_handle) { strcpy_s(filename, _filename); strcpy_s(secondaryFilename, _secondaryFilename); }
	};

	static Graphics* s_theGraphics;
	
	bool CheckError(GLuint obj);
	unsigned int m_currShaderProgram;
	unsigned int m_lastShaderProgram;
	
public:
	enum DefaultShaderProgramEnum
	{
		eShader_Default,
		eShader_Color,
		eShader_Texture,

		eShader_Count
	};

private:

	unsigned int m_defaultShaderPrograms[eShader_Count];

	unsigned int m_currTexture;

	HandleHolder m_handles[MAX_HANDLES];
	unsigned int m_numHandles;
	LightSource m_lights[MAX_LIGHTS];
	CObjMesh m_debugCubeMesh;

	Array<CObjMesh> m_meshes;
	unsigned int m_numMeshes;

	unsigned int m_lightShader;

	bool Find(unsigned int &handleOut, char* name, char* secondName = 0);

	bool m_bLight1;
	bool m_bLight2;
	bool m_bLight3;
	bool m_bLight4;
	bool m_bSpotLight1;
	bool m_bSpotLight2;
	bool m_bSpotLight3;
	bool m_bSpotLight4;

	void DrawSquare_imp(Vector2 center, Vector2 size, int layer, float rotation = 0);

public:
	Graphics(void);
	~Graphics(void);

	static Graphics* Get();

	void Update(float dt);
	
	GLenum LoadShaderProgram(char *sVertexShader, char *sPixelShader);
	void SetShaderSampler(GLenum oProg, char *sSamplerName, unsigned int nSlot, unsigned int nTexHandle);
	void SetShaderUniformFloat(GLenum oProg, char *sUniformName, float fVal1);
	void SetShaderUniformVec3(GLenum oProg, char *sUniformName, float fVal1, float fVal2, float fVal3);
	void SetShaderUniformVec4(GLenum oProg, char *sUniformName, float fVal1, float fVal2, float fVal3, float fVal4);
	void SetShaderUniformMat4(GLenum oProg, char *sUniformName, float *pVals);
	void SetShaderProgram(GLenum oProgram);
	void SetDefaultShaderProgram(DefaultShaderProgramEnum prog);

	void SetSampler(char *sSamplerName, unsigned int nSlot, unsigned int nTexHandle);
	void SetValue(char* varName, Vector3 value);
	void SetValue(char* varName, Vector4 value);
	void SetValue(char* varName, Matrix value);
	void SetValue(char* varName, float value);
	void SetAllLights(LightSource lights[]);
	void GetClosestLights(LightSource lightsOut[], Vector3 pos);
	void RenderAllLights(Matrix viewMat);
	unsigned int LoadTexture(char* filename);
	void SetTexture(unsigned int nTexHandle);
	unsigned int LoadMesh(char* filename);
	void SetAllLightsBasedOnPos(Vector3 pos);

	void RenderMesh(unsigned int meshHandle);
	void SetShaderVertexArray(GLenum oProg, char *sAttribName, GLint size, GLenum type, GLsizei stride, const void *pVals);
	void SetShaderArray(char* name, int size, unsigned int type, int stride, const void *pValues);
	void SetVertexPositions(const void *pPositions);
	void SetVertexNormals(const void *pNormals);
	void SetVertexTexCoords(const void *pTexCoords);

	void DrawDebugCube(float size, Vector3 pos, Vector4 color);
	void DrawDebugCube(Vector3 size, Vector3 pos, Vector4 color);
	void DrawDebugCube(Matrix mat, Vector4 color);
	
	void DrawSquareWithColor(Vector2 center, Vector2 size, int layer, Vector4 color);
	void DrawSquareWithColor(Vector2 center, Vector2 size, int layer, Vector4 color, float rotation);
	void DrawSquareWithTexture(Vector2 center, Vector2 size, int layer, unsigned int textureHandle);
	void DrawSquareWithTexture(Vector2 center, Vector2 size, int layer, unsigned int textureHandle, float rotation);

	//void DrawLine(Vector3 p1, Vector3 p2, Vector4 color);

	//void SetValue(Value v);
	//void SetAllProjLights(LightSource lights[]);
	//void GetClosestProjLights(LightSource lightsOut[], Vector3 pos);
	//void SetSpotLight(LightSource_SpotLight light, char number = 1);
	//void SetLight(char* lightName, LightSource light);
};
