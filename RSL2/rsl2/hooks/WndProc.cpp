#include "WndProc.h"
#include "rsl2/misc/GlobalState.h"

//Functions for locking / unlocking the games auto-centering and hiding of the mouse. For imgui interaction with game running
bool MouseUnlocked = false;
char* MouseGenericPollOriginalCode = nullptr;
char* CenterMouseCursorOriginalCode = nullptr;

void LockMouse()
{
    if (!MouseUnlocked)
        return;

    static RSL2_GlobalState* globalState = GetGlobalState();
    globalState->SnippetManager.RestoreSnippet("MouseGenericPollMouseVisible", true);
    globalState->SnippetManager.RestoreSnippet("CenterMouseCursorCall", true); //Todo: See if this patch is even needed anymore

    printf("Locked mouse movement.\n");
    MouseUnlocked = false;
}
void UnlockMouse()
{
    if (MouseUnlocked)
        return;

    static RSL2_GlobalState* globalState = GetGlobalState();
    globalState->SnippetManager.BackupSnippet("MouseGenericPollMouseVisible", globalState->MouseGenericPollMouseVisibleAddress, 12, true);
    globalState->SnippetManager.BackupSnippet("CenterMouseCursorCall", globalState->CenterMouseCursorCallAddress, 11, true);

    MouseUnlocked = true;
    printf("Unlocked mouse movement.\n");
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT ProcessInput(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);// , const KeyState& keys);
extern LRESULT __stdcall RSL2_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static RSL2_GlobalState* globalState = GetGlobalState();

    //Pass input to RSL2 input function
    ProcessInput(hWnd, msg, wParam, lParam);

    //Todo: Make it so you can have imgui visible without unlocking mouse look. Useful for data overlays
    if (globalState->GuiActive)
    {
        //When imgui is active only pass WM_SIZE messages to game so it doesn't react to gui mouse/keyboard input
        if (msg == WM_SIZE)
            return CallWindowProc(globalState->RfgWndProc, hWnd, msg, wParam, lParam);

        //Todo: Pass input to dear imgui when necessary
        ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
        return true;
    }

    //If no blocking conditions met simply pass message to game WndProc
    return CallWindowProc(globalState->RfgWndProc, hWnd, msg, wParam, lParam);
}

LRESULT ProcessInput(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)// , const KeyState& keys);
{
    static RSL2_GlobalState* globalState = GetGlobalState();
    if (!globalState->ImGuiInitialized)
        return 0;

    if (msg == WM_KEYDOWN)
    {
        switch (wParam)
        {
        case VK_F1:
            globalState->GuiActive = !globalState->GuiActive;
            if (globalState->GuiActive)
                UnlockMouse();
            else
                LockMouse();

            break;
        case VK_F2:
            globalState->OverlayActive = !globalState->OverlayActive;
        }
    }
}