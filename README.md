Note: Development of this project is on hold to focus on [Nanoforge](https://github.com/Moneyl/Nanoforge/).

# RSL2
Redesign/re-write of the [RSL (Remarstered Script Loader)](https://github.com/rsl-dev/RSL) aiming to learn from the mistakes in design and direction of the original RSL. Due to the amount of issues with the old codebase and how clunky it had become to develop for it was easier just to start from scratch and take a new approach.

RSL2 uses the same code injection and function hooking code as [DashFaction](https://github.com/rafalh/dashfaction), a Red Faction 1 Community Patch.

## Reasons for a rewrite
- Long iteration times: The RSL1 codebase got to a point where incremental compiles could take up to 2 minutes. On top of that the game needed to be restarted for each change. This made development a long and tedious process even for small changes
- Large but mostly useless scripting API: When the scripting API was added the intent was to give modders as many tools as possible. This meant binding every member variable the C++ code could access to Lua. The intent was good but in practice this backfired. Many of variables bound to Lua had no known use, meaning it was hard for users to sort out the useful variables from the chaff. It likely also contributed to the long compile times.

## How the rewrite fixes these issues
- Plugin based architecture: Most of the code lies in DLLs which can be hot reloaded without needing to restart the game. This improves developer iteration times significantly. It should also help with compile times long term because any code with a long compile time could be moved into it's own DLL.
- Selective scripting API additions: RSL2 doesn't have scripting yet, but when it's added a different approach will be taken. Instead of binding everything available to it only variables and functions with a proven use will be added. This will result in a smaller API but everything in it will have a clear and well described purpose. 
