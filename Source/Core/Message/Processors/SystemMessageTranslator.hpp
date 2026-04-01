#pragma once

// Internal Project
#include "MessageContext.hpp"

namespace N503::Core::Message::Processors
{

    /// @brief
    class SystemMessageTranslator final
    {
    public:
        /// @brief
        SystemMessageTranslator() = default;

        /// @brief
        ~SystemMessageTranslator() = default;

    public:
        /// @brief
        /// @param context
        /// @return
        auto Process(MessageContext& context) -> bool;
    };

} // namespace N503::Core::Message::Processors
