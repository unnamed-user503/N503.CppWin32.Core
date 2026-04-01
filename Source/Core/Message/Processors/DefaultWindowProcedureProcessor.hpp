#pragma once

// Internal Project
#include "MessageContext.hpp"

namespace N503::Core::Message::Processors
{

    /// @brief
    class DefaultWindowProcedureProcessor final
    {
    public:
        /// @brief
        DefaultWindowProcedureProcessor() = default;

        /// @brief
        ~DefaultWindowProcedureProcessor() = default;

    public:
        /// @brief
        /// @param context
        /// @return
        auto Process(MessageContext& context) -> bool;
    };

} // namespace N503::Core::Message::Processors
