#include "Pch.hpp"
#include "CoreEngine.hpp"

// Internal Project
#include "Command/CommandDispatcher.hpp"
#include "Command/CommandQueue.hpp"
#include "Event/EventDispatcher.hpp"
#include "Message/MessageDispatcher.hpp"

// C++ Standard Libraries
#include <atomic>
#include <memory>
#include <semaphore>
#include <stop_token>
#include <thread>
#include <utility>

#ifdef _DEBUG
#include <cassert>
#endif

// Platform/Thirdparty Libraries
#include <Windows.h>

namespace N503::Core
{

    /// @brief 
    /// @return 
    auto CoreEngine::Instance() -> CoreEngine&
    {
        static CoreEngine coreEngine;
        return coreEngine;
    }

    /// @brief 
    CoreEngine::CoreEngine()
    {
        m_CommandQueue    = std::make_unique<CommandQueue>();
        m_CommandExecutor = std::make_unique<CommandExecutor>();
        m_EventQueue      = std::make_unique<EventQueue>();
        m_MessageRouter   = std::make_unique<DefaultMessageRouter>();

        std::binary_semaphore signal{ 0 };

        m_UIThread = std::jthread([this, &signal](std::stop_token stopToken)
        {
            signal.release();
            this->Run(std::move(stopToken));
        });

        signal.acquire(); // スレッドが開始されるのを待つ
    }

    /// @brief 
    /// @param stopToken 
    /// @return 
    auto CoreEngine::Run(std::stop_token stopToken) -> void
    {
        // スレッドが実行中の状態を表す
        static std::atomic_flag isRunning = ATOMIC_FLAG_INIT;

        // スレッドが実行中の状態の旗が立っていた場合何もしないで帰る
#ifdef _DEBUG
        if (isRunning.test_and_set(std::memory_order_acquire))
        {
            assert(!"CoreEngine::Run is already running!");
            return;
        }
#else
        if (isRunning.test_and_set(std::memory_order_acquire)) return;
#endif

        // イベントスレッドの起動
        m_EventThread = std::jthread([this](std::stop_token stopToken)
        {
            using EventDispatcher = Event::EventDispatcher;

            EventDispatcher eventDispatcher;

            auto wakeupHandles = { m_EventQueue->GetWakeupEventHandle() };

            while (!stopToken.stop_requested())
            {
                auto result = ::WaitForMultipleObjects(static_cast<DWORD>(wakeupHandles.size()), wakeupHandles.begin(), TRUE, INFINITE);

                eventDispatcher.Dispatch(*m_EventQueue.get());
            }
        });

        // コマンドキューとOSメッセージの処理
        using CommandDispatcher = Command::CommandDispatcher;
        using MessageDispatcher = Message::MessageDispatcher;

        CommandDispatcher commandDispatcher;
        MessageDispatcher messageDispatcher;

        auto wakeupHandles = { m_CommandQueue->GetWakeupEventHandle() };

        while (!stopToken.stop_requested())
        {
            auto result = ::MsgWaitForMultipleObjectsEx(static_cast<DWORD>(wakeupHandles.size()), wakeupHandles.begin(), INFINITE, QS_ALLINPUT, MWMO_INPUTAVAILABLE);

            commandDispatcher.Dispatch(*m_CommandQueue.get(), *m_CommandExecutor.get());

            if (!messageDispatcher.Dispatch())
            {
                break;
            }
        }

        // イベントスレッドに終了したい事を通知する
        m_EventThread.request_stop();
        ::SetEvent(m_EventQueue->GetWakeupEventHandle());

        // イベントスレッドが終了するまで待機する
        if (m_EventThread.joinable())
        {
            m_EventThread.join();
        }
    }

    /// @brief 
    /// @return 
    auto CoreEngine::AwaitUIThreadCompletion() -> void
    {
        if (m_UIThread.joinable())
        {
            m_UIThread.join();
        }
    }

    /// @brief 
    /// @return 
    auto CoreEngine::GetCommandQueue() -> CommandQueue&
    {
        return *m_CommandQueue;
    }

    /// @brief 
    /// @return 
    auto CoreEngine::GetCommandExecutor() -> CommandExecutor&
    {
        return *m_CommandExecutor;
    }

    /// @brief 
    /// @return 
    auto CoreEngine::GetEventQueue() -> EventQueue&
    {
        return *m_EventQueue;
    }

    /// @brief 
    /// @return 
    auto CoreEngine::GetDefaultMessageRouter() -> DefaultMessageRouter&
    {
        return *m_MessageRouter;
    }

} // namespace N503::System
