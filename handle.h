class Handle
{
public:
   HANDLE handle;

   Handle(HANDLE handle) : handle(handle) {};

   ~Handle()
   {
      CloseHandle(handle);
   }

   Module *get_module(const char *module_name)
   {
      HMODULE modules[2048];
      DWORD needed;

      if (!EnumProcessModulesEx(handle, modules, sizeof(modules), &needed, LIST_MODULES_ALL))
      {
         printf("Failed to enumerate process modules\n");
         return nullptr;
      }

      HMODULE module = nullptr;
      for (int i = 0; i < needed / sizeof(HMODULE); i++)
      {
         char name[256];

         if (!GetModuleBaseNameA(handle, modules[i], name, sizeof(name)))
         {
            printf("Failed to get module name\n");
            return nullptr;
         }

         if (strcmp(name, module_name) == 0)
         {
            module = modules[i];
            break;
         }
      }

      if (module == nullptr)
      {
         printf("Module not found\n");
         return nullptr;
      }

      MODULEINFO info;
      if (!GetModuleInformation(handle, module, &info, sizeof(info)))
      {
         printf("Failed to get module information\n");
         return nullptr;
      }

      return new Module(module, info);
   }

   void *find_signature_address_in_module(void *module_address, uint32_t module_size, uint8_t *signature, size_t signature_size)
   {
      size_t bytes_read;
      uint8_t buffer[4096];

      for (size_t i = 0; i < module_size; i += bytes_read)
      {
         if (!ReadProcessMemory(handle, (void *)((uintptr_t)module_address + i), buffer, 4096, &bytes_read))
         {
            printf("Failed to read process memory\n");
            return nullptr;
         }

         for (size_t j = 0; j < (bytes_read - signature_size); j++)
         {
            bool found = true;

            for (size_t k = 0; k < signature_size; k++)
            {
               if (buffer[j + k] != signature[k])
               {
                  found = false;
                  break;
               }
            }

            if (found)
            {
               return (void *)((uintptr_t)module_address + i + j);
            }
         }
      }

      return nullptr;
   }
};
