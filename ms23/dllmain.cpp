#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NON_CONFORMING_SWPRINTFS
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <codecvt>

#include <Console.hpp>
#include <Utils.hpp>

#include "classes/camera.hpp"
#include "classes/cinematic.hpp"
#include "classes/filo.hpp"
#include "classes/game_engine.hpp"
#include "classes/game_globals.hpp"
#include "classes/game_state.hpp"
#include "classes/game_systems.hpp"
#include "classes/level.hpp"
#include "classes/map.hpp"
#include "classes/misc.hpp"
#include "classes/networking.hpp"
#include "classes/physics.hpp"
#include "classes/player_profile.hpp"
#include "classes/players.hpp"
#include "classes/preferences.hpp"
#include "classes/print.hpp"
#include "classes/simulation.hpp"
#include "classes/tags.hpp"
#include "classes/ui.hpp"
#include "classes/unit.hpp"

#include "memory/tag_groups.hpp"
#include "memory/local_types.hpp"


int AssignHotkeys(float sleep_time = 0.5)
{
	while (true)
	{
		AssignHotkey(VK_F2, &SetAllQualitySettingsLow);
		AssignHotkey(VK_F3, &SetAllQualitySettingsMedium);
		AssignHotkey(VK_F4, &SetAllQualitySettingsHigh);

		AssignHotkey(VK_F5, &update_debug_loading_type);

		Sleep((int)(sleep_time * 1000));
	}
	return 0;
}
int MainThread()
{
	ConfigManager.Init(".\\bin\\ms23.ini");
	PreferenceManager.Init(".\\bin\\preferences.ini");
	PreferenceManagerAppname = "Preference";

	g_saves_path = ConfigManager.GetWString("Saves", "Path");
	UseStandardRendering(ConfigManager.GetBool("Rendering", "UseStandardRendering"));
	g_maps_path = ConfigManager.GetString("Maps", "Path")[0] != '\0' ? ConfigManager.GetString("Maps", "Path") : g_default_maps_path;
	update_debug_loading_type();

	Blam::StringIDCache::Instance.Load(g_maps_path + "string_ids.dat");

	AddCameraHooks("Camera");
	AddCameraPatches("Camera");
	AddFiloHooks("Filo");
	AddFiloPatches("Filo");
	AddGameEngineHooks("GameEngine");
	AddGameEnginePatches("GameEngine");
	AddGameGlobalsHooks("GameGlobals");
	AddGameGlobalsPatches("GameGlobals");
	AddGameStateHooks("GameState");
	AddGameStatePatches("GameState");
	AddGameSystemsHooks("GameSystems");
	AddGameSystemsPatches("GameSystems");
	AddLevelHooks("Level");
	AddLevelPatches("Level");
	AddMapHooks("Map");
	AddMapPatches("Map");
	AddMiscHooks("Misc");
	AddMiscPatches("Misc");
	AddNetworkingHooks("Networking");
	AddNetworkingPatches("Networking");
	AddPhysicsHooks("Physics");
	AddPhysicsPatches("Physics");
	AddPlayerProfileHooks("PlayerProfile");
	AddPlayerProfilePatches("PlayerProfile");
	AddPlayersHooks("Players");
	AddPlayersPatches("Players");
	AddPreferencesHooks("Preferences");
	AddPreferencesPatches("Preferences");
	AddPrintHooks("Print");
	AddPrintPatches("Print");
	AddRendererHooks("Renderer");
	AddRendererPatches("Renderer");
	AddSimulationHooks("Simulation");
	AddSimulationPatches("Simulation");
	AddTagsHooks("Tags");
	AddTagsPatches("Tags");
	AddUiHooks("Ui");
	AddUiPatches("Ui");
	AddUnitHooks("Unit");
	AddUnitPatches("Unit");

	HookManager.ApplyHooks();
	PatchManager.ApplyPatches();

	g_use_default_system_ui_language = ConfigManager.GetBool("Language", "UseSystemDefault");
	g_new_system_ui_language = ConfigManager.GetLanguage("Language", "Selected");

	PrintGPU();

	//PrintTagGroupEnum();
	//PrintGameSystems(/*e_game_system::_levels*/);
	//g_vftables.AddEndAddress(0x069B1FFF)->Print();

	return AssignHotkeys(1.5);
}

void LaunchMainmenu()
{
	LaunchScenario("levels\\ui\\mainmenu\\mainmenu", e_scenario_type::_mainmenu);
}
void ForceLoad()
{
	auto scenario_path = ConfigManager.GetString("ForceLoad", "ScenarioPath");
	auto scenario_type = ConfigManager.GetInt("ForceLoad", "ScenarioType");
	auto game_type = ConfigManager.GetInt("ForceLoad", "GameType");
	auto team_game = ConfigManager.GetBool("ForceLoad", "TeamGame");
	auto time_limit = ConfigManager.GetInt("ForceLoad", "TimeLimit");
	auto respawn_time = ConfigManager.GetInt("ForceLoad", "RespawnTime");

	LaunchScenario(scenario_path, scenario_type, game_type, team_game, time_limit, respawn_time);
}
int ForceLoadThread()
{
	while (true)
	{
		AssignHotkey(VK_F6, &LaunchMainmenu);
		AssignHotkey(VK_F7, &ForceLoad);
		Sleep(150);
	}
	return 0;
}

#pragma region Setup

BOOL InitInstance(HINSTANCE hModule)
{
	DisableThreadLibraryCalls(hModule);

	Console::Resize(995, 520, -(1920 - (1280 / 4)), -200);

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&MainThread, NULL, 0, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&ForceLoadThread, NULL, 0, NULL);

	return true;
}

BOOL ExitInstance(HINSTANCE hModule)
{
	FreeLibrary(hModule);
	return true;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hModule, DWORD Reason, LPVOID Misc)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH: return InitInstance(hModule);
	case DLL_PROCESS_DETACH: return ExitInstance(hModule);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		return true;
	}
}

#pragma endregion
