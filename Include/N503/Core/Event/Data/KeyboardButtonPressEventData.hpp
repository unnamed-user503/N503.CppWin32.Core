#pragma once

// Internal Project
#include <N503/Core/Event/Tag.hpp>
#include <N503/Core/Event/Data/ModifierKeyState.hpp>

// C++ Standard Libraries
#include <cstdint>

namespace N503::Core::Event::Data
{

    /// @brief
    struct KeyboardButtonPressEventData final
    {
        /// @brief 
        static constexpr ::N503::Core::Event::Tag Tag = ::N503::Core::Event::Tag::Keyboard;

        /// @brief
        std::uint32_t KeyCode;

        /// @brief
        bool IsRepeat;

        /// @brief
        ModifierKeyState KeyState;
    };

} // namespace N503::Core::Event::Data
