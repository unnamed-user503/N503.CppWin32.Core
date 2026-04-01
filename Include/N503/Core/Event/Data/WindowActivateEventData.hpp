#pragma once

// Internal Project
#include <N503/Core/Event/Tag.hpp>

namespace N503::Core::Event::Data
{

    /// @brief
    struct WindowActivateEventData final
    {
        /// @brief 
        static constexpr ::N503::Core::Event::Tag Tag = ::N503::Core::Event::Tag::Window;

        /// @brief
        bool IsActive;

        /// @brief
        bool IsMinimized;
    };

} // namespace N503::Core::Event::Data
