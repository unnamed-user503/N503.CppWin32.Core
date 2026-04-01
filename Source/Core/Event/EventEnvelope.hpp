#pragma once

// Internal Project
#include "Data/EventPacket.hpp"

// C++ Standard Libraries
#include <semaphore>

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
        std::binary_semaphore* Signal = nullptr;

        /// @brief 
        HWND Target = nullptr;
    };

} // namespace N503::Core
