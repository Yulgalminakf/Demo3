#include "main.h"
#include <assert.h>
#include <windows.h>
#include <stdio.h>
#include "ObjMesh.h"
#include "glew.h"
#include "Graphics.h"
#include "Array.h"
#include "Collision.h"
#include <time.h>
//#include "MeshSplitting.h"
#include "Tga.h"
#include "Color.h"

Matrix g_meshMatrix;

HANDLE g_consoleHandle;

Vector3 g_oCameraPos;
Vector3 g_defaultCameraPos = Vector3(0,0,-100);
Vector3 g_oLookAt;
Vector3 g_defaultLookAt = Vector3(0,0,0);
Matrix g_viewMat;
Matrix g_objectMat = Matrix::CreateIdentity();
const Vector2 g_screenSize(g_screenWidth, g_screenHeight);

#define PROJECTION_VIEW_MATRIX 1
#define ORTHOGRAPHIC_VIEW_MATRIX !PROJECTION_VIEW_MATRIX
#define MOVEMENT 1

//////////////////////////////////////////////////////////////////////////////////
// Instancing Test
#define NUM_OBJECTS 10000
Matrix g_matrices[NUM_OBJECTS];
Vector3 g_colors[NUM_OBJECTS];
unsigned int g_instanceTestShaderHandle = 0;
//////////////////////////////////////////////////////////////////////////////////

double g_deltaTime = 0.0;

#if TWEAK_MENU
TwBar *myBar;
TwBar *TwkBar()
{
	return myBar;
}
#endif // TWEAK_MENU

CObjMesh g_mesh;

Profiler *g_updateProfiler;
Profiler *g_drawProfiler;

unsigned int g_duckieTexHandle = 0;
unsigned int g_shaderHandle = 0;
unsigned int g_crossHairTexHandle, g_brickTexHandle;

Vector3 ScreenToWorld(const Vector2 &pointOnScreen, const Vector3 cameraPos, const Matrix &viewMat, const float screenWidth, const float screenHeight)
{
	Vector3 point = Vector3::zero;
	float length = tan( 0.5 / ((screenWidth / screenHeight) / 2) );
	Vector3 cameraForward = viewMat.GetForward();
	point = cameraPos;
	point += cameraForward * length;
	Vector2 screenPctCoord = Vector2(pointOnScreen.x / screenWidth, pointOnScreen.y / screenHeight);

	screenPctCoord -= Vector2(0.5f,0.5f);
	screenPctCoord.y *= -1;

	point += screenPctCoord.x * 2.0f * viewMat.GetRight();
	point += screenPctCoord.y * 2.0f * viewMat.GetUp();
	return point;
}

Vector3 ScreenToWorld(Vector2 pointOnScreen)
{
	return ScreenToWorld(pointOnScreen, g_oCameraPos, g_viewMat, g_screenWidth, g_screenHeight);
}

//----------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	#if TWEAK_MENU
		int handled = TwEventWin(hWnd, message, wParam, lParam);
		if(handled)
		{
			return 0;
		}
	#endif //TWEAK_MENU

	PAINTSTRUCT ps;
	HDC hdc;

	switch ( message ) 
	{
	case WM_KEYDOWN:
		Input::Get()->KeyDown(wParam);
		if( wParam == VK_ESCAPE )
		{
			DestroyWindow( hWnd );
		}
		break;
	case WM_KEYUP:
		Input::Get()->KeyUp(wParam);
		break;
	case WM_MOUSEMOVE:
		{
			Input::Get()->MouseMove(Vector2(LOWORD(lParam),HIWORD(lParam)));
			// lParam lower 16 bits: x position
			// lParam upper 16 bits: y position
			break;
		}
	case WM_LBUTTONDOWN:
		{
			Input::Get()->MouseLeftDown();
			Input::Get()->MouseMove(Vector2(LOWORD(lParam),HIWORD(lParam)));
			// lParam lower 16 bits: x position
			// lParam upper 16 bits: y position
			break;
		}
	case WM_LBUTTONUP:
		{
			Input::Get()->MouseLeftUp();
			Input::Get()->MouseMove(Vector2(LOWORD(lParam),HIWORD(lParam)));
			// lParam lower 16 bits: x position
			// lParam upper 16 bits: y position
			break;
		}			
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}
#pragma region OpenGL Functions

//----------------------------------------------------------------------------------------------
HWND CreateOpenGLWindow(char* title, int x, int y, int width, int height, BYTE type, DWORD flags)
{
	int         pf;
	HDC         hDC;
	HWND        hWnd;
	WNDCLASS    wc;
	PIXELFORMATDESCRIPTOR pfd;
	static HINSTANCE hInstance = 0;

	// only register the window class once - use hInstance as a flag.
	if (!hInstance) 
	{
		hInstance = GetModuleHandle(NULL);
		wc.style         = CS_OWNDC;
		wc.lpfnWndProc   = (WNDPROC)WndProc;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = hInstance;
		wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName  = NULL;
		wc.lpszClassName = "OpenGL";

		if (!RegisterClass(&wc)) {
			MessageBox(NULL, "RegisterClass() failed:  "
				"Cannot register window class.", "Error", MB_OK);
			return NULL;
		}
	}

	// Calculate entire client-area rectangle by taking into account window styles
	unsigned int nStyle = WS_OVERLAPPED|WS_SYSMENU|WS_VISIBLE|WS_CAPTION|WS_MINIMIZEBOX ;

	RECT oWindowRect;
	oWindowRect.left = oWindowRect.top = 0;
	oWindowRect.right = width;
	oWindowRect.bottom = height;
	AdjustWindowRect( &oWindowRect, nStyle, false );

	unsigned int nAdjustedWidth = oWindowRect.right - oWindowRect.left;
	unsigned int nAdjustedHeight = oWindowRect.bottom - oWindowRect.top;

	hWnd = CreateWindow("OpenGL", title, nStyle,
		x, y, nAdjustedWidth, nAdjustedHeight, NULL, NULL, hInstance, NULL);
	
	if (hWnd == NULL) {
		MessageBox(NULL, "CreateWindow() failed:  Cannot create a window.",
			"Error", MB_OK);
		return NULL;
	}

	hDC = GetDC(hWnd);

	// there is no guarantee that the contents of the stack that become
	// the pfd are zeroed, therefore _make sure_ to clear these bits. 
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize        = sizeof(pfd);
	pfd.nVersion     = 1;
	pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | flags;
	pfd.iPixelType   = type;
	pfd.cColorBits   = 32;

	pf = ChoosePixelFormat(hDC, &pfd);
	if (pf == 0) {
		MessageBox(NULL, "ChoosePixelFormat() failed:  "
			"Cannot find a suitable pixel format.", "Error", MB_OK); 
		return 0;
	} 

	if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
		MessageBox(NULL, "SetPixelFormat() failed:  "
			"Cannot set format specified.", "Error", MB_OK);
		return 0;
	} 

	DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	ReleaseDC(hWnd, hDC);

	return hWnd;
}    

//----------------------------------------------------------------------------------------------
static HWND Init(HINSTANCE hInstance, const char *sName, int nCmdShow)
{
	// Create OpenGL window
	HWND hWnd = CreateOpenGLWindow("minimal", 0, 0, (int)g_screenWidth, (int)g_screenHeight, PFD_TYPE_RGBA, 0);
	assert(hWnd!=NULL);

	HDC hDC = GetDC(hWnd);
	HGLRC hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

	ShowWindow(hWnd, nCmdShow);

	// Initialize extension wrangler.  Allows us to easily use opengl extensions
	GLenum err = glewInit();
	if (err != GLEW_OK)
		return false;

	return hWnd;
}
#pragma endregion

unsigned int g_deltaTimeBufferIndex = 0;
const unsigned int g_deltaTimeBufferSize = 20;
float g_avgFPSBuffer[g_deltaTimeBufferSize];

float GetAvgDeltaTime()
{
	float dt = 0.0f;
	for(int i = 0; i < g_deltaTimeBufferSize; ++i)
	{
		dt += g_avgFPSBuffer[i];
	}
	dt /= g_deltaTimeBufferSize;
	return dt;
}

void AddToDeltaTimeBuffer(float dt)
{
	g_avgFPSBuffer[g_deltaTimeBufferIndex++ % g_deltaTimeBufferSize] = dt;
}

float g_timeToPrintFPS = 1.0f;
float g_timer = 0.0f;
//----------------------------------------------------------------------------------------------
void Update(double fDelta)
{
	Input *input = Input::Get();

	AddToDeltaTimeBuffer(fDelta);

	#if TWK_PROFILER
		TwRefreshBar(TwkBarProfiler());
	#endif //TWK_PROFILER

	g_timer += fDelta;
	if(g_timer >= g_timeToPrintFPS)
	{
		g_timer = 0.0f;
		float avgDt = GetAvgDeltaTime();

		Print("\nFrames per second: %f\n", 1.0f / avgDt);
	}
#if MOVEMENT
#pragma region Movement

	Vector3 right,up,forward;
	Matrix view = Matrix::CreateViewMatrix(g_oCameraPos,g_oLookAt);

	right = view.GetRight();
	up = view.GetUp();
	forward = view.GetForward();
	
	float movementSpeed = 50.0f;
	float turnRate = 4.5f;
	float dt = (float)fDelta;

	if(input->IsKeyDown(BUTTON_SHIFT))
	{
		movementSpeed *= 2;
	}

	if(input->IsKeyDown(BUTTON_CONTROL))
	{
		movementSpeed *= 0.5f;
	}

	if(input->IsKeyDown(BUTTON_UP))
	{
		Matrix rot = Matrix::CreateRotAxis(right,-turnRate * dt, g_oCameraPos);
		g_oLookAt = rot.Transform(g_oLookAt);
	}
	else if(input->IsKeyDown(BUTTON_DOWN))
	{
		Matrix rot = Matrix::CreateRotAxis(right,turnRate * dt, g_oCameraPos);
		g_oLookAt = rot.Transform(g_oLookAt);
	}

	if(input->IsKeyDown(BUTTON_LEFT))
	{
		Matrix rot = Matrix::CreateRotAxis(up,-turnRate * dt, g_oCameraPos);
		g_oLookAt = rot.Transform(g_oLookAt);
	}
	else if(input->IsKeyDown(BUTTON_RIGHT))
	{
		Matrix rot = Matrix::CreateRotAxis(up,turnRate * dt, g_oCameraPos);
		g_oLookAt = rot.Transform(g_oLookAt);
	}

	if(input->IsKeyDown(BUTTON_W))
	{
		g_oCameraPos += forward * dt * movementSpeed;
		g_oLookAt += forward * dt * movementSpeed;
	}
	else if(input->IsKeyDown(BUTTON_S))
	{
		g_oCameraPos -= forward * dt * movementSpeed;
		g_oLookAt -= forward * dt * movementSpeed;
	}

	if(input->IsKeyDown(BUTTON_A))
	{
		g_oCameraPos -= right * dt * movementSpeed;
		g_oLookAt -= right * dt * movementSpeed;
	}
	else if(input->IsKeyDown(BUTTON_D))
	{
		g_oCameraPos += right * dt * movementSpeed;
		g_oLookAt += right * dt * movementSpeed;
	}

	if(input->IsKeyDown(BUTTON_E))
	{
		g_oCameraPos += up * dt * movementSpeed;
		g_oLookAt += up * dt * movementSpeed;
	}
	else if(input->IsKeyDown(BUTTON_Q))
	{
		g_oCameraPos -= up * dt * movementSpeed;
		g_oLookAt -= up * dt * movementSpeed;
	}

	if(input->IsKeyDown(BUTTON_SPACE))
	{
		g_oCameraPos = g_defaultCameraPos;
		g_oLookAt = g_defaultLookAt;
	}
#pragma endregion //Movement
#endif //MOVEMENT
	Graphics::Get()->Update((float)fDelta);
	Input::Get()->Update();
}

void RenderGUI()
{
	/*
	glDisable(GL_DEPTH_TEST);

	glLoadMatrixf(Matrix::CreateTranslation(Vector3(0,0,-10)).GetBuffer());
	Graphics *graphics = Graphics::Get();
	graphics->SetShaderProgram(g_shaderHandle);
	graphics->SetValue("color", Vector3(1,1,0.5f));
	//graphics->SetValue("a_position", Vector3(0.5,0.5,-1));
	g_mesh.Render();

	glEnable(GL_DEPTH_TEST);
	*/
}

//----------------------------------------------------------------------------------------------
void RenderWorld()
{
	unsigned int error = glGetError();
	#if PROFILER
		g_updateProfiler->Stop();
		g_drawProfiler->ResetAndStart();
	#endif //PROFILER
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_CONSTANT_ALPHA_EXT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_LESS, 1.0f);
	Graphics* graphics = Graphics::Get();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	//graphics->SetShaderProgram(g_oProgram);

	Matrix oView = Matrix::CreateViewMatrix(g_oCameraPos, g_oLookAt);
	g_viewMat = oView;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(oView.GetBuffer());

	//Graphics::Get()->SetTexture(g_duckieTexHandle);
	graphics->SetShaderProgram(g_instanceTestShaderHandle);

	unsigned int matricesHandle = glGetAttribLocation(g_instanceTestShaderHandle, "matrix");	
	error = glGetError();
    for (unsigned int i = 0; i < 4 ; i++) 
	{
        glEnableVertexAttribArray(matricesHandle + i);
		error = glGetError();
		char* ptr = ((char*)g_matrices) + (i * sizeof(float) * 4);
        glVertexAttribPointer(matricesHandle + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), ptr);
		error = glGetError();
		glVertexAttribDivisorARB(matricesHandle + i, 1);
		error = glGetError();
        //glVertexAttribDivisor(matricesHandle + i, 1);
    }

	unsigned int colorsHandle = glGetAttribLocation(g_instanceTestShaderHandle, "color");
	glEnableVertexAttribArray(colorsHandle);
	glVertexAttribPointer(colorsHandle, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), g_colors);
	glVertexAttribDivisorARB(colorsHandle, 1);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	//unsigned int colorHandle = glGetUniformLocation(g_instanceTestShaderHandle, "color");
	//glUniform3f(colorHandle, 1,1,1);
	glVertexPointer(3, GL_FLOAT, sizeof(Vector3), g_mesh.GetVertexPointer());
	glDrawElementsInstanced(GL_TRIANGLES, g_mesh.GetIndicesSize(), GL_UNSIGNED_INT, g_mesh.GetIndicesPointer(), NUM_OBJECTS);

	/*
	graphics->SetValue("color", Vector3(1,0,1));
	graphics->SetValue("alpha", 0.5);
	glLoadMatrixf( (oView * g_meshMatrix).GetBuffer() );
	g_mesh.Render(true, GL_LINE);
	*/

	//graphics->DrawSquareWithTexture(Vector2(0,0),Vector2(10,10),0,g_brickTexHandle, g_rot);
	//graphics->DrawSquareWithColor(Vector2(0,0),Vector2(10,10),0,Vector4(1,1,1,1), 0);

	//g_boid.Draw();
	/*
	Vector2 mousePos = Input::Get()->GetMousePos();
	mousePos = g_screenSize - mousePos;
	//mousePos -= g_screenSize;
	unsigned int x,y;
	Flocking::ConvertPosToXY(mousePos,x,y);
	Flocking *flocking = Flocking::Get();

	int width = 3, height = 3;
	for(int i = x - width, iMax = x + width; i <= iMax; ++i)
	{
		for(int j = y - height, jMax = y + height; j <= jMax; ++j)
		{
			int wrappedI = GeneralMath::WrapInt(i, 0, Flocking::k_numBlocksWidth - 1);
			int wrappedJ = GeneralMath::WrapInt(j, 0, Flocking::k_numBlocksHeight - 1);
			unsigned int index;

			Flocking::ConvertXYToIndex(wrappedI,wrappedJ,index);
			if(wrappedI == x && wrappedJ == y)
			{
				flocking->DrawBlock(index, Vector4(0.75f,0.75f,0.75f,1.0f));
			}
			else
			{
				flocking->DrawBlock(index, Vector4(0.0f,0.5f,0.0f,1.0f));
			}
		}
	}*/

	RenderGUI();
	//g_angle += 0.01f;
	#if PROFILER
		g_drawProfiler->Stop();
	#endif //PROFILER
	

#if TWEAK_MENU
	TwDraw();
#endif // TWEAK_MENU
}

void Print(char* format, ...)
{
	const int MAX_WORD_LENGTH = 2048;
	static char buffer[MAX_WORD_LENGTH];
	buffer[MAX_WORD_LENGTH - 2] = '\n';

	va_list ap;

	va_start(ap, format);

	vsprintf(buffer, format, ap);


	DWORD cCharsWritten;
	//fprintf(stdout,format,...);
	WriteConsole(g_consoleHandle, buffer, strlen(buffer), &cCharsWritten, NULL);
	//delete[] buffer;
	va_end(ap); /* Cleanup the va_list */
}

void Init()
{
//GLAPI void GLAPIENTRY glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
	//glDrawElementsInstanced(
	//glUniform1fv(
		// main TweakBar initialization

	for(int i = 0; i < NUM_OBJECTS; ++i)
	{
		float x,y,z;
		const static unsigned int maxPos = 1000;
		x = rand() % maxPos;
		y = rand() % maxPos;
		z = rand() % maxPos;
		x -= maxPos / 2.0f;
		y -= maxPos / 2.0f;
		z -= maxPos / 2.0f;
		//x = 0.0f;
		//y = 0.0f;
		const static float minSize = 0.55f;
		const static float maxSize = 2.5f;
		float size = (minSize == maxSize) ? minSize : (Utility::RandomDouble(false) * (maxSize - minSize)) + minSize;
		float xRot = 0.0f, yRot = 0.0f, zRot = 0.0f;
		xRot = rand() % 360;
		yRot = rand() % 360;
		zRot = rand() % 360;
		g_matrices[i] = Matrix::CreateTranslation(Vector3(x, y, z)) * Matrix::CreateScaleUniform(size) * Matrix::CreateRotXYZ(xRot, yRot, zRot);

		g_colors[i] = Color::GetColor((Color::CommonColor)(rand() % Color::CommonColor::Count));
		/*
		for(int j = 0; j < 3; ++j)
		{
			for(int k = 0; k < 3; ++k)
			{
				g_matrices[i].Set(j,k,Utility::RandomDouble(true) * 1.0f);
			}
		}*/
	}

	g_instanceTestShaderHandle = Graphics::Get()->LoadShaderProgram("Shaders/instance_test_vs.glsl", "Shaders/instance_test_ps.glsl");
	#if TWEAK_MENU
		TwInit(TW_OPENGL, NULL);
		TwWindowSize(g_screenWidth, g_screenHeight);
		myBar = TwNewBar("Tweak");
	#endif // TWEAK_MENU

	#if TWK_PROFILER
		InitTwkBarProfiler();
		SetTwkBarProfilerDeltaTimeVariable(&g_deltaTime);
	#endif //TWEAK_MENU && PROFILER

	#if PROFILER
		g_updateProfiler = new Profiler();
		g_updateProfiler->SetName("Entire Update");
		g_drawProfiler = new Profiler();
		g_drawProfiler->SetName("Entire Draw");
//Profiler g_updateProfiler;
//Profiler g_drawProfiler;
	#endif //PROFILER

	// test TweakBar initialization
	#if TWEAK_MENU
		//TwAddVarRW(TwkBar(), "Seperation Coefficient", TW_TYPE_FLOAT, &Boid::s_seperationCoefficient, "min=-10 max=10");
	//	TwAddVarRW(TwkBar(), "Flocking", TW_TYPE_BOOL8, &Boid::s_bFlockingCalculations, "");
	#endif //TWEAK_MENU

	Graphics *graphics = Graphics::Get();
	g_brickTexHandle = graphics->LoadTexture("Textures/TilingBrickTexture.tga");
	//g_crossHairTexHandle = graphics->LoadTexture("Textures/Corrshairs.tga");

	//g_shaderHandle = graphics->LoadShaderProgram("Shaders\\texture_vs.glsl", "Shaders\\texture_ps.glsl");
	g_shaderHandle = graphics->LoadShaderProgram("Shaders\\color_vs.glsl", "Shaders\\color_ps.glsl");
	//g_shaderHandle = graphics->LoadShaderProgram("Shaders\\default_vs.glsl", "Shaders\\default_ps.glsl");
	
	g_oCameraPos = g_defaultCameraPos;
	g_oLookAt = g_defaultLookAt;

	g_mesh.LoadFromFile("Meshes/cube.obj");

	#if TWEAK_MENU
	//	Boid *boid = Flocking::Get()->GetBoidAt(0);
	//	TwAddVarRW(TwkBar(), "Draw Neighbors", TW_TYPE_BOOL8, &boid->m_bDrawNeighbors, "");
	#endif //TWEAK_MENU
	//g_mesh.LoadFromFile("suzanne.obj");
	//g_mesh.LoadFromFile("bunny.obj");

	for(int i = 0; i < g_deltaTimeBufferSize; ++i)
	{
		g_avgFPSBuffer[i] = 0.0f;
	}
}

void CleanUp()
{
#if TWEAK_MENU
	TwTerminate();
#endif // TWEAK_MENU

	delete Graphics::Get();
	delete Input::Get();
}

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPTSTR    lpCmdLine,
					 int       nCmdShow)
{
//	srand(time(0));

 	AllocConsole();
	g_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND hWnd = Init(hInstance, "Test", nCmdShow);

	HDC hDC = GetDC(hWnd);
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	
	{
		Init();

		//----------------------------------------------------------------------------------------------------------------------
		

		__int64 m_i64Frequency;
		BOOL bPerformaceTimer = QueryPerformanceFrequency(( LARGE_INTEGER * ) &m_i64Frequency );

		if(bPerformaceTimer == 1) {}

		// Calculate the timer resolution using the timer frequency
		double m_fResolution = ( ((double)1.0f) / ((double)m_i64Frequency) );

		// Will need to multiply by this scale to get msec 
		// The smaller this is, the greater the resolution of the clock
		double m_fScale = m_fResolution;

		__int64 i64Time;
		QueryPerformanceCounter((LARGE_INTEGER *) &i64Time);	
		double fPrevTime = (double)i64Time * m_fScale;

		double timePassed = 0.0;
//		double timeToStop = 10.0;

		while( msg.message!=WM_QUIT )
		{
			if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				i64Time;
				QueryPerformanceCounter((LARGE_INTEGER *) &i64Time);	

				double fTime =  (double)i64Time * m_fScale; //timeGetTime();
				double fDuration = (fTime) - (fPrevTime);
				fPrevTime = fTime;

				g_deltaTime = fDuration;

				timePassed += fDuration;

				// Clear the screen
				glClearColor(0, 0, 0, 1);	// Values ranges from 0-1 
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();

#if PROJECTION_VIEW_MATRIX
				// for projection matrix
				glMatrixMode(GL_PROJECTION);
				Matrix oProjMat = Matrix::CreateProjection(3.1415f/2.0f, g_screenHeight / g_screenHeight, 1.0f, 3000.0f);
				glLoadMatrixf(oProjMat.GetBuffer());
#elif ORTHOGRAPHIC_VIEW_MATRIX
				// for orothographic matrix
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				//glOrtho(-100,100,-100,100,1,1000);
				glOrtho(-g_screenWidth / 2.0f,g_screenWidth / 2.0f,-g_screenHeight / 2.0f,g_screenHeight / 2.0f,1,1000);
#endif

				#if PROFILER
					g_updateProfiler->ResetAndStart();
				#endif //PROFILER
				Update(fDuration);
				#if PROFILER
					g_updateProfiler->Stop();
					g_drawProfiler->ResetAndStart();
				#endif //PROFILER
				RenderWorld();
				#if PROFILER
					g_drawProfiler->Stop();
				#endif //PROFILER
				//--------------------------------------------------------------------------------------------

				// Flush the screen
				glFlush();
				SwapBuffers(hDC);
				glFinish();
			}
		}

		HGLRC hRC = wglGetCurrentContext();
		wglMakeCurrent(NULL, NULL);
		ReleaseDC(hWnd, hDC);
		wglDeleteContext(hRC);
		DestroyWindow(hWnd);

		CleanUp();
	}

	return 0;
}