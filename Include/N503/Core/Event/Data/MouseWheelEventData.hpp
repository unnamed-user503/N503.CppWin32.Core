#pragma once

// Internal Project
#include <N503/Core/Event/Tag.hpp>
#include <N503/Core/Event/Data/ModifierKeyState.hpp>
#include <N503/Core/Event/Data/MouseButton.hpp>
#include <N503/Core/Event/Data/MouseLocation.hpp>

// C++ Standard Libraries
#include <cstdint>

namespace N503::Core::Event::Data
{

    /// @brief
    struct MouseWheelEventData final
    {
        /// @brief 
        static constexpr ::N503::Core::Event::Tag Tag = ::N503::Core::Event::Tag::Mouse;

        /// @brief
        MouseLocation Location;

        /// @brief
        MouseButton Button;

        /// @brief
        ModifierKeyState KeyState;

        /// @brief
        struct
        {
            /// @brief
            struct
            {
                float X = 0.0f;
                float Y = 0.0f;
            } Value;

            /// @brief
            struct
            {
                std::int32_t X = 0;
                std::int32_t Y = 0;
            } Raw;
        } Delta;
    };

} // namespace N503::Core::Event::Data
