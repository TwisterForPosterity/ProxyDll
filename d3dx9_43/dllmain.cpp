#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NON_CONFORMING_SWPRINTFS
#include <codecvt>
#include <string>
#include <thread>

#include "d3dx9_43.hpp"
#include "Console.hpp"
#include "Utils.hpp"

BOOL InitInstance(HINSTANCE hModule)
{
	if (!GetProxy().Setup())
		return false;
	Console::Init("Proxied " + GetProxy().Name + "_org.dll");
	PluginManager.Run(".\\bin\\plugins.ini");

	return true;
}

BOOL ExitInstance()
{
	PluginManager.FreeAllPlugins();
	FreeLibrary(GetProxy().Dll);
	return true;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hModule, DWORD Reason, LPVOID Misc)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH: return InitInstance(hModule);
	case DLL_PROCESS_DETACH: return ExitInstance();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		return true;
	}
}
