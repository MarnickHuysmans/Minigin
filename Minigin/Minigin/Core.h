#pragma once
#ifdef MINIGIN_PLATFORM_WINDOWS
#ifdef MINIGIN_BUILD_DLL
#define MINIGIN_API __declspec(dllexport)
#else
#define MINIGIN_API __declspec(dllimport)
#endif
#else
#error Minigin only supports Windows!
#endif