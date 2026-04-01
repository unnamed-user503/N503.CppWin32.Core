#include "Pch.hpp"
#include "WindowLifecycleTracker.hpp"

// Internal Project
#include "../../CoreEngine.hpp"
#include "../../Command/Packets/StopEngineCommand.hpp"
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

    /// @brief
    /// @param context
    /// @return
    auto WindowLifecycleTracker::Process(MessageContext& context) -> bool
    {
        static constexpr std::tuple<
            MessageDelegate<std::integral_constant<UINT, WM_NCCREATE>,  &WindowLifecycleTracker::OnNCCreate>,
            MessageDelegate<std::integral_constant<UINT, WM_NCDESTROY>, &WindowLifecycleTracker::OnNCDestroy>
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
    auto WindowLifecycleTracker::OnNCCreate(MessageContext& context) -> bool
    {
        m_WindowPopulationCount++;
        return false;
    }

    /// @brief
    /// @param context
    /// @return
    auto WindowLifecycleTracker::OnNCDestroy(MessageContext& context) -> bool
    {
        m_WindowPopulationCount--;

        if (m_WindowPopulationCount == 0)
        {
            auto packet = Command::Packets::StopEngineCommand
            {
                .ExitCode = 0
            };

            CoreEngine::Instance().GetCommandQueue().Push(std::move(packet));
        }

        return false;
    }

} // namespace N503::Core::Message::Processors
