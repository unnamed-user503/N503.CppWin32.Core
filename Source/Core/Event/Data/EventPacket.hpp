#pragma once

// Internal Project
#include <N503/Core/Event/Data/WindowActivateEventData.hpp>
#include <N503/Core/Event/Data/WindowCloseEventData.hpp>
#include <N503/Core/Event/Data/WindowMoveEventData.hpp>
#include <N503/Core/Event/Data/WindowSizeEventData.hpp>

#include <N503/Core/Event/Data/MouseButtonPressEventData.hpp>
#include <N503/Core/Event/Data/MouseButtonReleaseEventData.hpp>
#include <N503/Core/Event/Data/MouseMoveEventData.hpp>
#include <N503/Core/Event/Data/MouseWheelEventData.hpp>

#include <N503/Core/Event/Data/KeyboardButtonPressEventData.hpp>
#include <N503/Core/Event/Data/KeyboardButtonReleaseEventData.hpp>
#include <N503/Core/Event/Data/KeyboardCharEventData.hpp>

// C++ Standard Libraries
#include <variant>

namespace N503::Core::Event::Data
{

    /// @brief 
    using EventPacket = std::variant<
        WindowActivateEventData,
        WindowCloseEventData,
        WindowMoveEventData,
        WindowSizeEventData,
        MouseButtonPressEventData,
        MouseButtonReleaseEventData,
        MouseMoveEventData,
        MouseWheelEventData,
        KeyboardButtonPressEventData,
        KeyboardButtonReleaseEventData,
        KeyboardCharEventData
    >;

} // namespace N503::Core::Event::Data
