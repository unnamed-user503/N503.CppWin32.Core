#pragma once

// C++ Standard Libraries
#include <cstdint>

namespace N503::Core::Command::Packets
{

    /// @brief 
    struct StopEngineCommand final
    {
        /// @brief 
        std::int32_t ExitCode = 0;
    };

} // namespace N503::Core
