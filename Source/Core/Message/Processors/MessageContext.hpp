#pragma once

// Platform/Thirdparty Libraries
#include <Windows.h>

namespace N503::Core::Message::Processors
{

    /// @brief
    struct MessageContext final
    {
        /// @brief
        HWND Target = nullptr;

        /// @brief
        UINT Message = 0;

        /// @brief
        WPARAM WParam = 0;

        /// @brief
        LPARAM LParam = 0;

        /// @brief
        bool Handled = false;

        /// @brief
        LRESULT Result = 0;
    };

} // namespace N503::Core::Message::Processors
