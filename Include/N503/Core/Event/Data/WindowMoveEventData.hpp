#pragma once

// Internal Project
#include <N503/Core/Event/Tag.hpp>

// C++ Standard Libraries
#include <cstdint>

namespace N503::Core::Event::Data
{

    /// @brief
    struct WindowMoveEventData final
    {
        /// @brief 
        static constexpr ::N503::Core::Event::Tag Tag = ::N503::Core::Event::Tag::Window;

        /// @brief
        std::int32_t X;

        /// @brief
        std::int32_t Y;
    };

} // namespace N503::Core::Event::Data
