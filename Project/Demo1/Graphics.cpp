#include "Graphics.h"
#include <string>
#include "Tga.h"
#include "Utility.h"
//#include "Projectile.h"

//extern vector<Projectile*> g_projs;
Graphics* Graphics::s_theGraphics = 0;

extern Matrix g_viewMat;

Graphics::Graphics(void)
{
	m_debugCubeMesh.LoadFromFile("cube.obj");
	m_currShaderProgram = 0;
	m_lastShaderProgram = 0;

	m_meshes.Resize(MAX_MESHES);
	m_numMeshes = 0;
	//m_handles.Resize(MAX_HANDLES);
	m_numHandles = 0;

	float spacing = 100.0f;
	int index = 0;
	int rows = 5;
	int columns = 6;
	float intensity = LIGHT_INTENSITY;
	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < columns; ++j)
		{
			float x, y, z;

			x = (i - rows / 2) * spacing;
			z = (j - columns / 2) * spacing;
			y = 50.0f;

			Vector3 lightPos(x, y, z);
			Vector3 lightIntensity((rand() % 100) / 100.0f * intensity, (rand() % 100) / 100.0f * intensity, (rand() % 100) / 100.0f * intensity);
			//Vector3 lightIntensity(10000,10000,10000);
			m_lights[index].m_lightIntensity = lightIntensity;
			m_lights[index].SetPos(lightPos);
			++index;
		}
	}

	m_lightShader = LoadShaderProgram("Shaders/default_vs.glsl", "Shaders/default_ps.glsl");
	m_defaultShaderPrograms[eShader_Default] = LoadShaderProgram("Shaders/default_vs.glsl", "Shaders/default_ps.glsl");
	m_defaultShaderPrograms[eShader_Color] = LoadShaderProgram("Shaders/color_vs.glsl", "Shaders/color_ps.glsl");
	m_defaultShaderPrograms[eShader_Texture] = LoadShaderProgram("Shaders/texture_vs.glsl", "Shaders/texture_ps.glsl");
	m_currTexture = (unsigned int)-1;

	
	m_bLight1 = true;
	m_bLight2 = true;
	m_bLight3 = true;
	m_bLight4 = true;
	m_bSpotLight1 = true;
	m_bSpotLight2 = true;
	m_bSpotLight3 = true;
	m_bSpotLight4 = true;
}

Graphics::~Graphics(void)
{
}

Graphics* Graphics::Get()
{
	if(s_theGraphics == 0)
	{
		s_theGraphics = new Graphics();
	}

	return s_theGraphics;
}

void Graphics::Update(float dt)
{
	Input* input = Input::Get();

	if(input->IsKeyTapped(BUTTON_1))
	{
		m_bLight1 = !m_bLight1;
	}
	if(input->IsKeyTapped(BUTTON_2))
	{
		m_bLight2 = !m_bLight2;
	}
	if(input->IsKeyTapped(BUTTON_3))
	{
		m_bLight3 = !m_bLight3;
	}
	if(input->IsKeyTapped(BUTTON_4))
	{
		m_bLight4 = !m_bLight4;
	}
	if(input->IsKeyTapped(BUTTON_5))
	{
		m_bSpotLight1 = !m_bSpotLight1;
	}
	if(input->IsKeyTapped(BUTTON_6))
	{
		m_bSpotLight1 = !m_bSpotLight2;
	}
	if(input->IsKeyTapped(BUTTON_7))
	{
		m_bSpotLight1 = !m_bSpotLight3;
	}
	if(input->IsKeyTapped(BUTTON_8))
	{
		m_bSpotLight1 = !m_bSpotLight4;
	}
}

//----------------------------------------------------------------------------------------------
void Graphics::SetShaderSampler(GLenum oProg, char *sSamplerName, unsigned int nSlot, unsigned int nTexHandle)
{
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE0 + nSlot);
	glBindTexture(GL_TEXTURE_2D, nTexHandle);
	unsigned int nSamplerLoc = glGetUniformLocation(oProg, sSamplerName);
	glUniform1iARB(nSamplerLoc, nSlot);
}

void Graphics::SetSampler(char *sSamplerName, unsigned int nSlot, unsigned int nTexHandle)
{
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE0 + nSlot);
	glBindTexture(GL_TEXTURE_2D, nTexHandle);
	unsigned int nSamplerLoc = glGetUniformLocation(m_currShaderProgram, sSamplerName);
	glUniform1iARB(nSamplerLoc, nSlot);
}

//----------------------------------------------------------------------------------------------
bool Graphics::CheckError(GLuint obj)
{
	int infologLength = 0;
	int charsWritten  = 0;

	int nStatus;
	glGetShaderiv(obj, GL_COMPILE_STATUS, &nStatus);
	if (nStatus == GL_TRUE)
		return true;	// Compiled successfully

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
		GLchar *infoLog = new GLchar[infologLength];
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		assert(false && infoLog);
		delete [] infoLog;
	}

	return false;
}

//----------------------------------------------------------------------------------------------
GLenum Graphics::LoadShaderProgram(char *sVertexShader, char *sPixelShader)
{
	GLenum oProgram;

	if(Find(oProgram, sVertexShader, sPixelShader))
	{
		return oProgram;
	}

	//for(int i = 0; i < m_handles.size(); ++i)
	//{
	//	HandleHolder h = m_handles[i];

	//	if(h.handleType == HANDLE_TYPE_SHADER_PROGRAM)
	//	{
	//		if(strcmp(h.filename, sVertexShader) == 0 && strcmp(h.secondaryFilename, sPixelShader) == 0)
	//		{
	//			return h.handle;
	//		}
	//	}
	//}

	char *pVSBuffer;
	char *pPSBuffer;

	{
		// Reading a binary file
		FILE *pFile = NULL;
		if ( (pFile = fopen( sVertexShader, "rb" )) == 0 )
			return false;

		// Get the size of the file
		fseek( pFile, 0L, SEEK_END );			// Position to end of file
		unsigned int nBufferSize = ftell( pFile );		// Get file length 
		rewind( pFile );						// Back to start of file

		// Read in the entire file and close the file handle
		pVSBuffer = new char[nBufferSize+1];
		if ( fread( pVSBuffer, nBufferSize, 1, pFile ) <= 0 )
		{
			fclose( pFile );
			return false;
		}
		pVSBuffer[nBufferSize] = '\0';
		fclose( pFile );

		if ( (pFile = fopen( sPixelShader, "rb" )) == 0 )
			return false;

		// Get the size of the file
		fseek( pFile, 0L, SEEK_END );			// Position to end of file
		nBufferSize = ftell( pFile );		// Get file length 
		rewind( pFile );						// Back to start of file

		// Read in the entire file and close the file handle
		pPSBuffer = new char[nBufferSize+1];
		if ( fread( pPSBuffer, nBufferSize, 1, pFile ) <= 0 )
		{
			fclose( pFile );
			return false;
		}
		pPSBuffer[nBufferSize] = '\0';
		fclose( pFile );
	}

	// Create Shader And Program Objects
	oProgram = glCreateProgramObjectARB();
	GLenum oVS = glCreateShaderObjectARB(GL_VERTEX_SHADER);
	GLenum oPS = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	// Load Shader Sources
	glShaderSourceARB(oVS, 1, (const char **)&pVSBuffer, NULL);
	glShaderSourceARB(oPS, 1, (const char **)&pPSBuffer, NULL);

	// Compile The Shaders
	glCompileShaderARB(oVS); CheckError(oVS);
	glCompileShaderARB(oPS); CheckError(oPS);

	// Attach The Shader Objects To The Program Object
	glAttachObjectARB(oProgram, oVS);
	glAttachObjectARB(oProgram, oPS);

	// Link The Program Object
	glLinkProgramARB(oProgram);

	delete [] pVSBuffer;
	delete [] pPSBuffer;

	m_handles[m_numHandles++] = HandleHolder(HANDLE_TYPE_SHADER_PROGRAM, sVertexShader, sPixelShader, oProgram);

	return oProgram;
}


//----------------------------------------------------------------------------------------------
void Graphics::SetShaderUniformVec3(GLenum oProg, char *sUniformName, float fVal1, float fVal2, float fVal3)
{
	unsigned int nMatLoc = glGetUniformLocation(oProg, sUniformName);
	glUniform3fARB(nMatLoc, fVal1, fVal2, fVal3);
}

//----------------------------------------------------------------------------------------------
void Graphics::SetShaderUniformFloat(GLenum oProg, char *sUniformName, float fVal1)
{
	unsigned int nMatLoc = glGetUniformLocation(oProg, sUniformName);
	glUniform1fARB(nMatLoc, fVal1);
}

//----------------------------------------------------------------------------------------------
void Graphics::SetShaderUniformVec4(GLenum oProg, char *sUniformName, float fVal1, float fVal2, float fVal3, float fVal4)
{
	unsigned int nMatLoc = glGetUniformLocation(oProg, sUniformName);
	glUniform4fARB(nMatLoc, fVal1, fVal2, fVal3, fVal4);
}

//----------------------------------------------------------------------------------------------
void Graphics::SetShaderUniformMat4(GLenum oProg, char *sUniformName, float *pVals)
{
	unsigned int nMatLoc = glGetUniformLocation(oProg, sUniformName);
	glUniformMatrix4fvARB(nMatLoc, 1, false, pVals);
}

//----------------------------------------------------------------------------------------------
void Graphics::SetShaderProgram(GLenum oProgram)
{
	m_lastShaderProgram = m_currShaderProgram;
	m_currShaderProgram = oProgram;
	glUseProgramObjectARB(oProgram);
}

void Graphics::SetDefaultShaderProgram(DefaultShaderProgramEnum prog)
{
	if(prog == eShader_Count)
	{
		return;
	}

	SetShaderProgram(m_defaultShaderPrograms[prog]);
}

//void Graphics::SetValue(Value v)
//{
//	switch(v.valueType)
//	{
//	case VALUE_TYPE_FLOAT:
//		SetShaderUniformFloat(m_currShaderProgram, v.variableName, v.fValue);
//		break;
//	case VALUE_TYPE_VECTOR3:
//		
//		break;
//	case VALUE_TYPE_VECTOR4:
//		
//		break;
//	case VALUE_TYPE_MATRIX:
//		SetShaderUniformMat4(m_currShaderProgram, v.variableName, v.matValue.GetBuffer());
//		break;
//	case VALUE_TYPE_NONE:
//		break;
//	default:
//		break;
//	}
//}

void Graphics::SetValue(char* varName, Vector3 value)
{
	SetShaderUniformVec3(m_currShaderProgram, varName, value.x, value.y, value.z);
}

void Graphics::SetValue(char* varName, Vector4 value)
{
	SetShaderUniformVec4(m_currShaderProgram, varName, value.x, value.y, value.z, value.w);
}

void Graphics::SetValue(char* varName, Matrix value)
{
	SetShaderUniformMat4(m_currShaderProgram, varName, value.GetBuffer());
}

void Graphics::SetValue(char* varName, float value)
{
	SetShaderUniformFloat(m_currShaderProgram, varName, value);
}

void Graphics::SetAllLightsBasedOnPos(Vector3 pos)
{
	LightSource lights[4];
	GetClosestLights(lights, pos);
	SetAllLights(lights);
}

/*
void Graphics::SetAllProjLights(LightSource lights[])
{
	SetValue("projLightIntensity1", lights[0].m_lightIntensity);
	SetValue("projLightPosition1", lights[0].GetPos());

	SetValue("projLightIntensity2", lights[1].m_lightIntensity);
	SetValue("projLightPosition2", lights[1].GetPos());

	SetValue("projLightIntensity3", lights[2].m_lightIntensity);
	SetValue("projLightPosition3", lights[2].GetPos());

	SetValue("projLightIntensity4", lights[3].m_lightIntensity);
	SetValue("projLightPosition4", lights[3].GetPos());
}
*/

/*
void Graphics::GetClosestProjLights(LightSource lightsOut[], Vector3 pos)
{
	LightDistance lights[MAX_LIGHTS_PER_SHADER];

	for(int i = 0; i < MAX_LIGHTS_PER_SHADER; ++i)
	{
		lights[i].light.m_lightIntensity.Set(0,0,0);
		lights[i].light.SetPos(Vector3(0,0,0));
		lights[i].distSqr = 9999999;
	}

	for(int i = 0; i < g_projs.size(); ++i)
	{
		LightSource iLight = g_projs[i]->GetLight();
		float distSqr = (iLight.GetPos() - pos).LengthSqr();

		for(int j = 0; j < MAX_LIGHTS_PER_SHADER; ++j)
		{
			float checkDist = lights[j].distSqr;
			if(distSqr < checkDist)
			{
				lights[j].light = iLight;
				lights[j].distSqr = distSqr;
				break;
			}
		}
	}

	for(int i = 0; i < MAX_LIGHTS_PER_SHADER; ++i)
	{
		lightsOut[i] = lights[i].light;
	}
}
*/

void Graphics::GetClosestLights(LightSource lightsOut[], Vector3 pos)
{
	LightDistance lights[MAX_LIGHTS_PER_SHADER];

	for(int i = 0; i < MAX_LIGHTS_PER_SHADER; ++i)
	{
		lights[i].light = m_lights[i];
		lights[i].distSqr = (m_lights[i].GetPos() - pos).LengthSqr();
	}

	for(int i = MAX_LIGHTS_PER_SHADER; i < MAX_LIGHTS; ++i)
	{
		LightSource iLight = m_lights[i];
		float distSqr = (iLight.GetPos() - pos).LengthSqr();

		for(int j = 0; j < MAX_LIGHTS_PER_SHADER; ++j)
		{
			float checkDist = lights[j].distSqr;
			if(distSqr < checkDist)
			{
				lights[j].light = iLight;
				lights[j].distSqr = distSqr;
				break;
			}
		}
	}

	/*for(int i = 0; i < g_projs.size(); ++i)
	{
		Projectile* iProj = g_projs[i];
		float distSqr = (iProj->GetPos() - pos).LengthSqr();

		for(int j = 0; j < MAX_LIGHTS_PER_SHADER; ++j)
		{
			float checkDist = lights[j].distSqr;
			if(distSqr < checkDist)
			{
				lights[j].light = iProj->GetLight();
				lights[j].distSqr = distSqr;
				break;
			}
		}
	}*/

	for(int i = 0; i < MAX_LIGHTS_PER_SHADER; ++i)
	{
		lightsOut[i] = lights[i].light;
	}
}

void Graphics::RenderAllLights(Matrix viewMat)
{
	SetShaderProgram(m_lightShader);
	for(int i = 0; i < MAX_LIGHTS; ++i)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((viewMat * m_lights[i].m_matrix).GetBuffer());
		SetValue("color", m_lights[i].m_lightIntensity / LIGHT_INTENSITY);
		m_debugCubeMesh.Render();
	}
}

void Graphics::SetAllLights(LightSource lights[])
{
	if(m_bLight1)
	{
		SetValue("lightIntensity1", lights[0].m_lightIntensity);
		SetValue("lightPosition1", lights[0].GetPos());
	}
	else
	{
		SetValue("lightIntensity1", Vector3::zero);
		SetValue("lightPosition1", Vector3::zero);
	}

	if(m_bLight2)
	{
		SetValue("lightIntensity2", lights[1].m_lightIntensity);
		SetValue("lightPosition2", lights[1].GetPos());
	}
	else
	{
		SetValue("lightIntensity2", Vector3::zero);
		SetValue("lightPosition2", Vector3::zero);
	}

	if(m_bLight3)
	{
		SetValue("lightIntensity3", lights[2].m_lightIntensity);
		SetValue("lightPosition3", lights[2].GetPos());
	}
	else
	{
		SetValue("lightIntensity3", Vector3::zero);
		SetValue("lightPosition3", Vector3::zero);
	}

	if(m_bLight4)
	{
		SetValue("lightIntensity4", lights[3].m_lightIntensity);
		SetValue("lightPosition4", lights[3].GetPos());
	}
	else
	{
		SetValue("lightIntensity4", Vector3::zero);
		SetValue("lightPosition4", Vector3::zero);
	}
}

unsigned int Graphics::LoadTexture(char* filename)
{
	unsigned int nTexHandle;

	if(Find( nTexHandle,filename))
	{
		return nTexHandle;
	}

	GeneralLib::CTga oTga;
	oTga.Read(filename);
	unsigned int nFormat = GL_BGRA, nComponents = 4;
	if (oTga.GetBytesPerPixel() == 3)
	{
		nComponents = 3;
		nFormat = GL_BGR;
	}

	glGenTextures(1, &nTexHandle);
	glBindTexture(GL_TEXTURE_2D, nTexHandle);
	glTexImage2D (GL_TEXTURE_2D, 0, nComponents, oTga.GetWidth(), oTga.GetHeight(), 0, nFormat, GL_UNSIGNED_BYTE, oTga.GetBuffer());

	m_handles[m_numHandles++] = HandleHolder(HANDLE_TYPE_TEXTURE, filename, nTexHandle);

	return nTexHandle;
}

bool Graphics::Find(unsigned int &handleOut, char* name, char* secondName)
{
	for(unsigned int i = 0; i < m_numHandles; ++i)
	{
		if(strcmp(m_handles[i].filename, name) == 0)
		{
			if(secondName == 0 || strcmp(m_handles[i].secondaryFilename, secondName) == 0)
			{
				handleOut = m_handles[i].handle;
				return true;
			}
		}
	}

	return false;
}

void Graphics::SetTexture(unsigned int nTexHandle)
{
	SetSampler("texture", 0, nTexHandle);
	m_currTexture = nTexHandle;

	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, nTexHandle);
}

unsigned int Graphics::LoadMesh(char* filename)
{
	unsigned int meshHandle;
	if(Find(meshHandle, filename))
	{
		return meshHandle;
	}

	meshHandle = m_meshes.GetSize();

	CObjMesh mesh;
	mesh.LoadFromFile(filename);
	m_meshes[m_numMeshes++] = mesh;

	m_handles[m_numHandles++] = HandleHolder(HANDLE_TYPE_MESH, filename, meshHandle);

	return meshHandle;
}

void Graphics::RenderMesh(unsigned int meshHandle)
{
	m_meshes[meshHandle].Render();
}

void Graphics::SetShaderVertexArray(GLenum oProg, char *sAttribName, GLint size, GLenum type, GLsizei stride, const void *pVals)
{
	unsigned int nAttribLoc = glGetAttribLocation(oProg, sAttribName);
	glEnableVertexAttribArray(nAttribLoc);
	
	glVertexAttribPointer(nAttribLoc, size, type, false, stride, pVals);
}

void Graphics::SetShaderArray(char* name, int size, unsigned int type, int stride, const void *pValues)
{
	SetShaderVertexArray(m_currShaderProgram, name, size, type, stride, pValues);
}

void Graphics::SetVertexPositions(const void *pPositions)
{
	SetShaderVertexArray(m_currShaderProgram, "a_position", 3, GL_FLOAT, sizeof(Vector3), pPositions);
}

void Graphics::SetVertexNormals(const void *pNormals)
{
	SetShaderVertexArray(m_currShaderProgram, "a_normal", 3, GL_FLOAT, sizeof(Vector3), pNormals);
}

void Graphics::SetVertexTexCoords(const void *pTexCoords)
{
	SetShaderVertexArray(m_currShaderProgram, "a_uv", 2, GL_FLOAT, sizeof(Vector2), pTexCoords);
}


void Graphics::DrawDebugCube(float size, Vector3 pos, Vector4 color)
{
	Matrix scaleMat = Matrix::CreateScaleUniform(size);
	Matrix posMat = Matrix::CreateTranslation(pos);
	DrawDebugCube(posMat * scaleMat, color);
}

void Graphics::DrawDebugCube(Vector3 size, Vector3 pos, Vector4 color)
{
	Matrix scaleMat = Matrix::CreateScale(size.x, size.y, size.z);
	Matrix posMat = Matrix::CreateTranslation(pos);
	DrawDebugCube(posMat * scaleMat, color);
}

void Graphics::DrawDebugCube(Matrix mat, Vector4 color)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_CONSTANT_ALPHA_EXT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SetDefaultShaderProgram(eShader_Default);
	SetValue("color", color.ToVector3());
	SetValue("alpha", color.w);
	glLoadMatrixf( (g_viewMat * mat).GetBuffer() );
	m_debugCubeMesh.Render();
}

void Graphics::DrawSquare_imp(Vector2 center, Vector2 size, int layer, float rotation)
{
	Vector2 topLeft, topRight, bottomLeft, bottomRight;
	
	topLeft.Set(center.x - size.x / 2.0f, center.y - size.y / 2.0f);
	topRight.Set(topLeft.x + size.x, topLeft.y);
	bottomLeft.Set(topLeft.x, topLeft.y + size.y);
	bottomRight.Set(topLeft.x + size.x, topLeft.y + size.y);

	if(rotation != 0.0f)
	{
		Matrix rot = Matrix::CreateRotZ(rotation);

		topLeft = rot.Transform(topLeft);
		topRight = rot.Transform(topRight);
		bottomLeft = rot.Transform(bottomLeft);
		bottomRight = rot.Transform(bottomRight);
	}

	glBegin(GL_QUADS);
	
	glVertex3f(topLeft.x, topLeft.y, layer);
	glTexCoord2f(0,0);
	glVertex3f(bottomLeft.x, bottomLeft.y, layer);
	glTexCoord2f(0,1);
	glVertex3f(bottomRight.x, bottomRight.y, layer);
	glTexCoord2f(1,1);
	glVertex3f(topRight.x, topRight.y, layer);
	glTexCoord2f(1,0);

	glEnd();
}

void Graphics::DrawSquareWithColor(Vector2 center, Vector2 size, int layer, Vector4 color)
{
	glColor4f(color.x,color.y,color.z,color.w);
	SetValue("color", color);

	DrawSquare_imp(center,size,layer);
}

void Graphics::DrawSquareWithColor(Vector2 center, Vector2 size, int layer, Vector4 color, float rotation)
{
	glColor3f(color.x,color.y,color.z);
	SetValue("color", color.ToVector3());
	SetValue("alpha", color.w);

	DrawSquare_imp(center, size, layer, rotation);
}

void Graphics::DrawSquareWithTexture(Vector2 center, Vector2 size, int layer, unsigned int textureHandle)
{
	SetTexture(textureHandle);
	DrawSquare_imp(center,size,layer);
}

void Graphics::DrawSquareWithTexture(Vector2 center, Vector2 size, int layer, unsigned int textureHandle, float rotation)
{
	SetTexture(textureHandle);
	DrawSquare_imp(center,size,layer,rotation);
}

void Graphics::DrawRect_imp(Rect rect, int layer, float rotation)
{
	Vector2 topLeft, topRight, bottomLeft, bottomRight;
	
	rect.TopLeft(&topLeft);
	rect.TopRight(&topRight);
	rect.BottomLeft(&bottomLeft);
	rect.BottomRight(&bottomRight);

	if(rotation != 0.0f)
	{
		Matrix rot = Matrix::CreateRotZ(rotation);

		topLeft = rot.Transform(topLeft);
		topRight = rot.Transform(topRight);
		bottomLeft = rot.Transform(bottomLeft);
		bottomRight = rot.Transform(bottomRight);
	}

	glBegin(GL_QUADS);
	
	glVertex3f(topLeft.x, topLeft.y, layer);
	glTexCoord2f(0,0);
	glVertex3f(bottomLeft.x, bottomLeft.y, layer);
	glTexCoord2f(0,1);
	glVertex3f(bottomRight.x, bottomRight.y, layer);
	glTexCoord2f(1,1);
	glVertex3f(topRight.x, topRight.y, layer);
	glTexCoord2f(1,0);

	glEnd();
}

void Graphics::DrawRectWithColor(Rect rect, int layer, Vector4 color)
{
	SetDefaultShaderProgram(eShader_Color);
	glColor4f(color.x,color.y,color.z,color.w);
	SetValue("color", color);

	DrawRect_imp(rect, layer);
}

void Graphics::DrawRectWithTexture(Rect rect, int layer, unsigned int textureHandle)
{
	SetDefaultShaderProgram(eShader_Texture);
	SetTexture(textureHandle);

	DrawRect_imp(rect, layer);
}

/*
void Graphics::DrawLine(Vector3 p1, Vector3 p2, Vector4 color)
{
	float dist = (p1 - p2).Length();
	Matrix scaleMat = Matrix::CreateScale(0.01,0.01,dist);
	Matrix lookAt = Matrix::CreateViewMatrix(p1, p2);
	DrawDebugCube(scaleMat * lookAt, color);
}
*/

/*
void Graphics::SetSpotLight(LightSource_SpotLight light, char number)
{
	if(number == 1)
	{
		if(m_bSpotLight1)
		{
			SetValue("spotLightPosition1", light.GetPos());
			SetValue("spotLightIntensity1", light.m_lightIntensity);
			SetValue("spotLightDirection1", light.m_direction);
			SetValue("spotLightPhi1", light.m_phi);
			SetValue("spotLightTheta1", light.m_theta);
		}
		else
		{
			SetValue("spotLightPosition1", Vector3::zero);
			SetValue("spotLightIntensity1", Vector3::zero);
			SetValue("spotLightDirection1", Vector3::zero);
			SetValue("spotLightPhi1", 0);
			SetValue("spotLightTheta1", 0);
		}
	}
	else if(number == 2)
	{
		if(m_bSpotLight2)
		{
			SetValue("spotLightPosition2", light.GetPos());
			SetValue("spotLightIntensity2", light.m_lightIntensity);
			SetValue("spotLightDirection2", light.m_direction);
			SetValue("spotLightPhi2", light.m_phi);
			SetValue("spotLightTheta2", light.m_theta);
		}
		else
		{
			SetValue("spotLightPosition2", Vector3::zero);
			SetValue("spotLightIntensity2", Vector3::zero);
			SetValue("spotLightDirection2", Vector3::zero);
			SetValue("spotLightPhi2", 0);
			SetValue("spotLightTheta2", 0);
		}
	}
	else if(number == 3)
	{
		if(m_bSpotLight3)
		{
			SetValue("spotLightPosition3", light.GetPos());
			SetValue("spotLightIntensity3", light.m_lightIntensity);
			SetValue("spotLightDirection3", light.m_direction);
			SetValue("spotLightPhi3", light.m_phi);
			SetValue("spotLightTheta3", light.m_theta);
		}
		else
		{
			SetValue("spotLightPosition3", Vector3::zero);
			SetValue("spotLightIntensity3", Vector3::zero);
			SetValue("spotLightDirection3", Vector3::zero);
			SetValue("spotLightPhi3", 0);
			SetValue("spotLightTheta3", 0);
		}
	}
	else if(number == 4)
	{
		if(m_bSpotLight4)
		{
			SetValue("spotLightPosition4", light.GetPos());
			SetValue("spotLightIntensity4", light.m_lightIntensity);
			SetValue("spotLightDirection4", light.m_direction);
			SetValue("spotLightPhi4", light.m_phi);
			SetValue("spotLightTheta4", light.m_theta);
		}
		else
		{
			SetValue("spotLightPosition4", Vector3::zero);
			SetValue("spotLightIntensity4", Vector3::zero);
			SetValue("spotLightDirection4", Vector3::zero);
			SetValue("spotLightPhi4", 0);
			SetValue("spotLightTheta4", 0);
		}
	}
}
*/