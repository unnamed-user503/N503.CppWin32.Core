#include "Pch.hpp"
#include "SystemMessageTranslator.hpp"

// Internal Project
#include "MessageContext.hpp"
#include "MessageDelegate.hpp"

// C++ Standard Libraries
#include <tuple>
#include <type_traits>
#include <utility>

// Platform/Thirdparty Libraries
#include <Windows.h>

namespace N503::Core::Message::Processors
{

    /// @brief
    /// @param context
    /// @return
    auto SystemMessageTranslator::Process(MessageContext& context) -> bool
    {
        return false;
    }

} // namespace N503::Core::Message::Processors
