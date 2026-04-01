#pragma once

// C++ Standard Libraries
#include <string>

// Platform Libraries
#include <Windows.h>

namespace N503::Core::Command::Packets
{

    /// @brief 
    struct SetWindowTitleCommand final
    {
        /// @brief 
        HWND Target;

        /// @brief 
        std::string Title;
    };

} // namespace N503::Core
