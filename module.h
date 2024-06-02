class Module
{
public:
   HINSTANCE module;
   void *base;
   uint32_t size;

   Module(HINSTANCE module, MODULEINFO info)
   {
      module = module;
      base = info.lpBaseOfDll;
      size = info.SizeOfImage;
   }
};