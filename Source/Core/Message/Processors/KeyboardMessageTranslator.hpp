#pragma once

// Internal Project
#include "MessageContext.hpp"
#include <N503/Core/Event/Data/ModifierKeyState.hpp>

namespace N503::Core::Message::Processors
{

    /// @brief
    class KeyboardMessageTranslator final
    {
        using ModifierKeyState = Event::Data::ModifierKeyState;

    public:
        /// @brief
        KeyboardMessageTranslator() = default;

        /// @brief
        ~KeyboardMessageTranslator() = default;

    public:
        /// @brief
        /// @param context
        /// @return
        auto Process(MessageContext& context) -> bool;

    private:
        /// @brief 
        void UpdateModifierState();

        /// @brief 
        /// @param context 
        /// @return 
        auto OnKey(MessageContext&) -> bool;

        /// @brief 
        /// @param context 
        /// @return 
        auto OnChar(MessageContext&) -> bool;

        /// @brief 
        /// @param context 
        /// @return 
        auto OnFocus(MessageContext&) -> bool;

    private:
        /// @brief キーボードの状態スナップショット
        struct RawInputSnapshot
        {
            /// @brief 修飾キー（Shift, Ctrl, Alt）の状態
            ModifierKeyState KeyState{ ModifierKeyState::None };
        };

        RawInputSnapshot m_Latest{};
    };

} // namespace N503::Core::Message::Processors
