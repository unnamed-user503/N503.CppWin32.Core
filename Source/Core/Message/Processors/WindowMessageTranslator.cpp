#include "Pch.hpp"
#include "WindowMessageTranslator.hpp"

// Internal Project
#include "../../CoreEngine.hpp"
#include "MessageContext.hpp"
#include "MessageDelegate.hpp"
#include <N503/Core/Event/Data/WindowActivateEventData.hpp>
#include <N503/Core/Event/Data/WindowCloseEventData.hpp>
#include <N503/Core/Event/Data/WindowMoveEventData.hpp>
#include <N503/Core/Event/Data/WindowSizeEventData.hpp>

// C++ Standard Libraries
#include <cstdint>
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
    auto WindowMessageTranslator::Process(MessageContext& context) -> bool
    {
        const std::tuple<
            MessageDelegate<std::integral_constant<UINT, WM_MOVE>,     &WindowMessageTranslator::OnMove>,
            MessageDelegate<std::integral_constant<UINT, WM_SIZE>,     &WindowMessageTranslator::OnSize>,
            MessageDelegate<std::integral_constant<UINT, WM_ACTIVATE>, &WindowMessageTranslator::OnActivate>,
            MessageDelegate<std::integral_constant<UINT, WM_CLOSE>,    &WindowMessageTranslator::OnClose>
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
    /// @return
    auto WindowMessageTranslator::OnActivate(MessageContext& context) -> bool
    {
        const bool isActive = (LOWORD(context.WParam) != WA_INACTIVE);
        const bool isMinimized = (HIWORD(context.WParam) != 0);

        auto eventData = Event::Data::WindowActivateEventData{ .IsActive = isActive, .IsMinimized = isMinimized };

        CoreEngine::Instance().GetEventQueue().PushOrUpdate(context.Target, std::move(eventData));

        return false;
    }

    /// @brief 
    /// @param context 
    /// @return 
    auto WindowMessageTranslator::OnClose(MessageContext& context) -> bool
    {
        // メインスレッド（ユーザー側）に「閉じたいリクエスト」を投げる
        auto eventData = Event::Data::WindowCloseEventData{};

        CoreEngine::Instance().GetEventQueue().Push(context.Target, std::move(eventData));

        return false;
    }

    /// @brief
    /// @param context
    /// @return
    auto WindowMessageTranslator::OnMove(MessageContext& context) -> bool
    {
        // 座標の抽出 (x, y はクライアント領域ではなくスクリーン座標または親ウィンドウ相対)
        const int x = static_cast<int>(static_cast<short>(LOWORD(context.LParam)));
        const int y = static_cast<int>(static_cast<short>(HIWORD(context.LParam)));

        auto eventData = Event::Data::WindowMoveEventData{ .X = x, .Y = y };

        CoreEngine::Instance().GetEventQueue().PushOrUpdate(context.Target, std::move(eventData));

        return false;
    }

    /// @brief
    /// @param context
    /// @return
    auto WindowMessageTranslator::OnSize(MessageContext& context) -> bool
    {
        const uint32_t width  = static_cast<uint32_t>(LOWORD(context.LParam));
        const uint32_t height = static_cast<uint32_t>(HIWORD(context.LParam));
        const UINT resizeType = static_cast<UINT>(context.WParam); // SIZE_MAXIMIZED 等

        auto eventData = Event::Data::WindowSizeEventData{ .Width = width, .Height = height, .Type = resizeType };

        CoreEngine::Instance().GetEventQueue().PushOrUpdate(context.Target, std::move(eventData));

        return false;
    }

} // namespace N503::Core::Message::Processors
