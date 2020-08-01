﻿#include "common/windows/WindowsWrapper.h"
#include "common/patching/FunHook.h"
#include "hooks/PlayerDoFrame.h"
#include "functions/Functions.h"
#include "hooks/GrdRenderHooks.h"
#include "hooks/RenderHooks.h"
#include "hooks/WndProc.h"
#include "misc/GlobalState.h"
#include "common/Typedefs.h"
#include "common/Common.h"
#include "rsl2/patching/Offset.h"
#include "rsl2/hooks/Camera.h"
#ifdef COMPILE_IN_PROFILER
#include "tracy/Tracy.hpp"
#endif
#include <kiero/kiero.h>
#include <cstdio>

//Note: When a plugin is reloaded the host calls RSL2_PluginUnload and then RSL2_PluginInit again
//      as if it were the first time the host were loading the plugin.

//Need to use extern "C" to avoid C++ export name mangling. Lets us use the exact name RSL2_XXXX with GetProcAddress in the host
extern "C"
{
    //Called when the host dll loads this plugin
    DLLEXPORT bool __cdecl RSL2_PluginInit()
    {
        printf("RSL2.dll RSL2_PluginInit() called!!\n");
        RSL2_GlobalState* globalState = GetGlobalState();

        globalState->ModuleBase = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
        printf("ModuleBase: %d\n", globalState->ModuleBase);

        globalState->World = OffsetPtr<world*>(0x02B97490);
        globalState->MainCamera = OffsetPtr<rfg_camera*>(0x019E3B50);

        if (kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success)
        {
            printf("Error! Failed to init kiero in RSdL2.dll!\n"); 
            
            return false;
        }

        globalState->MouseGenericPollMouseVisibleAddress = globalState->ModuleBase + 0x001B88DC;
        globalState->CenterMouseCursorCallAddress = globalState->ModuleBase + 0x878D90;

        //Todo: Maybe set ModuleBase in FuncHook and provide option to offset function address from it for convenience & less mistakes
        PlayerDoFrame_hook.SetAddr(globalState->ModuleBase + 0x6E6290);
        PlayerDoFrame_hook.Install();
        keen_graphics_beginFrame.SetAddr(globalState->ModuleBase + 0x0086A8A0);
        keen_graphics_beginFrame.Install();

        //Setup pointers to game functions
        InitFunctionPointers();
        InitCameraPatches();

        //D3D11 hooks
        D3D11_ResizeBuffersHook.SetAddr(kiero::getMethodsTable()[13]);
        D3D11_ResizeBuffersHook.Install();
        D3D11_PresentHook.SetAddr(kiero::getMethodsTable()[8]);
        D3D11_PresentHook.Install();

        grd_string_hook.SetAddr(globalState->ModuleBase + 0x000B8DA0);
        grd_string_hook.Install();
        grd_3d_string_hook.SetAddr(globalState->ModuleBase + 0x000B8DE0);
        grd_3d_string_hook.Install();
        grd_get_string_size_hook.SetAddr(globalState->ModuleBase + 0x000B8DF0);
        grd_get_string_size_hook.Install();
        grd_get_font_height.SetAddr(globalState->ModuleBase + 0x000B8E30);
        grd_get_font_height.Install();
        grd_rect_hook.SetAddr(globalState->ModuleBase + 0x000B8E40);
        grd_rect_hook.Install();
        grd_set_color_hook.SetAddr(globalState->ModuleBase + 0x000B8E50);
        grd_set_color_hook.Install();
        grd_line_hook.SetAddr(globalState->ModuleBase + 0x000B8EB0);
        grd_line_hook.Install();
        grd_sphere_hook.SetAddr(globalState->ModuleBase + 0x000B8EC0);
        grd_sphere_hook.Install();
        grd_bbox_aligned_hook.SetAddr(globalState->ModuleBase + 0x000B8F00);
        grd_bbox_aligned_hook.Install();
        grd_bbox_oriented_hook.SetAddr(globalState->ModuleBase + 0x000B8F20);
        grd_bbox_oriented_hook.Install();

        primitive_renderer_begin_deferredHook.SetAddr(globalState->ModuleBase + 0x000F0E50);
        primitive_renderer_begin_deferredHook.Install();

        return true;
    }

    //Called when the host dll unloads this plugin
    DLLEXPORT bool __cdecl RSL2_PluginUnload()
    {
        printf("RSL2.dll RSL2_PluginUnload() called!\n");
        RSL2_GlobalState* globalState = GetGlobalState();

        kiero::shutdown();

        //Remove hooks
        PlayerDoFrame_hook.Remove();
        keen_graphics_beginFrame.Remove();
        D3D11_ResizeBuffersHook.Remove();
        D3D11_PresentHook.Remove();
        grd_string_hook.Remove();
        grd_3d_string_hook.Remove();
        grd_get_string_size_hook.Remove();
        grd_get_font_height.Remove();
        grd_rect_hook.Remove();
        grd_set_color_hook.Remove();
        grd_line_hook.Remove();
        grd_sphere_hook.Remove();
        grd_bbox_aligned_hook.Remove();
        grd_bbox_oriented_hook.Remove();
        primitive_renderer_begin_deferredHook.Remove();


        //Relock mouse and camera so game has full control of them and patches are removed
        LockMouse();
        LockCamera();

        //Remove custom WndProc
        SetWindowLongPtr(globalState->gGameWindowHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(globalState->RfgWndProc));
        return true;
    }
}