#ifdef LBBML_GRAPHICS_DLL_EXPORTS
#define LBBML_GRAPHICS_DLL_EXPORTS __declspec(dllexport)
#else
#define LBBML_GRAPHICS_DLL_EXPORTS __declspec(dllimport)
#endif