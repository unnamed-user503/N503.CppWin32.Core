#pragma once

// Internal Project
#include "Data/EventPacket.hpp"

// Platform/Thirdparty Libraries
#include <Windows.h>

namespace N503::Core::Event
{

    /// @brief 
    struct EventEnvelope final
    {
        /// @brief 
        Data::EventPacket Packet;

        /// @brief 
        HWND Target = nullptr;
    };

} // namespace N503::Core
