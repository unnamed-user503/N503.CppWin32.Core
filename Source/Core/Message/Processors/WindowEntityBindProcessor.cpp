#include "Pch.hpp"
#include "WindowEntityBindProcessor.hpp"

// Internal Project
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
    auto WindowEntityBindProcessor::Process(MessageContext& context) -> bool
    {
        static constexpr std::tuple<
            MessageDelegate<std::integral_constant<UINT, WM_NCCREATE>,  &WindowEntityBindProcessor::OnNCCreate>,
            MessageDelegate<std::integral_constant<UINT, WM_NCDESTROY>, &WindowEntityBindProcessor::OnNCDestroy>
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
    auto WindowEntityBindProcessor::OnNCCreate(MessageContext& context) -> bool
    {
        if (auto* pBlueprint = reinterpret_cast<CREATESTRUCTW*>(context.LParam))
        {
            ::SetWindowLongPtrW(context.Target, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pBlueprint->lpCreateParams));
        }

        return false;
    }

    /// @brief
    /// @param context
    /// @return
    auto WindowEntityBindProcessor::OnNCDestroy(MessageContext& context) -> bool
    {
        ::SetWindowLongPtrW(context.Target, GWLP_USERDATA, 0);
        return false;
    }

} // namespace N503::Core::Message::Processors
