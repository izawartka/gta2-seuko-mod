#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <unknwn.h>

typedef HRESULT(WINAPI* DirectInput8Create_t)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);

static HMODULE hOriginalDll = nullptr;
static DirectInput8Create_t pDirectInput8Create = nullptr;

extern "C" __declspec(dllexport) HRESULT WINAPI DirectInput8Create(
    HINSTANCE hinst,
    DWORD dwVersion,
    REFIID riidltf,
    LPVOID* ppvOut,
    LPUNKNOWN punkOuter)
{
    if (!hOriginalDll)
    {
        char systemPath[MAX_PATH];
        GetSystemDirectoryA(systemPath, MAX_PATH);
        strcat_s(systemPath, "\\dinput8.dll");
        
        hOriginalDll = LoadLibraryA(systemPath);
        
        if (!hOriginalDll)
        {
            MessageBoxA(nullptr, "Failed to load original dinput8.dll from System32", "Seukomod proxy error", MB_ICONERROR);
            return E_FAIL;
        }
        
        pDirectInput8Create = (DirectInput8Create_t)GetProcAddress(hOriginalDll, "DirectInput8Create");
        
        if (!pDirectInput8Create)
        {
            MessageBoxA(nullptr, "Failed to get DirectInput8Create from original dinput8.dll", "Seukomod proxy error", MB_ICONERROR);
            return E_FAIL;
        }
    }
    
    return pDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

void UnloadOriginalDll()
{
    if (hOriginalDll)
    {
        FreeLibrary(hOriginalDll);
        hOriginalDll = nullptr;
        pDirectInput8Create = nullptr;
    }
}
