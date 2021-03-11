#pragma once
#ifdef MINIGIN_EXPORT
#define MINIGIN_ENTRY __declspec(dllexport)
#else
#ifdef MINIGIN_IMPORT
#define MINIGIN_ENTRY __declspec(dllimport)
#else
#define MINIGIN_ENTRY
#endif
#endif
