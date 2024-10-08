#include "cGameSettings.h"



cGameSettings::cGameSettings()
{
}

cGameSettings::~cGameSettings()
{
}

bool cGameSettings::LoadConfigFile(std::string sFile)
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	// Load game settings file
	int r = luaL_loadfile(L, sFile.c_str());
	if (r != LUA_OK)
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << errormsg << std::endl;
		return false;
	}

	// Execute it
	int i = lua_pcall(L, 0, LUA_MULTRET, 0);
	if (i != LUA_OK)
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << errormsg << std::endl;
		return false;
	}	

	lua_getglobal(L, "PixelWidth");
	if (lua_isinteger(L, -1)) cGameSettings::nPixelWidth = (int)lua_tointeger(L, -1);

	lua_getglobal(L, "PixelHeight");
	if (lua_isinteger(L, -1)) cGameSettings::nPixelHeight = (int)lua_tointeger(L, -1);

	lua_getglobal(L, "ScreenWidth");
	if (lua_isinteger(L, -1)) cGameSettings::nScreenWidth = (int)lua_tointeger(L, -1);

	lua_getglobal(L, "ScreenHeight");
	if (lua_isinteger(L, -1)) cGameSettings::nScreenHeight = (int)lua_tointeger(L, -1);

	

	lua_getglobal(L, "FullScreen");
	if (lua_isboolean(L, -1)) cGameSettings::bFullScreen = lua_toboolean(L, -1);


	//// Load System Texture files
	
	

	return true;
}

int cGameSettings::nScreenWidth = 495;
int cGameSettings::nScreenHeight = 256;
int cGameSettings::nPixelWidth = 2;
int cGameSettings::nPixelHeight =2;
bool cGameSettings::bFullScreen = true;

