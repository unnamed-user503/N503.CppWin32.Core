#pragma once

// Internal Project
#include "MessageContext.hpp"
#include <N503/Core/Event/Data/ModifierKeyState.hpp>
#include <N503/Core/Event/Data/MouseLocation.hpp>
#include <N503/Core/Event/Data/MouseButton.hpp>

namespace N503::Core::Message::Processors
{

    /// @brief
    class MouseMessageTranslator final
    {
        /// @brief 
        using MouseLocation = Event::Data::MouseLocation;

        /// @brief 
        using ModifierKeyState = Event::Data::ModifierKeyState;

    public:
        /// @brief
        MouseMessageTranslator() = default;

        /// @brief
        ~MouseMessageTranslator() = default;

    public:
        /// @brief
        /// @param context
        /// @return
        auto Process(MessageContext& context) -> bool;

    private:
        /// @brief 
        /// @param  
        /// @param  
        void UpdateSnapshot(const MessageContext&);

        /// @brief 
        /// @param  
        auto OnMouseMove(MessageContext&) -> bool;

        /// @brief 
        auto OnMouseLeave(MessageContext&) -> bool;

        /// @brief 
        /// @param  
        /// @param  
        auto OnMouseButton(MessageContext&) -> bool;

        /// @brief 
        /// @param  
        auto OnMouseWheel(MessageContext&) -> bool;

    private:
        /// @brief 
        struct RawInputSnapshot
        {
            /// @brief 
            MouseLocation Location{};

            /// @brief 
            ModifierKeyState KeyState{ ModifierKeyState::None };

            /// @brief 
            bool IsInside{ false };
        };

        /// @brief 
        RawInputSnapshot m_Latest;
    };

} // namespace N503::Core::Message::Processors
