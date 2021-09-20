// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <memory>
#include "IBaseClientDLL.hpp"
#include "offsets.h"
#include "vmthook.h"
//#include "SkinChanger.hpp"
#include "Interface.hpp"
#include "Virtuals.hpp"

struct Vec3 {
    float x, y, z;
};

std::unique_ptr<VMTHook> clientDLLhook;
IBaseClientDLL* clientDLL = nullptr;
uintptr_t clientModule = NULL;

void skinChanger(uintptr_t clientModule, int weaponID, int skinID, float wear) {
    uintptr_t localPlayer = *(uintptr_t*)(clientModule + offsets::dwLocalPlayer); //get local player (moduleBase - ptr to client.dll)

    if (localPlayer) { //if our local player exists
        for (int i = 0; i < 8; i++) //iterate over the weapons in the players inventory (I assume 8 becuase: Rifle, Handgun, Knife, Grenade 1, Grenade 2, Defuse Kit, Tazer, and Armor) 
        {
            int cWeapon = *(int*)(localPlayer + offsets::m_hMyWeapons + i * 0x4) & 0xfff; //get the current weapon
            cWeapon = *(int*)(clientModule + offsets::dwEntityList + (cWeapon - 1) * 0x10); //find the weapon in the entity list
            if (cWeapon != 0) { //if the weapon is valid
                short cWeaponID = *(short*)(cWeapon + offsets::m_iItemDefinitionIndex); //get the weapon ID (AK-47 is ID 7)
                if (cWeaponID == weaponID) { //if the weapon is an AK-47
                    *(int*)(cWeapon + offsets::m_iItemIDHigh) = -1; //force the game to use the fallback values
                    *(int*)(cWeapon + offsets::m_nFallbackPaintKit) = skinID; //set the paintkit
                    *(float*)(cWeapon + offsets::m_flFallbackWear) = wear; //set the wear
                }
            }
        }
    }
}

    // Weapon ID: 7, (For AK-47) Skin ID: 639, (For Bloodsport) Wear: 0.00001f (Float)
    //skinChanger(clientModule, 7, 639, 0.00001f);

// Function signature of IBaseClientDLL::FrameStageNotify.
typedef void(__thiscall* FrameStageNotify_t) (IBaseClientDLL*, ClientFrameStage_t);

// Replacement function that will get called at various points during frame rendering.
void __fastcall hkFrameStageNotify(IBaseClientDLL* thisptr, void* edx, ClientFrameStage_t stage) {
    MessageBox(NULL, L"bruh", L"DLL says:", MB_OK);
    // Weapon ID: 7, (For AK-47) Skin ID: 639, (For Bloodsport) Wear: 0.00001f (Float)
    skinChanger(clientModule, 7, 639, 0.00001f);
}

int WINAPI MainThread(HMODULE hModule) {
    MessageBox(NULL, L"Hello from HackThread!", L"DLL says:", MB_OK);

    clientModule = (uintptr_t)GetModuleHandle(L"client.dll");
    uintptr_t engineModule = (uintptr_t)GetModuleHandle(L"engine.dll");
    uintptr_t localPlayer = *(uintptr_t*)(clientModule + offsets::dwLocalPlayer);
    clientDLL = CaptureInterface<IBaseClientDLL>("client.dll", CLIENT_DLL_INTERFACE_VERSION);

    //Vec3* viewAngles = (Vec3*)(*(uintptr_t*)(engineModule + offsets::dwClientState) + offsets::dwClientState_ViewAngles);
    //*viewAngles = { 0, 0, 0 };

    clientDLLhook = std::make_unique<VMTHook>(clientDLL);
    clientDLLhook->HookFunction(hkFrameStageNotify, 36);

    MessageBox(NULL, L"Successfully hooked!", L"DLL says:", MB_OK);

    FreeLibraryAndExitThread(hModule, 0);
    CloseHandle(hModule);
    return 0;

    // CS:GO updates being weird with changing 'client_panorama.dll' and 'client.dll'. Valve, that's not gonna stop cheats?!?
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, nullptr);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
