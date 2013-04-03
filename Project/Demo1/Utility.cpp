#include "Utility.h"


t_float Utility::RandomDouble(const bool canBeNegative)
{
	//number between 0 and 1000
	t_int randomInt = rand() % (k_accuracy + 1);
	//number between 0 and 1
	t_float randomFloat = (t_float)randomInt / (t_float)k_accuracy;

	//if the number can be negative, have another random roll for that
	//if not, simply return the number
	if(canBeNegative)
	{
		if(rand() % 2 == 0)
		{
			randomFloat = -randomFloat;
		}
	}

	return randomFloat;
}

#pragma region Profiler

#if PROFILER
#if TWK_PROFILER
	static TwBar *g_twkBarProfiler = NULL;
	TwBar *TwkBarProfiler()
	{
		return g_twkBarProfiler;
	}

	void InitTwkBarProfiler()
	{
		g_twkBarProfiler = TwNewBar("Profiler");
	}

	void SetTwkBarProfilerDeltaTimeVariable(double *dt)
	{
		TwAddVarRO(TwkBarProfiler(), "Delta Time", TW_TYPE_DOUBLE, dt, "precision=6");
	}
#endif //TWEAK_MENU && PROFILER
	
Profiler::Profiler()
{
	m_time = 0.0;
	m_bRunning = false;

	#if TWK_PROFILER
		m_bInTwkBar = false;
	#endif //TWK_PROFILER
}

Profiler::Profiler(const char* name)
{
	m_time = 0.0;
	m_bRunning = false;

	#if TWK_PROFILER
		m_bInTwkBar = false;
	#endif //TWK_PROFILER

	SetName(name);
}

Profiler::~Profiler()
{
	#if TWK_PROFILER
		RemoveFromTwkBar();
	#endif //TWK_PROFILER
}

#if TWK_PROFILER
	void Profiler::AddToTwkBar()
	{
		if(m_bInTwkBar)
		{
			return;
		}

		if(TwkBarProfiler() == NULL)
		{
			return;
		}

		m_bInTwkBar = true;
		//TwAddVarRW(TwkBar(), "Max Alignment Force", TW_TYPE_FLOAT, &Boid::s_maxForce, "min=0 max=500");
		//TwAddVarRO(TwkBarProfiler(), "
		TwAddVarRO(TwkBarProfiler(), m_name, TW_TYPE_DOUBLE, &m_time, "precision=6");
	}

	void Profiler::RemoveFromTwkBar()
	{
		if(!m_bInTwkBar)
		{
			return;
		}
		
		if(TwkBarProfiler() == NULL)
		{
			return;
		}

		m_bInTwkBar = false;
		TwRemoveVar(TwkBarProfiler(), m_name);
	}
#endif //TWK_PROFILER

void Profiler::SetName(const char *name)
{
	#if TWK_PROFILER
		if(m_bInTwkBar)
		{
			RemoveFromTwkBar();
		}
	#endif //TWK_PROFILER

	strcpy_s(m_name, name);

	#if TWK_PROFILER
		AddToTwkBar();
	#endif //TWK_PROFILER
}

void Profiler::ResetAndStart()
{
	Reset();
	Start();
}

#pragma endregion //Profiler

#endif //PROFILER

#pragma region Input

Input* Input::s_theInput = NULL;

Input::Input(void)
{
	ResetButtons();
}

Input::~Input(void)
{
}

Input* Input::Get()
{
	if(s_theInput == NULL)
	{
		s_theInput = new Input();
	}

	return s_theInput;
}

bool Input::IsKeyDown(buttons b)
{
	if(b != BUTTON_NONE)
	{
		return m_buttons[b];
	}
	return false;
}

bool Input::IsKeyTapped(buttons b)
{
	if(b != BUTTON_NONE)
	{
		return m_buttons[b] && !m_buttonsLast[b];
	}
	return false;
}

void Input::KeyDown(WPARAM key)
{
	buttons b = ConvertWPARAMbuttons(key);
	
	if(b != BUTTON_NONE)
	{
		m_buttons[b] = true;
	}
}

void Input::KeyUp(WPARAM key)
{
	buttons b = ConvertWPARAMbuttons(key);
	
	if(b != BUTTON_NONE)
	{
		m_buttons[b] = false;
	}
}

buttons Input::ConvertWPARAMbuttons(WPARAM key)
{
	switch(key)
	{
	case VK_UP:
		return BUTTON_UP;
	case VK_DOWN:
		return BUTTON_DOWN;
	case VK_LEFT:
		return BUTTON_LEFT;
	case VK_RIGHT:
		return BUTTON_RIGHT;
	case VK_OEM_PLUS:
		return BUTTON_PLUS;
	case VK_OEM_MINUS:
		return BUTTON_MINUS;
	case VK_OEM_4:
		return BUTTON_BRACKET_LEFT;
	case VK_OEM_6:
		return BUTTON_BRACKET_RIGHT;
	case VK_SPACE:
		return BUTTON_SPACE;
	case VK_SHIFT:
		return BUTTON_SHIFT;
	case VK_LSHIFT:
		return BUTTON_LSHIFT;
	case VK_RSHIFT:
		return BUTTON_RSHIFT;
	case VK_CONTROL:
		return BUTTON_CONTROL;
	case VK_LCONTROL:
		return BUTTON_LCONTROL;
	case VK_RCONTROL:
		return BUTTON_RCONTROL;
	case VK_OEM_COMMA:
		return BUTTON_COMMA;
	case VK_OEM_PERIOD:
		return BUTTON_PERIOD;
	case VK_OEM_3:				// '`~' for US
		return BUTTON_TILDE;

		/*
	case VK_NUMPAD0:
	case VK_NUMPAD1:
	case VK_NUMPAD2:
	case VK_NUMPAD3:
	case VK_NUMPAD4:
	case VK_NUMPAD5:
	case VK_NUMPAD6:
	case VK_NUMPAD7:
	case VK_NUMPAD8:
	case VK_NUMPAD9:
	case VK_MULTIPLY:
	case VK_ADD:
	case VK_SEPARATOR:
	case VK_SUBTRACT:
	case VK_DECIMAL:
	case VK_DIVIDE:
	case VK_F1:
	case VK_F2:
	case VK_F3:
	case VK_F4:
	case VK_F5:
	case VK_F6:
	case VK_F7:
	case VK_F8:
	case VK_F9:
	case VK_F10:
	case VK_F11:
	case VK_F12:
	case VK_F13:
	case VK_F14:
	case VK_F15:
	case VK_F16:
	case VK_F17:
	case VK_F18:
	case VK_F19:
	case VK_F20:
	case VK_F21:
	case VK_F22:
	case VK_F23:
	case VK_F24:*/
	}

	char cKey = (char)key;
	
	switch(cKey)
	{
	case 'A':
		return BUTTON_A;
	case 'B':
		return BUTTON_B;
	case 'C':
		return BUTTON_C;
	case 'D':
		return BUTTON_D;
	case 'E':
		return BUTTON_E;
	case 'F':
		return BUTTON_F;
	case 'G':
		return BUTTON_G;
	case 'H':
		return BUTTON_H;
	case 'I':
		return BUTTON_I;
	case 'J':
		return BUTTON_J;
	case 'K':
		return BUTTON_K;
	case 'L':
		return BUTTON_L;
	case 'M':
		return BUTTON_M;
	case 'N':
		return BUTTON_N;
	case 'O':
		return BUTTON_O;
	case 'P':
		return BUTTON_P;
	case 'Q':
		return BUTTON_Q;
	case 'R':
		return BUTTON_R;
	case 'S':
		return BUTTON_S;
	case 'T':
		return BUTTON_T;
	case 'U':
		return BUTTON_U;
	case 'V':
		return BUTTON_V;
	case 'W':
		return BUTTON_W;
	case 'X':
		return BUTTON_X;
	case 'Y':
		return BUTTON_Y;
	case 'Z':
		return BUTTON_Z;
	case '0':		
		return BUTTON_0;
	case '1':		
		return BUTTON_1;
	case '2':		
		return BUTTON_2;
	case '3':		
		return BUTTON_3;
	case '4':		
		return BUTTON_4;
	case '5':		
		return BUTTON_5;
	case '6':		
		return BUTTON_6;
	case '7':		
		return BUTTON_7;
	case '8':		
		return BUTTON_8;
	case '9':		
		return BUTTON_9;
	}

	return BUTTON_NONE;
}

void Input::ResetButtons()
{
	for(t_int i = 0; i < BUTTON_MAX; ++i)
	{
		m_buttons[i] = false;
	}
}

void Input::Update()
{
	for(t_int i = 0; i < BUTTON_MAX; ++i)
	{
		m_buttonsLast[i] = m_buttons[i];
	}
}

void Input::MouseLeftUp()
{
	m_buttons[BUTTON_MOUSE_LEFT] = false;
}

void Input::MouseLeftDown()
{
	m_buttons[BUTTON_MOUSE_LEFT] = true;
}

void Input::MouseMove(Vector2 pos)
{
	m_mousePos = pos;
}

void Input::MouseWheelRolled()
{
}

#pragma endregion

#pragma region RealTimer

RealTimer::RealTimer(void)
{
	QueryPerformanceFrequency(( LARGE_INTEGER * ) &m_i64Frequency );

	// Calculate the timer resolution using the timer frequency
	m_fResolution = ( ((double)1.0f) / ((double)m_i64Frequency) );

	// Will need to multiply by this scale to get msec 
	// The smaller this is, the greater the resolution of the clock
	m_fScale = m_fResolution;// * 1000.0f;

	__int64 i64Time;

	QueryPerformanceCounter((LARGE_INTEGER *) &i64Time);	

	double fTime =  (double)i64Time * m_fScale; //timeGetTime();
	m_initialTime = (fTime);
}

RealTimer::~RealTimer(void)
{
}

void RealTimer::ResetTimer()
{
	__int64 i64Time;

	QueryPerformanceCounter((LARGE_INTEGER *) &i64Time);	

	double fTime =  (double)i64Time * m_fScale; //timeGetTime();
	m_initialTime = (fTime);
}

double RealTimer::GetCurrTime()
{
	__int64 i64Time;

	QueryPerformanceCounter((LARGE_INTEGER *) &i64Time);	

	double fTime =  (double)i64Time * m_fScale; //timeGetTime();
	return fTime - m_initialTime;
}

#pragma endregion

#pragma region MinMax

MinMax::MinMax(void)
{
	m_min = 0;
	m_max = 0;
	m_accuracy = 1.0f;
}

MinMax::MinMax(t_float min, t_float max)
{
	m_min = min;
	m_max = max;
	m_accuracy = 1.0f;

	CalcAccuracy();
}

MinMax::~MinMax(void)
{
}

void MinMax::CalcAccuracy()
{
	while(m_min - (t_int)m_min != 0.0f || m_max - (t_int)m_max != 0.0f)
	{
		m_min *= 10.0f;
		m_max *= 10.0f;
		m_accuracy *= 10.0f;
	}
}

t_float MinMax::GetRandom(t_int min, t_int max, t_float accuracy)
{
	if(min == max)
	{
		return (t_float)min;
	}

	return ((rand() % (max - min)) + min) / (accuracy == 0.0f ? 1.0f : accuracy);
}

void MinMax::SetMin(t_float min)
{
	m_max /= m_accuracy;
	m_accuracy = 1.0f;
	m_min = min;

	CalcAccuracy();
}

void MinMax::SetMax(t_float max)
{
	m_min /= m_accuracy;
	m_accuracy = 1.0f;
	m_max = max;

	CalcAccuracy();
}

t_float MinMax::GetRandom() const
{
	if(m_min == m_max)
	{
		return m_min;
	}

	return ((rand() % (t_int)(m_max - m_min)) + m_min) / (m_accuracy == 0.0f ? 1.0f : m_accuracy);
}

#pragma endregion