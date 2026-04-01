#include "Pch.hpp"
#include "MessageDispatcher.hpp"

// Internal Project
#include "../CoreEngine.hpp"

// Platform/Thirdparty Libraries
#include <Windows.h>

namespace N503::Core::Message
{

    // --- static method ---

    /// @brief
    /// @param
    /// @param
    /// @param
    /// @param
    /// @return
    auto MessageDispatcher::OSMessageCallback(HWND target, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        return CoreEngine::Instance().GetDefaultMessageRouter().Route(target, message, wParam, lParam);
    }

    // --- instance method ---

    /// @brief
    /// @return
    auto MessageDispatcher::Dispatch() -> bool
    {
        MSG osMessage{};

        while (::PeekMessage(&osMessage, nullptr, 0, 0, PM_REMOVE))
        {
            if (osMessage.message == WM_QUIT)
            {
                return false;
            }

            ::TranslateMessage(&osMessage);
            ::DispatchMessageA(&osMessage);
        }

        return true;
    }

} // namespace N503::Core::Message
