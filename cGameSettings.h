#pragma once

#include <iostream>
#include <string>
#include <vector>

extern "C"
{
#include "lua533/include/lua.h"
#include "lua533/include/lauxlib.h"
#include "lua533/include/lualib.h"
}

#ifdef _WIN64
	#pragma comment(lib, "lua533/lua54.lib")
#endif

/*
	This is a singleton that stores all the games configuration settings.
	These settings are loaded on game start up and are to be considered
	read-only.
*/


class cGameSettings
{
public:
	cGameSettings();
	~cGameSettings();

public:
	bool LoadConfigFile(std::string sFile);

public:
	static int nScreenWidth;
	static int nScreenHeight;
	static int nPixelWidth;
	static int nPixelHeight;
	static bool bFullScreen;

	static int nDefaultMapWidth;
	static int nDefaultMapHeight;
	

};

