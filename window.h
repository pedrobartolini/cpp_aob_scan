class Window
{
public:
   HWND hwnd;
   std::string title;

   Window(HWND hwnd, const char *title)
   {
      this->hwnd = hwnd;
      this->title = title;
   }

   bool is_focused()
   {
      return GetForegroundWindow() == hwnd;
   }

   uint32_t get_pid()
   {
      DWORD pid;
      GetWindowThreadProcessId(hwnd, &pid);
      return pid;
   }

   Handle *get_process_handle()
   {
      HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, get_pid());

      if (handle == NULL)
      {
         return nullptr;
      }

      return new Handle(handle);
   }
};

int EnumWindowsProc(HWND hwnd, LPARAM windows)
{
   char title[256];
   GetWindowTextA(hwnd, title, sizeof(title));
   Window window(hwnd, title);
   ((std::vector<Window> *)windows)->push_back(window);
   return 1;
}

std::vector<Window> get_windows()
{
   std::vector<Window> windows;
   EnumWindows(EnumWindowsProc, (LPARAM)&windows);
   return windows;
}

Window *find_window_with_title(std::vector<Window> &windows, const char *title)
{
   for (Window &window : windows)
   {
      if (window.title.find(title) != std::string::npos)
      {
         return &window;
      }
   }
   return nullptr;
}
