
#include "Utility.h"

#pragma once
#ifndef MAIN_H
#define MAIN_H
void Print(char* format, ...);

#define MAX_NAME_SIZE 256

const int g_screenWidth = 1024;
const int g_screenHeight = 800;

extern const Vector2 g_screenSize;

extern Matrix g_meshMatrix;

extern HANDLE g_consoleHandle;

extern Vector3 g_oCameraPos;
extern Vector3 g_defaultCameraPos;
extern Vector3 g_oLookAt;
extern Vector3 g_defaultLookAt;
extern Matrix g_viewMat;
extern Matrix g_objectMat;

extern double g_deltaTime;
extern unsigned int g_frameNumber;

#define TWEAK_MENU 1

#if TWEAK_MENU
	#include "AntTweakBar.h"
	TwBar *TwkBar();
#endif //TWEAK_MENU

class Profiler;
extern Profiler* g_updateProfiler;
extern Profiler* g_drawProfiler;

#endif //MAIN_H