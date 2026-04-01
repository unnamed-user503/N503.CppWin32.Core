#pragma once

// Internal Project
#include "MessageRouter.hpp"
#include "Processors/DefaultWindowProcedureProcessor.hpp"
#include "Processors/KeyboardMessageTranslator.hpp"
#include "Processors/MouseMessageTranslator.hpp"
#include "Processors/SystemMessageTranslator.hpp"
#include "Processors/WindowEntityBindProcessor.hpp"
#include "Processors/WindowLifecycleTracker.hpp"
#include "Processors/WindowMessageTranslator.hpp"

namespace N503::Core::Message
{

    /// @brief 
    using DefaultMessageRouter = MessageRouter<
        Processors::WindowLifecycleTracker,
        Processors::WindowEntityBindProcessor,
        Processors::DefaultWindowProcedureProcessor,
        Processors::WindowMessageTranslator,
        Processors::MouseMessageTranslator,
        Processors::KeyboardMessageTranslator,
        Processors::SystemMessageTranslator
    >;

} // namespace N503::Core::Message
