#include <string>
#include <windows.h>
#include <psapi.h>
#include <vector>
#include <chrono>

#include "./module.h"
#include "./handle.h"
#include "./window.h"

int main()
{
   std::vector<Window> windows = get_windows();

   Window *window = find_window_with_title(windows, "Untitled - Notepad");
   if (window == nullptr)
   {
      printf("Window not found\n");
      return 1;
   }

   printf("PID: %d\n", window->get_pid());

   Handle *handle = window->get_process_handle();
   if (handle == nullptr)
   {
      printf("Failed to get process handle\n");
      return 1;
   }

   Module *module = handle->get_module("client.exe");
   if (module == nullptr)
   {
      printf("Failed to get module\n");
      return 1;
   }

   printf("Clientexe base: %p\n", module->base);
   printf("Clientexe size: %d\n", module->size);

   uint8_t signature[] = {0xFF, 0x50, 0x20, 0x8B, 0xCB, 0x66, 0x89, 0x43, 0x24, 0xFF, 0x15, 0x20, 0x60};

   auto start = std::chrono::high_resolution_clock::now();

   void *signature_addr = handle->find_signature_address_in_module(module->base, module->size, signature, sizeof(signature));

   auto end = std::chrono::high_resolution_clock::now();

   auto elapsed_micros = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

   printf("Elapsed time: %ld microseconds\n", elapsed_micros.count());
   printf("Signature address: %p\n", signature_addr);

   return 0;
}