#include "common/windows/WindowsWrapper.h"
#include "common/plugins/Plugin.h"
#include "common/filesystem/Path.h"
#include "common/timing/Timer.h"
#include <cstdio>
#include <vector>
#include <filesystem>
#include <TlHelp32.h>

DWORD WINAPI HostThread(HMODULE hModule);
void LoadPlugins();
void ResumeRfgMainThread(HMODULE hModule);
bool OnIgnoreList(const string& dllNameNoExt);
void OpenConsoleWindow();
void CloseConsoleWindow();

BOOL WINAPI DllMain(HINSTANCE dllHandle, DWORD reason, LPVOID reserved)
{
    //Reminder for later use if needed
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        OpenConsoleWindow();
        CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(HostThread), dllHandle, 0, 0);
        break;
    case DLL_PROCESS_DETACH:
        CloseConsoleWindow();
        break;
    default:
        break;
    }

    return TRUE;
}

//Holds plugins loaded by the host dll
static std::vector<Plugin> Plugins;
//Dll names that the host won't try to load as plugins 
static const std::vector<string> PluginIgnoreList =
{
    //DLLs included with RFGR 
    "binkw32",
    "discord-rpc",
    "Galaxy",
    "GalaxyPeer",
    "GfeSDK",
    "libeay32",
    "ssleay32",
    "sw_api",

    //Non-plugin RSL2 DLLs
    "Profiler",
    "Host"
};

//Todo: Stick this in a file that's loaded at runtime
//Path to look for plugin dlls. Special value in debug builds for convenience
#ifdef DEBUG_BUILD
string pluginsPath = "C:\\Users\\moneyl\\source\\repos\\RSL2\\out\\build\\x86-Debug\\bin\\";
#else
string pluginsPath = "./RSL2/Plugins/";
#endif

//Todo: Provide host interface for plugins to interact with. Uses:
//Todo:     - Query for list of plugins and info about them
//Todo:     - Grab functions exported from other plugins
//Todo:     - Trigger reload of self or other plugins
//Todo:     - Maybe save persistent state between reloads by allocating it on the heap and giving the host a pointer to that data temporarily

//Todo: (Maybe. Might need to do this for DLLs like Dx11 to load into rfg.exe. Necessary for patching D3D11Present) Patch tmain entrypoint to infinite loop so we can patch in piece.

DWORD WINAPI HostThread(HMODULE hModule)
{
    //Todo: Use an actual logger
    printf("Host thread started.\n");

    printf("Loading plugin dlls...\n");
    LoadPlugins();

    printf("Done loading plugin dlls. Resuming RFGR main thread...\n");
    ResumeRfgMainThread(hModule);

    //Loop through plugins periodically and reload any that need to be reloaded
    printf("RFG main thread resumed. Host thread will now perform period plugin change checks and reload them when necessary.\n");
    u32 pluginChangeCheckDelayMs = 1000u; //Time between plugin reload checks in ms (1000ms = 1s)
    Timer pluginChangeTimer;
    while (true)
    {
        if (pluginChangeTimer.ElapsedMilliseconds() < pluginChangeCheckDelayMs)
            continue;

        auto pluginIterator = Plugins.begin();
        while(pluginIterator != Plugins.end())
        {
            if (pluginIterator->NeedsReload())
            {
                bool reloadResult = pluginIterator->Reload();
                if (reloadResult)
                {
                    printf("Reloaded %s!\n", Path::GetFileName(pluginIterator->Path()).c_str());
                }
                else
                {
                    printf("Failed to reload %s. Removing it from the plugin list.\n", Path::GetFileName(pluginIterator->Path()).c_str());
                    pluginIterator = Plugins.erase(pluginIterator);
                    continue;
                }
            }
            pluginIterator++;
        }

        pluginChangeTimer.Reset();
    }

    //Exit
    printf("Host thread exiting...\n");
    FreeLibraryAndExitThread(hModule, 0);
}

void LoadPlugins()
{
    //Loop through DLLs in working directory. Attempt to load them as plugins
    for (const auto& entry : std::filesystem::directory_iterator(pluginsPath))
    {
        //Skip folders and non .dll files
        if (!entry.path().has_extension())
            continue;
        if (entry.path().extension() != ".dll")
            continue;
        if (OnIgnoreList(Path::GetFileNameNoExtension(entry.path())))
            continue;

        //Create plugin. Load it. Add to Plugins if it loads correctly
        Plugin plugin(entry.path().string());
        if (plugin.Load())
            Plugins.push_back(plugin);
    }
}

void ResumeRfgMainThread(HMODULE hModule)
{
    //Find main thread of rfg.exe and resume it so the game runs
    DWORD PID = GetCurrentProcessId();
    const DWORD hostThreadId = GetThreadId(hModule);
    HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (h != INVALID_HANDLE_VALUE)
    {
        THREADENTRY32 te;
        te.dwSize = sizeof(te);
        if (Thread32First(h, &te))
        {
            do
            {
                if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(te.th32OwnerProcessID))
                {
                    if (te.th32OwnerProcessID == PID)
                    {
                        if (te.th32ThreadID != hostThreadId)
                        {
                            HANDLE ThreadHandle = OpenThread(THREAD_ALL_ACCESS, FALSE, te.th32ThreadID);
                            if (ThreadHandle)
                            {
                                ResumeThread(ThreadHandle);
                            }
                        }
                        //printf("Process 0x%04x Thread 0x%04x\n", te.th32OwnerProcessID, te.th32ThreadID);
                        //if(te.th32ThreadID == LauncherThreadID)
                        //{
                        //    printf("^^^That's the launcher thread!\n");
                        //}
                    }
                }
                te.dwSize = sizeof(te);
            } while (Thread32Next(h, &te));
        }
        CloseHandle(h);
    }
    else
    {
        printf("Failed to get toolhelp 32 snapshot!\n");
    }
}

bool OnIgnoreList(const string& dllNameNoExt)
{
    //Ignore dlls that contain the substring "copy"
    if (dllNameNoExt.find("copy") != string::npos) //Todo: Make into helper functions in String:: namespace
        return true;

    //Ignore dlls with names on the ignore list
    for (const auto& ignoredName : PluginIgnoreList)
    {
        if (dllNameNoExt == ignoredName)
            return true;
    }

    return false;
}

void OpenConsoleWindow()
{
    AllocConsole();
    FILE* pFile = NULL;
    freopen_s(&pFile, "CONOUT$", "r+", stdout);
}

void CloseConsoleWindow()
{
    FreeConsole();
}