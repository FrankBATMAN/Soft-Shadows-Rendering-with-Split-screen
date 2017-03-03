#ifdef LBBML_DLL_EXPORTS
#define LBBML_DLL_EXPORTS __declspec(dllexport)
#else
#define LBBML_DLL_EXPORTS __declspec(dllimport)
#endif