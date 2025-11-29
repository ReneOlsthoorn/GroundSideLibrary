#include <windows.h>
#include "sidelib.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    switch (reason) {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

// Exported function using .def or __declspec(dllexport)
/*
__declspec(dllexport) int __cdecl add_numbers(int a, int b)
{
    return a + b;
}
*/