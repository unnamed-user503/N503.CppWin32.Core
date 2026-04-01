#pragma once

// Internal Project
#include "Packets/CommandPacket.hpp"

// C++ Standard Libraries
#include <semaphore>

namespace N503::Core::Command
{

    /// @brief 
    struct CommandEnvelope final
    {
        Packets::CommandPacket Packet;

        std::binary_semaphore* Signal = nullptr;
    };

} // namespace N503::Core
