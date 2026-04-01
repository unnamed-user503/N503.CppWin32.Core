#pragma once

// Internal Project
#include <N503/Core/Event/Tag.hpp>

// C++ Standard Libraries
#include <cstdint>

namespace N503::Core::Event::Data
{

    /// @brief
    struct WindowSizeEventData final
    {
        /// @brief 
        static constexpr ::N503::Core::Event::Tag Tag = ::N503::Core::Event::Tag::Window;

        /// @brief
        std::uint32_t Width;

        /// @brief
        std::uint32_t Height;

        /// @brief
        std::uint32_t Type;
    };

} // namespace N503::Core::Event::Data
