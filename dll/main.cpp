#include <windows.h>
#include <iostream>

void WINAPI child()
{
   MessageBoxA(nullptr, "Hello from child!", "Hello from child!", MB_OK);
}

uint32_t WINAPI Thread(void *module)
{

   MessageBoxA(nullptr, "Hello World!", "Hello World!", MB_OK);

   // spawn child thread
   CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)child, nullptr, 0, nullptr));

   FreeLibraryAndExitThread((HMODULE)module, 0);

   return 0;
}

bool APIENTRY DllMain(void *module, uint32_t reason, void *reserved)
{
   switch (reason)
   {
   case DLL_PROCESS_ATTACH:
      CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Thread, module, 0, nullptr));
      break;
   case DLL_THREAD_ATTACH:
   case DLL_THREAD_DETACH:
   case DLL_PROCESS_DETACH:
      break;
   }

   return TRUE;
}