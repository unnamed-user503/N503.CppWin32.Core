#include "Pch.hpp"
#include "MouseMessageTranslator.hpp"

// Internal Project
#include "../../CoreEngine.hpp"
#include "MessageContext.hpp"
#include "MessageDelegate.hpp"
#include <N503/Core/Event/Data/ModifierKeyState.hpp>
#include <N503/Core/Event/Data/MouseButton.hpp>
#include <N503/Core/Event/Data/MouseButtonPressEventData.hpp>
#include <N503/Core/Event/Data/MouseButtonReleaseEventData.hpp>
#include <N503/Core/Event/Data/MouseMoveEventData.hpp>
#include <N503/Core/Event/Data/MouseWheelEventData.hpp>

// C++ Standard Libraries
#include <cstdint>
#include <tuple>
#include <type_traits>
#include <utility>

// Platform/Thirdparty Libraries
#include <Windows.h>
#include <windowsx.h>

namespace N503::Core::Message::Processors
{

    /// @brief
    namespace
    {
        constexpr float WheelDeltaMagnitude = static_cast<float>(WHEEL_DELTA);
    }

    /// @brief
    /// @param context
    /// @return
    auto MouseMessageTranslator::Process(MessageContext& context) -> bool
    {
        // 範囲外なら即終了
        if (context.Message < WM_MOUSEFIRST || context.Message > WM_MOUSELAST)
        {
            return false;
        }

        // 共通の前処理：Snapshotの更新
        if (context.Message != WM_MOUSELEAVE)
        {
            UpdateSnapshot(context);
        }

        // 静的パイプライン定義
        static constexpr std::tuple<
            MessageDelegate<std::integral_constant<UINT, WM_MOUSEMOVE>,   &MouseMessageTranslator::OnMouseMove>,
            MessageDelegate<std::integral_constant<UINT, WM_MOUSELEAVE>,  &MouseMessageTranslator::OnMouseLeave>,
            MessageDelegate<std::integral_constant<UINT, WM_LBUTTONDOWN>, &MouseMessageTranslator::OnMouseButton>,
            MessageDelegate<std::integral_constant<UINT, WM_LBUTTONUP>,   &MouseMessageTranslator::OnMouseButton>,
            MessageDelegate<std::integral_constant<UINT, WM_RBUTTONDOWN>, &MouseMessageTranslator::OnMouseButton>,
            MessageDelegate<std::integral_constant<UINT, WM_RBUTTONUP>,   &MouseMessageTranslator::OnMouseButton>,
            MessageDelegate<std::integral_constant<UINT, WM_MBUTTONDOWN>, &MouseMessageTranslator::OnMouseButton>,
            MessageDelegate<std::integral_constant<UINT, WM_MBUTTONUP>,   &MouseMessageTranslator::OnMouseButton>,
            MessageDelegate<std::integral_constant<UINT, WM_XBUTTONDOWN>, &MouseMessageTranslator::OnMouseButton>,
            MessageDelegate<std::integral_constant<UINT, WM_XBUTTONUP>,   &MouseMessageTranslator::OnMouseButton>,
            MessageDelegate<std::integral_constant<UINT, WM_MOUSEWHEEL>,  &MouseMessageTranslator::OnMouseWheel>,
            MessageDelegate<std::integral_constant<UINT, WM_MOUSEHWHEEL>, &MouseMessageTranslator::OnMouseWheel>
        > pipeline;

        return std::apply(
            [&](auto&... delegates)
            {
                return (... || (context.Message == std::remove_cvref_t<decltype(delegates)>::MessageID::value && delegates(this, context)));
            },
            pipeline);
    }

    /// @brief
    /// @param context
    void MouseMessageTranslator::UpdateSnapshot(const MessageContext& context)
    {
        const bool isWheel = (context.Message == WM_MOUSEWHEEL || context.Message == WM_MOUSEHWHEEL);

        // 1. 座標更新 (ホイール以外)
        if (!isWheel)
        {
            m_Latest.Location = { .X = static_cast<std::int32_t>(GET_X_LPARAM(context.LParam)), .Y = static_cast<std::int32_t>(GET_Y_LPARAM(context.LParam)) };
        }

        // 2. ModifierKeyState 更新
        const WORD flags = isWheel ? LOWORD(context.WParam) : static_cast<WORD>(context.WParam);

        ModifierKeyState state = ModifierKeyState::None;
        if (flags & MK_LBUTTON)
        {
            state = state | ModifierKeyState::LButton;
        }
        if (flags & MK_RBUTTON)
        {
            state = state | ModifierKeyState::RButton;
        }
        if (flags & MK_MBUTTON)
        {
            state = state | ModifierKeyState::MButton;
        }
        if (flags & MK_CONTROL)
        {
            state = state | ModifierKeyState::Control;
        }
        if (flags & MK_SHIFT)
        {
            state = state | ModifierKeyState::Shift;
        }
        if (::GetKeyState(VK_MENU) < 0)
        {
            state = state | ModifierKeyState::Alt;
        }
        if (::GetKeyState(VK_LWIN) < 0 || ::GetKeyState(VK_RWIN) < 0)
        {
            state = state | ModifierKeyState::Super;
        }

        m_Latest.KeyState = state;
    }

    /// @brief
    /// @param context
    /// @return
    auto MouseMessageTranslator::OnMouseMove(MessageContext& context) -> bool
    {
        if (!m_Latest.IsInside)
        {
            TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, context.Target };
            ::TrackMouseEvent(&tme);
            m_Latest.IsInside = true;
        }

        CoreEngine::Instance().GetEventQueue().PushOrUpdate(context.Target, Event::Data::MouseMoveEventData{ .Location = m_Latest.Location, .KeyState = m_Latest.KeyState });
        return false;
    }

    /// @brief
    /// @param context
    /// @return
    auto MouseMessageTranslator::OnMouseLeave(MessageContext& context) -> bool
    {
        m_Latest.IsInside = false;
        // EventQueue への Push 処理など
        return false;
    }

    /// @brief
    /// @param context
    /// @return
    auto MouseMessageTranslator::OnMouseButton(MessageContext& context) -> bool
    {
        auto button = Event::Data::MouseButton::None;

        switch (context.Message)
        {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
            button = Event::Data::MouseButton::Left;
            break;

        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            button = Event::Data::MouseButton::Right;
            break;

        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            button = Event::Data::MouseButton::Middle;
            break;

        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
            button = (GET_XBUTTON_WPARAM(context.WParam) == XBUTTON1) ? Event::Data::MouseButton::X1 : Event::Data::MouseButton::X2;
            break;
        }

        const bool isPressed = (context.Message == WM_LBUTTONDOWN || context.Message == WM_RBUTTONDOWN || context.Message == WM_MBUTTONDOWN || context.Message == WM_XBUTTONDOWN);

        if (isPressed)
        {
            auto eventData = Event::Data::MouseButtonPressEventData{ .Location = m_Latest.Location, .Button = button, .KeyState = m_Latest.KeyState };

            CoreEngine::Instance().GetEventQueue().Push(context.Target, std::move(eventData));
        }
        else
        {
            auto eventData = Event::Data::MouseButtonReleaseEventData{ .Location = m_Latest.Location, .Button = button, .KeyState = m_Latest.KeyState };

            CoreEngine::Instance().GetEventQueue().Push(context.Target, std::move(eventData));
        }

        return false;
    }

    /// @brief
    /// @param context
    /// @return
    auto MouseMessageTranslator::OnMouseWheel(MessageContext& context) -> bool
    {
        const bool isVertical = (context.Message == WM_MOUSEWHEEL);

        // 1. OSからの生の移動量を取得 (WHEEL_DELTA 単位)
        const auto rawDelta = static_cast<std::int16_t>(HIWORD(context.WParam));
        const float value = static_cast<float>(rawDelta) / WheelDeltaMagnitude;

        // 2. 座標補正 (WM_MOUSEWHEEL はスクリーン座標系のため)
        // Snapshot の Location は通常クライアント座標を期待するため、ここで補正をかけます
        POINT point = { .x = GET_X_LPARAM(context.LParam), .y = GET_Y_LPARAM(context.LParam) };
        ::ScreenToClient(context.Target, &point);

        MouseLocation currentLocation = { .X = point.x, .Y = point.y };

        // 3. イベントキューに既存のホイールイベントがあれば「借用」して累積する
        if (auto event = CoreEngine::Instance().GetEventQueue().TryBorrowBack<Event::Data::MouseWheelEventData>())
        {
            // 座標とキー状態を最新に更新
            event->Location = currentLocation;
            event->KeyState = m_Latest.KeyState;
            event->Button = Event::Data::MouseButton::None;

            if (isVertical)
            {
                event->Delta.Value.Y += value;
                event->Delta.Raw.Y += static_cast<std::int32_t>(rawDelta);
            }
            else
            {
                event->Delta.Value.X += value;
                event->Delta.Raw.X += static_cast<std::int32_t>(rawDelta);
            }

            return false; // 累積成功
        }

        // 4. 新規イベントを生成して PushOrUpdate
        auto eventData = Event::Data::MouseWheelEventData{ .Location = currentLocation, .Button = Event::Data::MouseButton::None, .KeyState = m_Latest.KeyState, .Delta = { .Value = { .X = !isVertical ? value : 0.0f, .Y = isVertical ? value : 0.0f }, .Raw = { .X = !isVertical ? static_cast<std::int32_t>(rawDelta) : 0, .Y = isVertical ? static_cast<std::int32_t>(rawDelta) : 0 } } };

        CoreEngine::Instance().GetEventQueue().PushOrUpdate(context.Target, std::move(eventData));

        return false;
    }

} // namespace N503::Core::Message::Processors
