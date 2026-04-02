#include "Pch.hpp"
#include "MessageInterruptProcessor.hpp"

// Internal Project
#include "../../CoreEngine.hpp"
#include "../../Command/CommandDispatcher.hpp"
#include "MessageContext.hpp"
#include "MessageDelegate.hpp"

// C++ Standard Libraries
#include <tuple>
#include <type_traits>
#include <utility>

// Platform/Thirdparty Libraries
#include <Windows.h>

namespace N503::Core::Message::Processors
{

    namespace
    {
        /// @brief 
        /// @return 
        auto CommandExecuteImmediate() -> void
        {
            auto& commandQueue    = CoreEngine::Instance().GetCommandQueue();
            auto& commandExecutor = CoreEngine::Instance().GetCommandExecutor();

            if (commandQueue.IsEmpty())
            {
                return;
            }

            Command::CommandDispatcher{}.Dispatch(commandQueue, commandExecutor);
        }

        /// @brief 
        /// @param code 
        /// @param wParam 
        /// @param lParam 
        /// @return 
        auto CALLBACK OSMessageCallback(int code, WPARAM wParam, LPARAM lParam) -> LRESULT
        {
            if (code == MSGF_DIALOGBOX || code == MSGF_MENU || code == MSGF_SCROLLBAR)
            {
                // 割り込み発生時に即座にキューを空にする
                CommandExecuteImmediate();
            }

            return ::CallNextHookEx(nullptr, code, wParam, lParam);
        }
    }

    /// @brief
    /// @param context
    /// @return
    auto MessageInterruptProcessor::Process(MessageContext& context) -> bool
    {
        // 静的パイプライン定義
        static constexpr std::tuple<
            MessageDelegate<std::integral_constant<UINT, WM_ENTERSIZEMOVE>, &MessageInterruptProcessor::OnEnterSizeMove>,
            MessageDelegate<std::integral_constant<UINT, WM_TIMER>,         &MessageInterruptProcessor::OnTimer>,
            MessageDelegate<std::integral_constant<UINT, WM_EXITSIZEMOVE>,  &MessageInterruptProcessor::OnExitSizeMove>,
            MessageDelegate<std::integral_constant<UINT, WM_ENTERIDLE>,     &MessageInterruptProcessor::OnEnterIdle>
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
    auto MessageInterruptProcessor::OnEnterSizeMove(MessageContext& context) -> bool
    {
        // WM_TIMER による心拍維持 (16ms)
        m_TimerID = ::SetTimer(context.Target, 0x1001, 16, nullptr);

        // WH_MSGFILTER による高精度な割り込みフック
        m_HookHandle.reset(::SetWindowsHookEx(WH_MSGFILTER, OSMessageCallback, nullptr, ::GetCurrentThreadId()));

        return false;
    }

    /// @brief 
    /// @param context 
    /// @return 
    auto MessageInterruptProcessor::OnTimer(MessageContext& context) -> bool
    {
        if (context.WParam == 0x1001)
        {
            CommandExecuteImmediate();
        }

        return false;
    }

    /// @brief 
    /// @param context 
    /// @return 
    auto MessageInterruptProcessor::OnExitSizeMove(MessageContext& context) -> bool
    {
        m_HookHandle.reset();

        ::KillTimer(context.Target, m_TimerID);
        m_TimerID = 0;

        return false;
    }

    /// @brief 
    /// @param context 
    /// @return 
    auto MessageInterruptProcessor::OnEnterIdle(MessageContext& context) -> bool
    {
        CommandExecuteImmediate();
        return false;
    }

} // namespace N503::Core::Message::Processors
