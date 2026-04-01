#pragma once

// Internal Project
#include "CreateWindowCommand.hpp"
#include "DestroyWindowCommand.hpp"
#include "SetWindowTitleCommand.hpp"
#include "StopEngineCommand.hpp"

// C++ Standard Libraries
#include <variant>

namespace N503::Core::Command::Packets
{

    /// @brief 
    using CommandPacket = std::variant<
        CreateWindowCommand,
        DestroyWindowCommand,
        SetWindowTitleCommand,
        StopEngineCommand
    >;

} // namespace N503::Core
