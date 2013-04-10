

#pragma once

#include <assert.h>

#define GETSET(dataType, varName, funcAdditive) \
	inline dataType Get##funcAdditive ()		{ return varName; } \
	inline void Set##funcAdditive(dataType &var)	{ varName = var; }
	//dataType *Get##funcAdditive##Ptr()	{ return &varName; } \
	//dataType Get##funcAdditive () const { return varName; } \

#define STATIC_INSTANCE(dataType, varName) \
	inline static dataType *Get()	{ assert(varName != 0); return varName; } \
	inline static void InitStaticInstance()		{ varName = new dataType(); } \
	inline static void DeleteStaticInstance()	{ if(varName != 0) {delete varName;} }

#define ADDSUB(dataType, varName, funcAdditive) \
	inline void Add##funcAdditive(const dataType &amount)	{ varName += amount; } \
	inline void Subtract##funcAdditive(const dataType &amount)   { varName -= amount; }
