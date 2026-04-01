#pragma once

// Internal Project
#include "Packets/CreateWindowCommand.hpp"
#include "Packets/DestroyWindowCommand.hpp"
#include "Packets/SetWindowTitleCommand.hpp"
#include "Packets/StopEngineCommand.hpp"

namespace N503::Core::Command
{

    /// @brief 
    class CommandExecutor final
    {
    public:
        /// @brief 
        CommandExecutor() = default;

        /// @brief 
        ~CommandExecutor() = default;

    public:
        /// @brief 
        /// @param command 
        auto operator()(const Packets::CreateWindowCommand& command) -> void;

        /// @brief 
        /// @param command 
        auto operator()(const Packets::DestroyWindowCommand& command) -> void;

        /// @brief 
        /// @param command 
        auto operator()(const Packets::SetWindowTitleCommand& command) -> void;

        /// @brief 
        /// @param command 
        auto operator()(const Packets::StopEngineCommand& command) -> void;

        /// @brief 
        /// @tparam T 
        /// @param command 
        /// @return 
        template<class T>
        auto operator()(const T& command) -> void { /* nop */ };
    };

} // namespace N503::Core
