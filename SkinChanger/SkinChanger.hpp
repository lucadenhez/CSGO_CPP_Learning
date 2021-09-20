#pragma once

#include "IBaseClientDLL.hpp"

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

// Function signature of IBaseClientDLL::FrameStageNotify.
typedef void(__thiscall* FrameStageNotify_t) (IBaseClientDLL*, ClientFrameStage_t);

// Replacement function that will get called at various points during frame rendering.
void __fastcall hkFrameStageNotify(IBaseClientDLL* thisptr, void* edx, ClientFrameStage_t stage) {
    // Weapon ID: 7, (For AK-47) Skin ID: 639, (For Bloodsport) Wear: 0.00001f (Float)
    skinChanger(clientModule, 7, 639, 0.00001f);
}