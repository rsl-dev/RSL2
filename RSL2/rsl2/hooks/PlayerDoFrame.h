#pragma once
#include "common/patching/FunHook.h"
#include "common/Typedefs.h"
#include "rfg/Player.h"
#include "rsl2/misc/GlobalState.h"
#include "rsl2/functions/Functions.h"
#include "rsl2/hooks/Camera.h"
#ifdef COMPILE_IN_PROFILER
#include "tracy/Tracy.hpp"
#endif

FunHook<void(Player*)> PlayerDoFrame_hook
{
    0x6E6290, //__cdecl
    [](Player* player)
    {
        if (!GetGlobalState()->Player)
        {
            GetGlobalState()->Player = player;
        }
        //Todo: Make RSL work with profiler without failing to load the DLL
        //Todo: Wrap tracy macros in our own that can work even when it's not compiled in
#ifdef COMPILE_IN_PROFILER
        ZoneScopedN("PlayerDoFrame");
#endif
        Object* playerObj = reinterpret_cast<Object*>(player);

        //Todo: Provide a per-frame callback that stuff like CameraUpdate can be ran via. Sticking everything in this hook isn't sustainable
        //Update camera
        CameraUpdate();
        //Update player
        PlayerDoFrame_hook.CallTarget(player);
    }
};