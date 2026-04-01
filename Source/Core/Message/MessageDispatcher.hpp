#pragma once

// Platform/Thirdparty Libraries
#include <Windows.h>

namespace N503::Core::Message
{

    /// @brief
    class MessageDispatcher
    {
    public:
        /// @brief
        /// @param
        /// @param
        /// @param
        /// @param
        /// @return
        static auto CALLBACK OSMessageCallback(HWND, UINT, WPARAM, LPARAM) -> LRESULT;

    public:
        /// @brief 
        MessageDispatcher() = default;

        /// @brief 
        ~MessageDispatcher() = default;

    public:
        /// @brief 
        /// @return 
        auto Dispatch() -> bool;
    };

} // namespace N503::Core::Message
