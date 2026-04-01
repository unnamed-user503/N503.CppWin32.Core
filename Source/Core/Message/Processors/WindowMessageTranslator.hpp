#pragma once

// Internal Project
#include "MessageContext.hpp"

namespace N503::Core::Message::Processors
{

    /// @brief
    class WindowMessageTranslator final
    {
    public:
        /// @brief
        WindowMessageTranslator() = default;

        /// @brief
        ~WindowMessageTranslator() = default;

    public:
        /// @brief 
        /// @param  
        /// @return 
        auto Process(MessageContext&) -> bool;

    private:
        /// @brief 
        /// @param  
        /// @return 
        auto OnActivate(MessageContext&) -> bool;

        /// @brief 
        /// @param  
        /// @return 
        auto OnClose(MessageContext&) -> bool;

        /// @brief 
        /// @param  
        /// @return 
        auto OnMove(MessageContext&) -> bool;

        /// @brief 
        /// @param  
        /// @return 
        auto OnSize(MessageContext&) -> bool;
    };

} // namespace N503::Core::Message::Processors
