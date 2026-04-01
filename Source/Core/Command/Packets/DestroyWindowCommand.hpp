#pragma once

// Platform Libraries
#include <Windows.h>

namespace N503::Core::Command::Packets
{

    /// @brief 
    struct DestroyWindowCommand final
    {
        /// @brief 
        HWND Target;
    };

} // namespace N503::Core
