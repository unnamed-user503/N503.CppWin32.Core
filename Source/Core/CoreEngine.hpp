#pragma once

// Internal Project
#include "Command/CommandQueue.hpp"
#include "Command/CommandExecutor.hpp"
#include "Event/EventQueue.hpp"
#include "Message/DefaultMessageRouter.hpp"

// External Project

// C++ Standard Libraries
#include <memory>
#include <stop_token>
#include <thread>

// Platform/Thirdparty Libraries

namespace N503::Core
{

    /// @brief 
    class CoreEngine final
    {
        /// @brief 
        using CommandQueue = Command::CommandQueue;

        /// @brief 
        using CommandExecutor = Command::CommandExecutor;

        /// @brief 
        using EventQueue = Event::EventQueue;

        /// @brief 
        using DefaultMessageRouter = Message::DefaultMessageRouter;

    public:
        /// @brief 
        /// @return 
        static auto Instance() -> CoreEngine&;

    private:
        /// @brief 
        CoreEngine();

    public:
        /// @brief 
        ~CoreEngine() = default;

        /// @brief 
        /// @param  
        CoreEngine(const CoreEngine&) = delete;

        /// @brief 
        /// @param  
        /// @return 
        auto operator=(const CoreEngine&) -> CoreEngine& = delete;

        /// @brief 
        /// @param  
        CoreEngine(CoreEngine&&) = delete;

        /// @brief 
        /// @param  
        /// @return 
        auto operator=(CoreEngine&&) -> CoreEngine& = delete;

    public:
        /// @brief 
        /// @param stopToken 
        /// @return 
        auto Run(std::stop_token stopToken) -> void;

        /// @brief 
        /// @return 
        auto AwaitUIThreadCompletion() -> void;

        /// @brief 
        /// @return 
        auto GetCommandQueue() -> CommandQueue&;

        /// @brief 
        /// @return 
        auto GetCommandExecutor() -> CommandExecutor&;

        /// @brief 
        /// @return 
        auto GetEventQueue() -> EventQueue&;

        /// @brief 
        /// @return 
        auto GetDefaultMessageRouter() -> DefaultMessageRouter&;

    private:
        /// @brief 
        std::unique_ptr<CommandQueue> m_CommandQueue;

        /// @brief 
        std::unique_ptr<CommandExecutor> m_CommandExecutor;

        /// @brief 
        std::unique_ptr<EventQueue> m_EventQueue;

        /// @brief 
        std::unique_ptr<DefaultMessageRouter> m_MessageRouter;

        /// @brief 
        std::jthread m_UIThread;

        /// @brief 
        std::jthread m_EventThread;
    };

} // namespace N503::Core
