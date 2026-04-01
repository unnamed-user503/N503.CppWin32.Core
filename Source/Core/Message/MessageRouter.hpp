#pragma once

// Internal Project
#include "Processors/MessageContext.hpp"

// External Project

// C++ Standard Libraries
#include <tuple>
#include <utility>

// Platform/Thirdparty Libraries
#include <Windows.h>

namespace N503::Core::Message
{

    /// @brief
    /// @tparam ...Processors
    template <typename... TProcessors> class MessageRouter final
    {
    public:
        /// @brief
        MessageRouter() = default;

        /// @brief
        ~MessageRouter() = default;

    public:
        /// @brief
        /// @param target
        /// @param message
        /// @param wParam
        /// @param lParam
        /// @return
        auto Route(HWND target, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT
        {
            auto context = Processors::MessageContext
            {
                .Target  = target,
                .Message = message,
                .WParam  = wParam,
                .LParam  = lParam
            };

            // C++17 畳み込み式 (Fold Expression) による静的ループ展開
            // processor.Process(context) が順次実行され、一つでも false を返すと短絡評価により停止する
            std::apply(
                [&context](auto&... processor)
                {
                    (..., (processor.Process(context) ? true : false));
                },
                m_Pipeline
            );

            // プロセッサのいずれかが「完了」とマークした場合はその結果を返す
            if (context.Handled)
            {
                return context.Result;
            }

            // 誰も処理しなかった、あるいはデフォルト処理が必要な場合
            return ::DefWindowProcW(target, message, wParam, lParam);
        }

    private:
        /// @brief
        std::tuple<TProcessors...> m_Pipeline;
    };

} // namespace N503::Core::Message
