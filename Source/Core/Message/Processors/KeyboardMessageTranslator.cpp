#include "Pch.hpp"
#include "KeyboardMessageTranslator.hpp"

// Internal Project
#include "../../CoreEngine.hpp"
#include "MessageContext.hpp"
#include "MessageDelegate.hpp"
#include <N503/Core/Event/Data/KeyboardButtonPressEventData.hpp>
#include <N503/Core/Event/Data/KeyboardButtonReleaseEventData.hpp>
#include <N503/Core/Event/Data/KeyboardCharEventData.hpp>

// C++ Standard Libraries
#include <cstdint>
#include <format>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

// Platform/Thirdparty Libraries
#include <Windows.h>

namespace N503::Core::Message::Processors
{

    /// @brief
    /// @param context
    /// @return
    auto KeyboardMessageTranslator::Process(MessageContext& context) -> bool
    {
        // 静的パイプライン定義
        static constexpr std::tuple<
            MessageDelegate<std::integral_constant<UINT, WM_KEYDOWN>,    &KeyboardMessageTranslator::OnKey>,
            MessageDelegate<std::integral_constant<UINT, WM_SYSKEYDOWN>, &KeyboardMessageTranslator::OnKey>,
            MessageDelegate<std::integral_constant<UINT, WM_KEYUP>,      &KeyboardMessageTranslator::OnKey>,
            MessageDelegate<std::integral_constant<UINT, WM_SYSKEYUP>,   &KeyboardMessageTranslator::OnKey>,
            MessageDelegate<std::integral_constant<UINT, WM_CHAR>,       &KeyboardMessageTranslator::OnChar>,
            MessageDelegate<std::integral_constant<UINT, WM_SETFOCUS>,   &KeyboardMessageTranslator::OnFocus>,
            MessageDelegate<std::integral_constant<UINT, WM_KILLFOCUS>,  &KeyboardMessageTranslator::OnFocus>
        > pipeline;

        return std::apply(
            [&](auto&... delegates)
            {
                return (... || (context.Message == std::remove_cvref_t<decltype(delegates)>::MessageID::value && delegates(this, context)));
            },
            pipeline);
    }

    /// @brief
    void KeyboardMessageTranslator::UpdateModifierState()
    {
        auto state = ModifierKeyState::None;
        if (::GetKeyState(VK_CONTROL) < 0)
        {
            state = state | ModifierKeyState::Control;
        }
        if (::GetKeyState(VK_SHIFT) < 0)
        {
            state = state | ModifierKeyState::Shift;
        }
        if (::GetKeyState(VK_MENU) < 0)
        {
            state = state | ModifierKeyState::Alt;
        }
        // OSのSuperキー(Winキー)も取得しておくと便利です
        if (::GetKeyState(VK_LWIN) < 0 || ::GetKeyState(VK_RWIN) < 0)
        {
            state = state | ModifierKeyState::Super;
        }

        m_Latest.KeyState = state;
    }

    /// @brief
    /// @param context
    /// @return
    auto KeyboardMessageTranslator::OnKey(MessageContext& context) -> bool
    {
        UpdateModifierState();

        const bool isPress = (context.Message == WM_KEYDOWN || context.Message == WM_SYSKEYDOWN);
        const auto vKey = static_cast<std::uint32_t>(context.WParam);

        if (isPress)
        {
            // 30bit: Previous key state (1 if the key is down before the message is sent)
            const bool isRepeat = (context.LParam & (1 << 30)) != 0;

            auto eventData = Event::Data::KeyboardButtonPressEventData{ .KeyCode = vKey, .IsRepeat = isRepeat, .KeyState = m_Latest.KeyState };

            CoreEngine::Instance().GetEventQueue().PushOrUpdate(context.Target, std::move(eventData));
        }
        else
        {
            auto eventData = Event::Data::KeyboardButtonReleaseEventData{ .KeyCode = vKey, .KeyState = m_Latest.KeyState };

            CoreEngine::Instance().GetEventQueue().PushOrUpdate(context.Target, std::move(eventData));
        }
        return false;
    }

    /// @brief
    /// @param context
    /// @return
    auto KeyboardMessageTranslator::OnChar(MessageContext& context) -> bool
    {
        UpdateModifierState();

        auto eventData = Event::Data::KeyboardCharEventData{ .CodePoint = static_cast<char32_t>(context.WParam & 0xFFFF), .KeyState = m_Latest.KeyState };

        CoreEngine::Instance().GetEventQueue().PushOrUpdate(context.Target, std::move(eventData));
        return false;
    }

    /// @brief
    /// @param context
    /// @return
    auto KeyboardMessageTranslator::OnFocus(MessageContext& context) -> bool
    {
        const bool hasFocus = (context.Message == WM_SETFOCUS);

#if _DEBUG
        ::OutputDebugStringW(std::format(L"[Keyboard] Focus {}: Update ModifierState (0x{:02X} -> 0x00)\n", hasFocus ? L"Found" : L"Lost", static_cast<std::uint32_t>(m_Latest.KeyState)).data());
#endif

        if (hasFocus)
        {
            UpdateModifierState();
        }
        else
        {
            m_Latest.KeyState = ModifierKeyState::None;
        }

        return false;
    }

} // namespace N503::Core::Message::Processors
