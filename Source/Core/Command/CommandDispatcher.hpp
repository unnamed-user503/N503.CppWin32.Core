#pragma once

// Internal Project
#include "CommandQueue.hpp"
#include "CommandExecutor.hpp"

namespace N503::Core::Command
{

    /// @brief 
    class CommandDispatcher final
    {
    public:
        /// @brief 
        CommandDispatcher() = default;

        /// @brief 
        ~CommandDispatcher() = default;

        /// @brief 
        /// @param  
        CommandDispatcher(const CommandDispatcher&) = delete;

        /// @brief 
        /// @param  
        /// @return 
        auto operator=(const CommandDispatcher&) -> CommandDispatcher& = delete;

        /// @brief 
        /// @param  
        CommandDispatcher(CommandDispatcher&&) = delete;

        /// @brief 
        /// @param  
        /// @return 
        auto operator=(CommandDispatcher&&) -> CommandDispatcher& = delete;

    public:
        /// @brief 
        /// @param  
        /// @param  
        /// @return 
        auto Dispatch(CommandQueue&, CommandExecutor&) -> void;
    };

} // namespace N503::Core
