﻿cmake_minimum_required (VERSION 3.8)
project (RSL2 VERSION 1.0.0
                  DESCRIPTION "RSL2"
                  LANGUAGES CXX)

# Make sure cmake doesn't add the build config name to the end of built DLLs (eg. RSL2_debug.dll instead of RSL2.dll)
set(CMAKE_DEBUG_POSTFIX "")
set(CMAKE_RELEASE_POSTFIX "")
set(CMAKE_RELWITHDEBINFO_POSTFIX "")

# Get list of source files in project src folders recursively
# Places in SOURCES variable
file(GLOB_RECURSE SOURCES 
    ${CMAKE_SOURCE_DIR}/RSL2/rsl2/*.cpp
    ${CMAKE_SOURCE_DIR}/RSL2/rsl2/*.h
)

add_compile_definitions(IMGUI_IMPL_WIN32_DISABLE_LINKING_XINPUT IMGUI_IMPL_WIN32_DISABLE_GAMEPAD)

# Add source to this project's DLL
add_library(RSL2 SHARED ${SOURCES}
            ${CMAKE_SOURCE_DIR}/Dependencies/subhook/subhook.c
            ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui.cpp
            ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui_demo.cpp
            ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui_draw.cpp
            ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui_widgets.cpp
            ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui_tables.cpp
            ${CMAKE_SOURCE_DIR}/dependencies/imgui/backends/imgui_impl_win32.cpp
            ${CMAKE_SOURCE_DIR}/dependencies/imgui/backends/imgui_impl_dx11.cpp
            ${CMAKE_SOURCE_DIR}/dependencies/kiero/kiero.cpp
            )

target_link_libraries(RSL2 PRIVATE subhook)
target_link_libraries(RSL2 PRIVATE RFGR_Types)
target_link_libraries(RSL2 PRIVATE Common)

# Compile in profiler if it's flag is set
if(COMPILE_IN_PROFILER)
    target_link_libraries(RSL2 PRIVATE Profiler)
endif()

# Add include directories
target_include_directories(RSL2 SYSTEM PRIVATE
    ${CMAKE_SOURCE_DIR}/Common/
    ${CMAKE_SOURCE_DIR}/RSL2/
    ${CMAKE_SOURCE_DIR}/dependencies/
    ${CMAKE_SOURCE_DIR}/dependencies/imgui/
    ${CMAKE_SOURCE_DIR}/dependencies/RFGR_Types/
)

add_compile_definitions(TRACY_IMPORTS)