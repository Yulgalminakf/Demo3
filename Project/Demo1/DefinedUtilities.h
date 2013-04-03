

#pragma once

#define GETTER_SETTER(dataType, varName, funcAdditive) \
	inline dataType Get##funcAdditive ()		{ return varName; } \
	inline void Set##funcAdditive(const dataType &var)	{ varName = var; }
	//dataType *Get##funcAdditive##Ptr()	{ return &varName; } \
	//dataType Get##funcAdditive () const { return varName; } \

#define STATIC_INSTANCE(dataType, varName) \
	inline static dataType *Get()	{ assert(varName != NULL); return varName; } \
	inline static void InitStaticInstance()		{ varName = new dataType(); } \
	inline static void DeleteStaticInstance()	{ if(varName != NULL) {delete varName;} }

#define ADD_SUBTRACT(dataType, varName, funcAdditive) \
	inline void Add##funcAdditive(const dataType &amount)	{ varName += amount; } \
	inline void Subtract##funcAdditive(const dataType &amount)   { varName -= amount; }
