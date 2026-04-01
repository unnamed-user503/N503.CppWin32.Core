#pragma once

// Internal Project
#include "MessageContext.hpp"

namespace N503::Core::Message::Processors
{

    template <typename TMessageID, auto TCallback>
    struct MessageDelegate
    {
        using MessageID = TMessageID;

        template <typename TProcessor>
        bool operator()(TProcessor* processor, MessageContext& context) const
        {
            return (processor->*TCallback)(context);
        }
    };

} // namespace N503::Core::Message::Processors
