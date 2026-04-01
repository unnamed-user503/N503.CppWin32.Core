#pragma once

// Internal Project
#include "EventQueue.hpp"

namespace N503::Core::Event
{

    /// @brief 
    class EventDispatcher final
    {
    public:
        /// @brief 
        EventDispatcher() = default;

        /// @brief 
        ~EventDispatcher() = default;

    public:
        /// @brief 
        /// @param  
        /// @return 
        auto Dispatch(EventQueue&) -> void;
    };

} // namespace N503::Core
