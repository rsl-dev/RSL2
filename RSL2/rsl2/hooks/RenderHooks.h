#pragma once
#include "common/patching/FunHook.h"
#include "common/windows/WindowsWrapper.h"
#include <d3d11.h>

struct IDXGISwapChain;
namespace keen
{
    struct GraphicsSystem;
    struct RenderSwapChain;
}

using D3D11_ResizeBuffersHook_Type = HRESULT __stdcall(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
extern HRESULT __stdcall D3D11_ResizeBuffersHookFunc(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
extern FunHook<D3D11_ResizeBuffersHook_Type> D3D11_ResizeBuffersHook;

//keen::GraphicsCommandBuffer *__cdecl keen::graphics::beginFrame(keen::GraphicsSystem *pGraphicsSystem, keen::RenderSwapChain *pSwapChain)
extern FunHook<void* (keen::GraphicsSystem* pGraphicsSystem, keen::RenderSwapChain* pSwapChain)> keen_graphics_beginFrame;

using D3D11_PresentHook_Type = HRESULT __stdcall(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
extern HRESULT __stdcall D3D11_PresentHookFunc(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
extern FunHook<D3D11_PresentHook_Type> D3D11_PresentHook;

extern bool ReadyForD3D11Init();

//Todo: Add this struct
struct rl_primitive_renderer;
using primitive_renderer_begin_deferredHook_Type = void __fastcall(rl_primitive_renderer* thisPtr);
extern void __fastcall primitive_renderer_begin_deferredHook_Func(rl_primitive_renderer* thisPtr);
extern FunHook<primitive_renderer_begin_deferredHook_Type> primitive_renderer_begin_deferredHook;


//Get ImGui context. Plugins which want to use dear imgui must get and set the context from this dll
struct ImGuiContext;
extern ImGuiContext* GetImGuiContext();

//Callback where you can safely use dear imgui windows/primitives
using ImGuiCallbackFunc = void(__cdecl*)();
extern void RegisterImGuiCallback(ImGuiCallbackFunc callback);
extern void RemoveImGuiCallback(ImGuiCallbackFunc callback);

//Callback where you can safely use dear imgui windows/primitives. Explicitly for the purpose of overlays that are independently visible from GUIs.
using OverlayCallbackFunc = void(__cdecl*)();
extern void RegisterOverlayCallback(OverlayCallbackFunc callback);
extern void RemoveOverlayCallback(OverlayCallbackFunc callback);

//Callback where rfg primitive rendering functions can be used safely. E.g. rfg::gr_rect and rfg::gr_3d_line
using PrimitiveDrawCallbackFunc = void(__cdecl*)();
extern void RegisterPrimitiveDrawCallback(PrimitiveDrawCallbackFunc callback);
extern void RemovePrimitiveDrawCallback(PrimitiveDrawCallbackFunc callback);