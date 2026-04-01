#include "Pch.hpp"
#include "CoreEngine.hpp"

// Internal Project
#include "Command/CommandDispatcher.hpp"
#include "Command/CommandQueue.hpp"
#include "Message/MessageDispatcher.hpp"
#include "Event/EventDispatcher.hpp"

// External Project

// C++ Standard Libraries
#include <memory>
#include <stop_token>
#include <thread>

// Platform/Thirdparty Libraries
#include <Windows.h>
#include <utility>

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
        m_CommandQueue  = std::make_unique<CommandQueue>();
        m_EventQueue    = std::make_unique<EventQueue>();
        m_MessageRouter = std::make_unique<DefaultMessageRouter>();

        m_UIThread = std::jthread([this](std::stop_token stopToken)
        {
            this->Run(std::move(stopToken));
        });
    }

    /// @brief 
    /// @param stopToken 
    /// @return 
    auto CoreEngine::Run(std::stop_token stopToken) -> void
    {
        using CommandDispatcher = Command::CommandDispatcher;
        using MessageDispatcher = Message::MessageDispatcher;
        using EventDispatcher   = Event::EventDispatcher;

        CommandDispatcher commandDispatcher;
        MessageDispatcher messageDispatcher;
        EventDispatcher   eventDispatcher;

        auto wakeupHandles = { m_CommandQueue->GetWakeupEventHandle(), m_EventQueue->GetWakeupEventHandle() };

        while (!stopToken.stop_requested())
        {
            // Step 1: 
            auto result = ::MsgWaitForMultipleObjectsEx(static_cast<DWORD>(wakeupHandles.size()), wakeupHandles.begin(), INFINITE, QS_ALLINPUT, MWMO_INPUTAVAILABLE);

            // Step 2: 
            commandDispatcher.Dispatch(*m_CommandQueue.get());

            // Step 3:
            if (!messageDispatcher.Dispatch())
            {
                break;
            }

            // Step 4:
            eventDispatcher.Dispatch(*m_EventQueue.get());
        }

        // --- ここからが「残務処理」フェーズ ---

        // UIスレッドが終了したとしても、メインスレッド側のキューには
        // 最後に投げられたイベントやコマンドが残っている可能性がある

        // 1. 最後にコマンドを一度だけさらっておく（Stop処理に伴うコマンドがある場合）
        commandDispatcher.Dispatch(*m_CommandQueue.get());

        // 2. イベントを「空になるまで」あるいは「一回だけ」掃き出す
        // UIスレッドが既に止まっている（あるいは止まる直前）なら、
        // これ以上新しいイベントは増えないはずなので、安全に掃き出せます
        eventDispatcher.Dispatch(*m_EventQueue.get());
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
