#ifdef LBBML_GRAPHICS_APP_EXPORTS
#define LBBML_GRAPHICS_APP_EXPORTS __declspec(dllexport)
#else
#define LBBML_GRAPHICS_APP_EXPORTS __declspec(dllimport)
#endif