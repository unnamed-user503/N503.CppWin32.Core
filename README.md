# N503 Core Library

---

## シンプルな設計

```cpp
#include <N503/Core/Window.hpp>
#include <Windows.h>
#include <format>

int main()
{
    N503::Core::Window coreWindow;

    coreWindow.OnEvent([](N503::Core::Events::MousePressed& data)
    {
        ::MessageBoxW(nullptr, L"Hello", L"World", MB_OK);
        return false;
    });

    coreWindow.OnEvent([&coreWindow](N503::Core::Events::MouseMoved& data)
    {
        auto message = std::format("Mouse Moved: x={}, y={}\n", data.Location.X, data.Location.Y);
        coreWindow.SetTitle(message);
        return false;
    });

    coreWindow.WaitForQuit();
    return 0;
}
```
