#pragma once

// Internal Project
#include <N503/Core/Event/Tag.hpp>
#include <N503/Core/Event/Data/ModifierKeyState.hpp>
#include <N503/Core/Event/Data/MouseButton.hpp>
#include <N503/Core/Event/Data/MouseLocation.hpp>

namespace N503::Core::Event::Data
{

    /// @brief
    struct MouseButtonPressEventData final
    {
        /// @brief 
        static constexpr ::N503::Core::Event::Tag Tag = ::N503::Core::Event::Tag::Mouse;

        /// @brief
        MouseLocation Location;

        /// @brief
        MouseButton Button;

        /// @brief
        ModifierKeyState KeyState;
    };

} // namespace N503::Core::Event::Data
