#pragma once

// Internal Project
#include "MessageContext.hpp"

// Platform/Thirdparty Libraries
#include <Windows.h>
#include <wil/resource.h>

namespace N503::Core::Message::Processors
{

    /// @brief
    class MessageInterruptProcessor final
    {
    public:
        /// @brief
        MessageInterruptProcessor() = default;

        /// @brief
        ~MessageInterruptProcessor() = default;

    public:
        /// @brief
        /// @param context
        /// @return
        auto Process(MessageContext& context) -> bool;

        /// @brief 
        /// @param context 
        /// @return 
        auto OnEnterSizeMove(MessageContext& context) -> bool;

        /// @brief 
        /// @param context 
        /// @return 
        auto OnTimer(MessageContext& context) -> bool;

        /// @brief 
        /// @param context 
        /// @return 
        auto OnExitSizeMove(MessageContext& context) -> bool;

        /// @brief 
        /// @param context 
        /// @return 
        auto OnEnterIdle(MessageContext& context) -> bool;

    private:
        /// @brief 
        UINT_PTR m_TimerID{ 0 };

        /// @brief 
        wil::unique_hhook m_HookHandle{ nullptr };
    };

} // namespace N503::Core::Message::Processors
