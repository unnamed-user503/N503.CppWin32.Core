#pragma once

// Internal Project
#include "MessageContext.hpp"

namespace N503::Core::Message::Processors
{

    /// @brief
    class WindowEntityBindProcessor final
    {
    public:
        /// @brief
        WindowEntityBindProcessor() = default;

        /// @brief
        ~WindowEntityBindProcessor() = default;

    public:
        /// @brief
        /// @param 
        /// @return
        auto Process(MessageContext&) -> bool;

        /// @brief 
        /// @param 
        /// @return 
        auto OnNCCreate(MessageContext&) -> bool;

        /// @brief 
        /// @param 
        /// @return 
        auto OnNCDestroy(MessageContext&) -> bool;
    };

} // namespace N503::Core::Message::Processors
