#pragma once

// C++ Standard Libraries
#include <cstdint>
#include <string_view>

// Platform Libraries
#include <Windows.h>

namespace N503::Core::Command::Packets
{

    /// @brief 
    struct CreateWindowCommand final
    {
        /// @brief 
        HWND Parent = nullptr;

        /// @brief 
        std::string_view Title = "";

        /// @brief 
        std::uint32_t Width = 640;

        /// @brief 
        std::uint32_t Height = 480;

        /// @brief 
        HWND* Result = nullptr;
    };

} // namespace N503::Core
