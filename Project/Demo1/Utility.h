#ifndef UTILITY
#define UTILITY
#include <windows.h>
#include "MathLibrary.h"

#include "main.h"

#define PROFILER 1
#define TWK_PROFILER PROFILER && TWEAK_MENU
#if TWK_PROFILER
#	include "AntTweakBar.h"
	class Profiler;

	TwBar *TwkBarProfiler();
	void InitTwkBarProfiler();
	void SetTwkBarProfilerDeltaTimeVariable(double *dt);
	void CleanUpTwkBarProfiler();
#endif //TWK_PROFILER

namespace Utility
{
	static const t_int k_accuracy = 10000;
	t_float RandomDouble(const bool canBeNegative = false);
}

unsigned int StrHash(const char* str);

#pragma region Input

enum buttons
{
	BUTTON_0,
	BUTTON_1,
	BUTTON_2,
	BUTTON_3,
	BUTTON_4,
	BUTTON_5,
	BUTTON_6,
	BUTTON_7,
	BUTTON_8,
	BUTTON_9,
	BUTTON_A,
	BUTTON_B,
	BUTTON_C,
	BUTTON_D,
	BUTTON_E,
	BUTTON_F,
	BUTTON_G,
	BUTTON_H,
	BUTTON_I,
	BUTTON_J,
	BUTTON_K,
	BUTTON_L,
	BUTTON_M,
	BUTTON_N,
	BUTTON_O,
	BUTTON_P,
	BUTTON_Q,
	BUTTON_R,
	BUTTON_S,
	BUTTON_T,
	BUTTON_U,
	BUTTON_V,
	BUTTON_W,
	BUTTON_X,
	BUTTON_Y,
	BUTTON_Z,
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_MOUSE_LEFT,
	BUTTON_PLUS,
	BUTTON_MINUS,
	BUTTON_BRACKET_LEFT,
	BUTTON_BRACKET_RIGHT,
	BUTTON_SPACE,
	BUTTON_SHIFT,
	BUTTON_LSHIFT,
	BUTTON_RSHIFT,
	BUTTON_CONTROL,
	BUTTON_LCONTROL,
	BUTTON_RCONTROL,
	BUTTON_COMMA,
	BUTTON_PERIOD,
	BUTTON_TILDE,


	BUTTON_NONE,
	BUTTON_MAX = BUTTON_NONE
};

class Input
{
	static Input* s_theInput;

	bool m_buttons[BUTTON_MAX];
	bool m_buttonsLast[BUTTON_MAX];

	Vector2 m_mousePos;

public:
	Input(void);
	~Input(void);

	bool IsKeyDown(buttons b);
	bool IsKeyTapped(buttons b);
	void KeyDown(WPARAM key);
	void KeyUp(WPARAM key);
	buttons ConvertWPARAMbuttons(WPARAM key);
	void ResetButtons();
	void Update();
	void MouseLeftUp();
	void MouseLeftDown();
	void MouseMove(Vector2 pos);
	Vector2 GetMousePos()				{return m_mousePos; }
	void MouseWheelRolled();

	static Input* Get();
};

#pragma endregion

#pragma region RealTimer

// This timer is not based on game time, but on real computer time
class RealTimer
{
	__int64 m_i64Frequency;

	// Calculate the timer resolution using the timer frequency
	double m_fResolution;

	// Will need to multiply by this scale to get msec 
	// The smaller this is, the greater the resolution of the clock
	double m_fScale;// * 1000.0f;	

	double m_initialTime;	
public:
	RealTimer(void);
	~RealTimer(void);

	void ResetTimer();
	double GetCurrTime();
};

#pragma endregion

#if PROFILER
#pragma region Profiler
class Profiler
{
	RealTimer m_timer;
	double m_time;
	bool m_bRunning;
	const static unsigned int k_nameLength = 128;
	char m_name[k_nameLength];

	#if TWK_PROFILER
		bool m_bInTwkBar;
		void AddToTwkBar();
		void RemoveFromTwkBar();
	#endif //TWK_PROFILER

public:
	Profiler();
	Profiler(const char* name);
	~Profiler();

	void Start()
	{
		if(m_bRunning == false)
		{
			m_bRunning = true;
			m_timer.ResetTimer();
		}
	}

	void Stop()
	{
		if(m_bRunning == true)
		{
			m_bRunning = false;
			m_time += m_timer.GetCurrTime();
		}
	}

	void Reset()
	{
		m_time = 0.0;
		m_bRunning = false;
	}

	void ResetAndStart();

	void SetName(const char *name);

	inline double GetTime()	{ return m_time; }
};
#pragma endregion
#endif //PROFILER

#pragma region MinMax

class MinMax
{
	t_float m_min,m_max,m_accuracy;

	void CalcAccuracy();

public:
	MinMax(void);
	MinMax(t_float min, t_float max);
	~MinMax(void);

	void SetMin(t_float min);
	void SetMax(t_float max);
	t_float GetRandom() const;

	static t_float GetRandom(t_int min, t_int max, t_float accuracy = 1.0f);
};

#pragma endregion

////////////////////////////////////////////////
//does not have a .cpp entry
#pragma region Scalar

template <class T>
class Scalar
{
private:

	T m_currScale,m_rate;
	bool m_bClamp, m_bReachedCeiling, m_bReachedFloor;
	T m_min,m_max;

public:

	Scalar() : m_bClamp(false), m_bReachedFloor(false), m_bReachedCeiling(false) {}
	Scalar(T currScale, T rate) : m_currScale(currScale), m_rate(rate), m_bClamp(false), m_bReachedFloor(false), m_bReachedCeiling(false) {}
	Scalar(T currScale, T rate, T min, T max) : m_currScale(currScale), m_rate(rate), m_bClamp(true), m_min(min), m_max(max), m_bReachedFloor(false), m_bReachedCeiling(false) {}
	~Scalar() {}

	void Update(t_float dt)						{m_currScale += m_rate * dt; if(m_bClamp) {if(m_currScale >= m_max) {m_currScale = m_max; m_bReachedCeiling = true; } else if(m_currScale <= m_min) {m_currScale = m_min; m_bReachedFloor = true; }}}
	void SetRate(T rate)						{m_rate = rate; }
	void SetCurrScale(T currScale)				{m_currScale = currScale; }
	T GetRate()									{return m_rate; }
	T GetCurrScale()							{return m_currScale; }
	void SetMinMax(T min, T max)				{m_min = min; m_max = max; m_bClamp = true; }
	void UnClamp()								{m_bClamp = false; }
	void Clamp()								{m_bClamp = true; }
	bool ReachedCeiling()						{return m_bReachedCeiling; }
	bool ReachedFloor()							{return m_bReachedFloor; }
	void Reset()								{m_currScale = 0.0f; m_bReachedCeiling = false; m_bReachedFloor = false; }
};

#pragma endregion

#pragma region Timer

class Timer
{
	//the current amount of time on it (counting up
	t_float m_time;

	//the amount of time until it's done
	t_float m_timer;

public:
	Timer(t_float timer) : m_time(0.0f), m_timer(timer) {}
	Timer() : m_time(0.0f),m_timer(0.0f){}
	~Timer(void) {};

	void Update(t_float dt)				{m_timer += dt; }
	void Reset()						{m_time = 0; }
	void SetTimer(t_float timer)			{m_timer = timer; }
	bool IsDone()						{return m_time >= m_timer; }
	t_float GetTimeLeft()					{return m_timer - m_time; }
};

#pragma endregion
////////////////////////////////////////////////

#endif